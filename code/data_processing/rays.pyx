from __future__ import division
import numpy as np
import datapool as dp
cimport numpy as np
cimport cython

DOUBLE = '<f8'
ctypedef np.float64_t DOUBLE_t
ctypedef np.uint8_t BOOL_t
ctypedef np.int_t INT_t

cdef:
    int DATA_RAYS = 2084
    int NUM_LASERS = 64
    DOUBLE_t HORIZONTAL_SHIFT = 0.1728
    DOUBLE_t ANGLE_WEDGE = np.radians(HORIZONTAL_SHIFT * 0.75)
    DOUBLE_t MAX_SQ_LASERDIST = 0.01

    DOUBLE_t MIN_RAY_FAR = 0.9
    DOUBLE_t MAX_RAY_FAR = 131.0

    tuple DATA_SHAPE = (NUM_LASERS, DATA_RAYS, 3)

    np.ndarray RAYS_VERT = np.radians(np.concatenate((np.linspace(2, (-8 - 1./3), 32, dtype=DOUBLE), np.linspace((-8 - 1./3 - 1./2), (-24 - 1./3), 32, dtype=DOUBLE))))
    np.ndarray VERT_ROTMAT = np.array([[[np.cos(angle), 0, -np.sin(angle)], [0, 1, 0], [np.sin(angle), 0, np.cos(angle)]] for angle in RAYS_VERT], dtype=DOUBLE)
    np.ndarray RAYS_HOR = np.radians(np.arange(0, 360, HORIZONTAL_SHIFT))
    np.ndarray HOR_ROTMAT = np.array([[[np.cos(angle), -np.sin(angle), 0], [np.sin(angle), np.cos(angle), 0], [0, 0, 1]] for angle in RAYS_HOR], dtype=DOUBLE)


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


@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
cdef np.ndarray[DOUBLE_t, ndim=3] _compute_lidar_data(np.ndarray[DOUBLE_t, ndim=2] pcl,
                                                      np.ndarray[DOUBLE_t, ndim=1] intensity,
                                                      np.ndarray[DOUBLE_t, ndim=1] lidar_center,
                                                      np.ndarray[DOUBLE_t, ndim=2] cam_rot,
                                                      DOUBLE_t timestamp):
    cdef:
        np.ndarray[BOOL_t, ndim=1, cast=True] wedge_mask, copy_mask, dist_mask
        np.ndarray[INT_t, ndim=1] min_inds, laser_ids = np.arange(NUM_LASERS)
        np.ndarray[DOUBLE_t, ndim=1] ray, angles, angle_diff, ptdists
        np.ndarray[DOUBLE_t, ndim=2] rays, pts, dists
        np.ndarray[DOUBLE_t, ndim=3] return_data, vert_mats = VERT_ROTMAT, hor_mats = HOR_ROTMAT, cross_res
        int i, j
        DOUBLE_t wedge_angle = ANGLE_WEDGE, cur_rot = 0.0, hor_shift = np.radians(HORIZONTAL_SHIFT), max_sq_laser = MAX_SQ_LASERDIST, min_ray_far = MIN_RAY_FAR, max_ray_far = MAX_RAY_FAR
    ray = np.array([1, 0, 0], dtype=DOUBLE)
    pcl = pcl - lidar_center[:, None]
    pcl = cam_rot.T @ pcl
    angles = np.arctan2(pcl[1, :], pcl[0, :])
    return_data = np.zeros(DATA_SHAPE, dtype=DOUBLE)
    for i in range(DATA_RAYS):
        angle_diff = np.abs((angles - cur_rot) % (2 * np.pi))
        wedge_mask = angle_diff < wedge_angle
        if wedge_mask.sum() != 0:
            rays = hor_mats[i] @ vert_mats @ ray
            cross_res = np.array([rays[:, 1, None] * pcl[None, 2, wedge_mask] - rays[:, 2, None] * pcl[None, 1, wedge_mask],
                                  -rays[:, 0, None] * pcl[None, 2, wedge_mask] + rays[:, 2, None] * pcl[None, 0, wedge_mask],
                                  rays[:, 0, None] * pcl[None, 1, wedge_mask] - rays[:, 1, None] * pcl[None, 0, wedge_mask]], dtype=DOUBLE)
            dists = np.sum((cross_res ** 2), axis=0) ** 0.5
            min_inds = np.argmin(dists, axis=1)
            copy_mask = dists[laser_ids, min_inds] <= max_sq_laser
            pts = pcl[:,wedge_mask][:, min_inds[copy_mask]]
            ptdists = np.sum((pts ** 2), axis=0) ** 0.5
            dist_mask = (ptdists >= min_ray_far) & (ptdists <= max_ray_far)
            return_data[laser_ids[copy_mask][dist_mask], i, 0] = ptdists[dist_mask]
            return_data[laser_ids[copy_mask][dist_mask], i, 1] = intensity[wedge_mask][min_inds[laser_ids[copy_mask][dist_mask]]]
            return_data[laser_ids[copy_mask][dist_mask], i, 2] = 1.0
        cur_rot += hor_shift
    return return_data


cpdef np.ndarray[DOUBLE_t, ndim=3] get_lidar_data(pool, DOUBLE_t timestamp, np.ndarray[DOUBLE_t, ndim=1] lidar_correction):
    if timestamp < pool.tss[0] or timestamp > pool.tss[-1]:
        return None
    cdef:
        np.ndarray[DOUBLE_t, ndim=2] pcl, cam_rot
        np.ndarray[DOUBLE_t, ndim=1] intensity, lidar_center
        np.ndarray[DOUBLE_t, ndim=3] result
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
    return _compute_lidar_data(pcl, intensity, lidar_center, cam_rot, timestamp)


@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
cpdef np.ndarray[DOUBLE_t, ndim=2] reconstruct_pcl(np.ndarray[DOUBLE_t, ndim=3] lidar_data):
    cdef:
        np.ndarray[INT_t, ndim=1] x, y
        np.ndarray[DOUBLE_t, ndim=1] ray = np.array([1., 0, 0], dtype=DOUBLE)
        np.ndarray[DOUBLE_t, ndim=2] result
        np.ndarray[DOUBLE_t, ndim=3] hor_mats = HOR_ROTMAT, vert_mats = VERT_ROTMAT
    x, y = np.where(lidar_data[:,:,2] >= 0.5)
    result = np.empty((4, x.size), dtype=DOUBLE)
    result[3, :] = lidar_data[x, y, 1]
    result[:3, :] = (lidar_data[x, y, 0, None] * (hor_mats[y] @ vert_mats[x] @ ray)).T
    return result.astype(DOUBLE)
