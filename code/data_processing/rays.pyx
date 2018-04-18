from __future__ import division
import numpy as np
import datapool as dp
cimport numpy as np
cimport cython

DOUBLE = '<f8'
INT = '<i8'
ctypedef np.float64_t DOUBLE_t
ctypedef np.uint8_t BOOL_t
ctypedef np.int_t INT_t

cdef:
    int DATA_RAYS = 2084
    int NUM_LASERS = 64
    DOUBLE_t HORIZONTAL_SHIFT = np.radians(0.1728)
    DOUBLE_t Y_WEDGE_ANG = 2 * HORIZONTAL_SHIFT/3
    DOUBLE_t MAX_SQ_LASERDIST = 0.01

    DOUBLE_t MAX_ZANGLE = np.radians(2.5)
    DOUBLE_t MIN_ZANGLE = np.radians(-25)

    DOUBLE_t MIN_RAY_FAR = 0.9
    DOUBLE_t MAX_RAY_FAR = 131.0

    tuple DATA_SHAPE = (NUM_LASERS, DATA_RAYS, 3)

    np.ndarray RAYS_VERT = np.radians(np.concatenate((np.linspace(2, (-8 - 1./3), 32, dtype=DOUBLE), np.linspace((-8 - 1./3 - 1./2), (-24 - 1./3), 32, dtype=DOUBLE))))
    np.ndarray VERT_ROTMAT = np.array([[[np.cos(angle), 0, -np.sin(angle)], [0, 1, 0], [np.sin(angle), 0, np.cos(angle)]] for angle in RAYS_VERT], dtype=DOUBLE)
    np.ndarray RAYS_HOR = np.radians(np.arange(0, 360, np.rad2deg(HORIZONTAL_SHIFT)))
    np.ndarray HOR_ROTMAT = np.array([[[np.cos(angle), -np.sin(angle), 0], [np.sin(angle), np.cos(angle), 0], [0, 0, 1]] for angle in RAYS_HOR], dtype=DOUBLE)
    np.ndarray LASER_IDS = np.arange(NUM_LASERS, dtype=INT)


cpdef np.ndarray[DOUBLE_t, ndim=2] gta_cam_rot(np.ndarray[DOUBLE_t, ndim=1] world_rot,
                                              np.ndarray[DOUBLE_t, ndim=1] relative_rot,
                                              bint rads=False):
    cdef:
        np.ndarray[DOUBLE_t, ndim=1] angles, sines, coses
        np.ndarray[DOUBLE_t, ndim=2] Rx, Ry, Rz
    if not rads:
        angles = np.radians(world_rot - relative_rot)
    else:
        angles = world_rot - relative_rot
    sines = np.sin(angles)
    coses = np.cos(angles)
    Rx = np.array([[1, 0, 0], [0, coses[0], -sines[0]], [0, sines[0], coses[0]]], dtype=DOUBLE)
    Ry = np.array([[coses[1], 0, sines[1]], [0, 1, 0], [-sines[1], 0, coses[1]]], dtype=DOUBLE)
    Rz = np.array([[coses[2], -sines[2], 0], [sines[2], coses[2], 0], [0, 0, 1]], dtype=DOUBLE)
    return (Rx @ Ry @ Rz)


cdef tuple unique_argmin(np.ndarray[DOUBLE_t, ndim=2] dists):
    cdef:
        np.ndarray[BOOL_t, ndim=1, cast=True] mask, valid
        np.ndarray[INT_t, ndim=2] sids = np.argsort(dists, axis=1)
        np.ndarray[INT_t, ndim=1] recter = np.zeros((NUM_LASERS,), dtype=INT), dist_ids = np.arange(NUM_LASERS, dtype=INT), counts, inv, uq, which
        int i, winner, pts = dists[0,:].size
    while True:
        valid = (dists[dist_ids, sids[dist_ids, recter]] <= MAX_SQ_LASERDIST).reshape((-1))
        dist_ids = dist_ids[valid]
        recter = recter[valid]
        uq, inv, counts = np.unique(sids[dist_ids,recter], return_inverse=True, return_counts=True)
        mask = counts > 1
        if not mask.any():
            break
        for i in np.where(mask)[0]:
            which = np.where(inv == i)[0]
            winner = np.argmin(dists[dist_ids[which], sids[dist_ids[which], recter[which]]])
            which = which[np.where(which != winner)]
            recter[which] += 1
        dist_ids = dist_ids[recter < pts]
        recter = recter[recter < pts]

    return sids[dist_ids, recter], dist_ids


@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
cdef np.ndarray[DOUBLE_t, ndim=3] _compute_lidar_data(np.ndarray[DOUBLE_t, ndim=2] pcl,
                                                      np.ndarray[DOUBLE_t, ndim=1] intensity,
                                                      np.ndarray[DOUBLE_t, ndim=1] lidar_center,
                                                      np.ndarray[DOUBLE_t, ndim=2] cam_rot):
    cdef:
        np.ndarray[BOOL_t, ndim=1, cast=True] wedge_mask, alive_mask, corr_dist_lasers, full_valid
        np.ndarray[DOUBLE_t, ndim=3] result = np.zeros(DATA_SHAPE), cross_res
        np.ndarray[DOUBLE_t, ndim=1] ray = np.array([1.0, 0, 0]), yangles, ptdists, zangles, inten_copy
        np.ndarray[DOUBLE_t, ndim=2] pcl_copy, cur_rays, sqdists
        np.ndarray[INT_t, ndim=1] min_ids, valid_lasers
        int i
    pcl_copy = (pcl - lidar_center[:, None]).copy()
    pcl_copy = cam_rot.T @ pcl_copy
    zangles = (np.pi/2 - np.arccos(pcl_copy[2,:]/((pcl_copy ** 2).sum(axis=0) ** 0.5)))
    full_valid = (zangles <= MAX_ZANGLE) & (zangles >= MIN_ZANGLE)
    pcl_copy = pcl_copy[:,full_valid]
    inten_copy = intensity[full_valid].copy()
    wedge_mask = np.empty((pcl_copy[0,:].size,), dtype=np.bool)
    alive_mask = np.ones((pcl_copy[0,:].size,), dtype=np.bool)
    yangles = np.arctan2(pcl_copy[1,:], pcl_copy[0,:]) % (2 * np.pi)
    for i in range(DATA_RAYS):
        wedge_mask[:] = 0
        wedge_mask[alive_mask] = np.abs(yangles[alive_mask] - RAYS_HOR[i]) < Y_WEDGE_ANG
        if wedge_mask.sum() == 0:
            continue
        cur_rays = HOR_ROTMAT[i] @ VERT_ROTMAT @ ray
        cross_res = np.array([cur_rays[:, 1, None] * pcl_copy[None, 2, wedge_mask] - cur_rays[:, 2, None] * pcl_copy[None, 1, wedge_mask],
                             -cur_rays[:, 0, None] * pcl_copy[None, 2, wedge_mask] + cur_rays[:, 2, None] * pcl_copy[None, 0, wedge_mask],
                              cur_rays[:, 0, None] * pcl_copy[None, 1, wedge_mask] - cur_rays[:, 1, None] * pcl_copy[None, 0, wedge_mask]], dtype=DOUBLE)
        sqdists = (cross_res ** 2).sum(axis=0)
        min_ids, valid_lasers = unique_argmin(sqdists)
        ptdists = (pcl_copy[:, wedge_mask][:, min_ids] ** 2).sum(axis=0) ** 0.5
        corr_dist_lasers = (ptdists >= MIN_RAY_FAR) & (ptdists <= MAX_RAY_FAR)
        alive_mask[np.where(wedge_mask)[0][min_ids]] = 0

        result[valid_lasers[corr_dist_lasers], i, 0] = ptdists[corr_dist_lasers]
        result[valid_lasers[corr_dist_lasers], i, 1] = inten_copy[wedge_mask][min_ids[corr_dist_lasers]]
        result[valid_lasers[corr_dist_lasers], i, 2] = 1
    return result


cpdef np.ndarray[DOUBLE_t, ndim=3] get_lidar_data(pool, DOUBLE_t timestamp, np.ndarray[DOUBLE_t, ndim=1] lidar_correction):
    if timestamp < pool.tss[0] or timestamp > pool.tss[-1]:
        return None
    cdef:
        np.ndarray[DOUBLE_t, ndim=2] pcl, cam_rot
        np.ndarray[DOUBLE_t, ndim=1] intensity, lidar_center
        bint gta
    gta = isinstance(pool, dp.GTADataPool)
    if gta:
        pcl, lidar_center, straight = pool.load_full_rot(timestamp, return_straight=True)
        cam_rot = gta_cam_rot(straight, np.array([0, 0, 0], dtype=DOUBLE), rads=True)
        intensity = np.ones((pcl.shape[1],), dtype=DOUBLE)
    else:
        pcl, lidar_center = pool.load_data(timestamp)
        cam_rot = pool.get_rotmat(timestamp).astype(DOUBLE)
        intensity = pcl[3, :].copy()
        pcl = pcl[:3, :]
    if lidar_correction is not None:
        lidar_center = lidar_center + lidar_correction
    return _compute_lidar_data(pcl, intensity, lidar_center, cam_rot)


@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
cpdef np.ndarray[DOUBLE_t, ndim=2] reconstruct_pcl(np.ndarray[DOUBLE_t, ndim=3] lidar_data):
    cdef:
        np.ndarray[INT_t, ndim=1] x, y
        np.ndarray[DOUBLE_t, ndim=1] ray = np.array([1., 0, 0], dtype=DOUBLE)
        np.ndarray[DOUBLE_t, ndim=2] result
    x, y = np.where(lidar_data[:,:,2] >= 0.5)
    result = np.empty((4, x.size), dtype=DOUBLE)
    result[3, :] = lidar_data[x, y, 1]
    result[:3, :] = (lidar_data[x, y, 0, None] * (HOR_ROTMAT[y] @ VERT_ROTMAT[x] @ ray)).T
    return result.astype(DOUBLE)
