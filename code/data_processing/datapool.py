import weakref
import multiprocessing as mp
import zipfile as zf
import json
import io
import numpy as np
import rays

class DataPool:
    def __init__(self, filelist, threads=None):
        filelist = sorted(filelist)
        self.filelist = filelist
        self.size = len(filelist)
        self.data = weakref.WeakValueDictionary()
        self.inverse_tss = {}
        if threads is -1:
            metas = list(map(read_metadata, filelist))
        else:
            with mp.Pool(threads) as pool:
                metas = pool.map(read_metadata, filelist)
        self.metas = dict(zip(self.filelist, metas))
        self.centers = {}
        self.tss = np.array([m['timestamp'] for m in metas])
        self.rots = None

    def _get_ith(self, i):
        name = self.filelist[i]
        d = self.data.get(name, None)
        if d is None:
            d = read_pcl(name)
            self.data[name] = d
        center = self.centers.get(name, None)
        if center is None:
            center = np.array(self.metas[name]['lidar_center'])
            self.centers[name] = center
        return d, center

    def _get_i(self, timestamp, after, check=True):
        i = self.inverse_tss.get((timestamp, after, check), None)
        if i is None:
            i = np.searchsorted(self.tss, timestamp, side=('left' if after else 'right'))
            if not after:
                i -= 1
            if check and i < 0 or i >= self.size:
                self.inverse_tss[(timestamp, after, check)] = False
                i = False
            else:
                self.inverse_tss[(timestamp, after, check)] = i
        return i

    def load_data(self, timestamp, after=True):
        i = self._get_i(timestamp, after)
        if i is False:
            return None
        return self._get_ith(i)

    def get_rotmat(self, timestamp, after=True):
        if self.rots is None:
            self.rots = np.array([m['matrix'] for _, m in sorted(self.metas.items())])[:, :3, :3]
        i = self._get_i(timestamp, after)
        if i is False:
            return None
        return self.rots[i]


class GTADataPool(DataPool):
    def __init__(self, filelist, threads=None):
        super().__init__(filelist, threads)
        self.full_rot_pcls = weakref.WeakValueDictionary()
        self.rots = np.array([rays.gta_cam_rot(np.array(m['orig_json']['camera_rot']),
                                               np.array(m['orig_json']['camera_relative_rotation']))
                              for _, m in sorted(self.metas.items())])

    def _get_full_rot(self, se):
        d = self.full_rot_pcls.get(se, None)
        if d is None:
            d = np.concatenate([self._get_ith(i)[0] for i in range(*se)], axis=1)
            self.full_rot_pcls[se] = d
        return d

    def load_full_rot(self, timestamp, after=False, this_pos=0, return_straight=False):
        index = self._get_i(timestamp, after, False)
        start = np.maximum(index - this_pos, 0)
        end = np.minimum(index + (4 - this_pos), self.size)
        lidar_positions = np.array([self.metas[name]['lidar_center']
                                    for name in self.filelist[start:end]]).T
        lidar_center = np.array([np.interp(timestamp, self.tss[start:end], pos)
                                 for pos in lidar_positions])
        to_return = [self._get_full_rot((start, end)), lidar_center]

        if return_straight:
            worldrots = np.array([self.metas[name]['orig_json']['camera_rot']
                                  for name in self.filelist[start:end]])
            relrots = np.array([self.metas[name]['orig_json']['camera_relative_rotation']
                                for name in self.filelist[start:end]])
            relangles = np.radians(worldrots - relrots).T
            straight = np.array([np.interp(timestamp, self.tss[start:end], ang)
                                 for ang in relangles])
            to_return.append(straight)

        return tuple(to_return)


def read_both(filename):
    with zf.ZipFile(filename, 'r') as cf:
        npdata = io.BytesIO(cf.read('pcl.npy'))
        meta = json.loads(cf.read('metadata.json').decode('utf-8'))
    pcl = np.load(npdata)
    return pcl, meta


def read_pcl(filename):
    with zf.ZipFile(filename, 'r') as cf:
        npdata = io.BytesIO(cf.read('pcl.npy'))
    pcl = np.load(npdata)
    return pcl


def read_metadata(filename):
    with zf.ZipFile(filename, 'r') as cf:
        meta = json.loads(cf.read('metadata.json').decode('utf-8'))
    return meta
