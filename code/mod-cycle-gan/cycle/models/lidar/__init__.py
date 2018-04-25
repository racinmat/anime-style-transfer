__all__ = ['X_normer', 'X_denormer', 'Y_normer', 'Y_denormer', 'DATA_SHAPE', 'BATCH_SIZE', 'Generator', 'Discriminator']

from . import _utils as utils
from ._nets import Generator, Discriminator

X_normer = utils.normer
Y_normer = utils.normer
X_denormer = utils.denormer
Y_denormer = utils.denormer
visualise = utils.visualise

DATA_SHAPE = (64, 2084, 3)
BATCH_SIZE = 1
