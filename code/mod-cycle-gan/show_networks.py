import os
import os.path as osp
import re
from datetime import datetime, date

from prettytable import PrettyTable


def load_runs():
    checkpoints_dir = '../../checkpoint'
    run_dirs = os.listdir(checkpoints_dir)

    run_from_time = datetime(2019, 1, 1)
    runs = {}
    for run_dir in run_dirs:
        run_time = datetime.strptime(run_dir[:-2], '%Y-%m-%d--%H-%M')
        if run_time < run_from_time:
            continue
        flagfile = osp.join(checkpoints_dir, run_dir, 'params.flagfile')
        params = {}
        with open(flagfile, mode='r', encoding='utf-8') as fp:
            for line in fp.readlines():
                line = line.replace('\n', '')
                if '=' in line:
                    name, value = re.match(r'--(.*)=(.*)', line).groups()
                else:
                    name = re.match(r'--(.*)', line).groups()
                    value = True
                params[name] = value
        runs[run_dir] = params
    return runs


def main():
    runs = load_runs()
    shown_params = ['gantype', 'YXgwl', 'XYgwl', 'Ytfr']

    x = PrettyTable(['name'] + shown_params)
    for run_name, run_params in runs.items():
        x.add_row([run_name] + [i for k, i in run_params.items() if k in shown_params])
    print(x)


if __name__ == '__main__':
    main()
