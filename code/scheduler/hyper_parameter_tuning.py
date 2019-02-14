import json
import random

import tensorflow as tf

RUNS_FILE = 'runs.json'


def generate_parameters() -> dict:
    params = {
        'gantype': random.choice(['GAN', 'LSGAN', 'WGAN']),
        'genstr': random.choice(['c-7-1-64-r;c-5-2-128-r;b-3-3-r;r-5-1-64-2-r;c-7-1-3-t;sc']),
        'disstr': random.choice(['c-5-2-64-l;c-5-2-128-l;c-5-2-256-l;c-5-2-1-i;']),
        'gll': 10 ** random.uniform(-2, 2),
        'dll': 10 ** random.uniform(-2, 2),
        'gwl': 10 ** random.uniform(-2, 2),
        'dwl': 10 ** random.uniform(-2, 2),
        'cll': 10 ** random.uniform(-2, 2),
    }
    return params


def generate_parameter_sets(number):
    param_sets = []
    while len(param_sets) < number:
        new_params = generate_parameters()
        if not is_configuration_present(param_sets, new_params):
            param_sets.append(new_params)
    return param_sets


def are_parameter_sets_same(param_set_1: dict, param_set_2: dict, eps=1e-6) -> bool:
    if param_set_1.keys() != param_set_2.keys():
        return False
    for val_1, val_2 in zip(param_set_1.values(), param_set_2.values()):
        if isinstance(val_1, float) and isinstance(val_2, float):
            if abs(val_1 - val_2) > eps:
                return False
        if val_1 != val_2:
            return False
    return True


def is_configuration_present(param_sets, new_params) -> bool:
    for param_set in param_sets:
        if are_parameter_sets_same(param_set, new_params):
            return True
    return False


def persist_param_sets(param_sets):
    with open(RUNS_FILE, encoding='utf-8', mode='w+') as f:
        json.dump(param_sets, f)


def main():
    # todo: vyzkoušet a dodělat
    param_sets = generate_parameter_sets(10)
    persist_param_sets(param_sets)


if __name__ == '__main__':
    tf.app.run()
