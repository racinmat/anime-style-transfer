from __future__ import division
import numpy as np
import datapool as dp
cimport numpy as np
cimport cython

DOUBLE = '<f8'
INT = '<i8'
FLOAT = '<f4'
ctypedef np.float32_t FLOAT_t
ctypedef np.float64_t DOUBLE_t
ctypedef np.uint8_t BOOL_t
ctypedef np.int_t INT_t

cdef:
    int DATA_RAYS = 2084
    int NUM_LASERS = 64
    DOUBLE_t HORIZONTAL_SHIFT = np.radians(0.1728)
    DOUBLE_t Y_WEDGE_ANG = 2 * HORIZONTAL_SHIFT/3

    DOUBLE_t MAX_ZANGLE = np.radians(3)
    DOUBLE_t MIN_ZANGLE = np.radians(-25.5)

    DOUBLE_t MIN_RAY_FAR = 0.9
    DOUBLE_t MAX_RAY_FAR = 131.0
    DOUBLE_t VALID_PT = 0.5

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


cdef tuple allowed_argmin(np.ndarray[DOUBLE_t, ndim=2] dists, np.ndarray[DOUBLE_t, ndim=1] allowance):
    cdef:
        np.ndarray[BOOL_t, ndim=1, cast=True] valid
        np.ndarray[INT_t, ndim=2] sids = np.argsort(dists, axis=1)
        np.ndarray[INT_t, ndim=1] recter = np.zeros((NUM_LASERS,), dtype=INT), dist_ids = np.arange(NUM_LASERS, dtype=INT)

    valid = (dists[dist_ids, sids[dist_ids, recter]] <= allowance[sids[dist_ids, recter]]).reshape((-1))
    dist_ids = dist_ids[valid]
    recter = recter[valid]

    return sids[dist_ids, recter], dist_ids


@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
cdef np.ndarray[FLOAT_t, ndim=3] _compute_lidar_data(np.ndarray[DOUBLE_t, ndim=2] pcl,
                                                      np.ndarray[DOUBLE_t, ndim=1] intensity,
                                                      np.ndarray[DOUBLE_t, ndim=1] lidar_center,
                                                      np.ndarray[DOUBLE_t, ndim=2] cam_rot,
                                                      DOUBLE_t allow):
    cdef:
        np.ndarray[BOOL_t, ndim=1, cast=True] wedge_mask, alive_mask, corr_dist_lasers, full_valid
        np.ndarray[DOUBLE_t, ndim=3] result = np.zeros(DATA_SHAPE), cross_res
        np.ndarray[DOUBLE_t, ndim=1] ray = np.array([1.0, 0, 0]), yangles, ptdists, zangles, inten_copy, precomp_dists, allowance
        np.ndarray[DOUBLE_t, ndim=2] pcl_copy, cur_rays, sqdists
        np.ndarray[INT_t, ndim=1] min_ids, valid_lasers
        int i
    pcl_copy = (pcl - lidar_center[:, None]).copy()
    pcl_copy = cam_rot.T @ pcl_copy
    zangles = (np.pi/2 - np.arccos(pcl_copy[2,:]/((pcl_copy ** 2).sum(axis=0) ** 0.5)))
    full_valid = (zangles <= MAX_ZANGLE) & (zangles >= MIN_ZANGLE)
    pcl_copy = pcl_copy[:,full_valid]
    precomp_dists = (pcl_copy ** 2).sum(axis=0) ** 0.5
    allowance = (precomp_dists * allow) ** 2
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
        min_ids, valid_lasers = allowed_argmin(sqdists, allowance[wedge_mask])
        ptdists = precomp_dists[wedge_mask][min_ids]
        corr_dist_lasers = (ptdists >= MIN_RAY_FAR) & (ptdists <= MAX_RAY_FAR)

        result[valid_lasers[corr_dist_lasers], i, 0] = ptdists[corr_dist_lasers]
        result[valid_lasers[corr_dist_lasers], i, 1] = inten_copy[wedge_mask][min_ids[corr_dist_lasers]]
        result[valid_lasers[corr_dist_lasers], i, 2] = 1
    return result.astype(FLOAT)


cpdef np.ndarray[FLOAT_t, ndim=3] interpolate_lidar(np.ndarray[FLOAT_t, ndim=3] lidar_data, DOUBLE_t to_fill, INT_t mask_size=1, INT_t iters=1):
    cdef:
        np.ndarray[BOOL_t, ndim=1, cast=True] valid
        np.ndarray[BOOL_t, ndim=3, cast=True] all_valid
        np.ndarray[INT_t, ndim=1] x, y
        np.ndarray[INT_t, ndim=2] x_shift, y_shift
        np.ndarray[INT_t, ndim=3] xx, yy
        np.ndarray[FLOAT_t, ndim=3] lidar_copy = np.pad(np.copy(lidar_data), ((mask_size, mask_size), (mask_size, mask_size), (0, 0)), 'constant'), valid_weights
        INT_t i

    x_shift, y_shift = np.meshgrid(np.arange(mask_size * 2 + 1) - mask_size, np.arange(mask_size * 2 + 1) - mask_size)

    for i in range(iters):
        x, y = np.where(lidar_copy[:,:,2] < VALID_PT)
        valid = (x >= mask_size) & (x < NUM_LASERS + mask_size) & (y >= mask_size) & (y < DATA_RAYS + mask_size)
        if valid.sum() == 0:
            break
        x = x[valid]
        y = y[valid]
        xx = x[:, np.newaxis, np.newaxis] + x_shift[np.newaxis, :]
        yy = y[:, np.newaxis, np.newaxis] + y_shift[np.newaxis, :]

        all_valid = (xx >= mask_size) & (xx < NUM_LASERS + mask_size) & (yy >= mask_size) & (yy < DATA_RAYS + mask_size)
        valid_weights = np.multiply(lidar_copy[xx, yy, 2], lidar_copy[xx, yy, 2] >= VALID_PT)
        valid = valid_weights.sum(axis=(1,2)) >= (all_valid.sum(axis=(1,2)) * to_fill)
        if valid.sum() == 0:
            break
        x = x[valid]
        y = y[valid]
        xx = xx[valid]
        yy = yy[valid]
        valid_weights = valid_weights[valid]

        lidar_copy[x, y, 0] = np.average(lidar_copy[xx, yy, 0], weights=valid_weights, axis=(1,2))
        lidar_copy[x, y, 1] = np.average(lidar_copy[xx, yy, 1], weights=valid_weights, axis=(1,2))
        lidar_copy[x, y, 2] = 1
        break

    return lidar_copy[mask_size:(NUM_LASERS + mask_size), mask_size:(DATA_RAYS + mask_size), :]


cpdef np.ndarray[FLOAT_t, ndim=3] get_lidar_data(pool, DOUBLE_t timestamp, np.ndarray[DOUBLE_t, ndim=1] lidar_correction, DOUBLE_t allowance):
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
    else:
        pcl, lidar_center = pool.load_data(timestamp)
        cam_rot = pool.get_rotmat(timestamp).astype(DOUBLE)
    intensity = pcl[3, :].copy()
    pcl = pcl[:3, :]
    if lidar_correction is not None:
        lidar_center = lidar_center + lidar_correction
    return _compute_lidar_data(pcl, intensity, lidar_center, cam_rot, allowance)


@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
cpdef np.ndarray[FLOAT_t, ndim=2] reconstruct_pcl(np.ndarray[FLOAT_t, ndim=3] lidar_data):
    cdef:
        np.ndarray[INT_t, ndim=1] x, y
        np.ndarray[DOUBLE_t, ndim=1] ray = np.array([1., 0, 0], dtype=DOUBLE)
        np.ndarray[FLOAT_t, ndim=2] result
    x, y = np.where((lidar_data[:,:,2] >= VALID_PT) & (lidar_data[:,:,0] >= MIN_RAY_FAR) & (lidar_data[:,:,0] <= MAX_RAY_FAR))
    result = np.empty((4, x.size), dtype=FLOAT)
    result[3, :] = lidar_data[x, y, 1].astype(FLOAT)
    result[:3, :] = ((lidar_data[x, y, 0, None] * (HOR_ROTMAT[y] @ VERT_ROTMAT[x] @ ray)).T).astype(FLOAT)
    return result
