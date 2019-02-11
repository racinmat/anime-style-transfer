### long-term ideas:

try to use autoencoder with embedding on individual anime, then PCA and the widget like in https://www.youtube.com/watch?v=wXWKWyALxYM?t=286

try to go through tips in: https://github.com/junyanz/pytorch-CycleGAN-and-pix2pix/blob/master/docs/tips.md
try the StyleGAN paper https://arxiv.org/abs/1812.04948
and also try the original neural style paper
take a look at DiscoGAN
take a look at https://www.cs.ru.nl/bachelors-theses/2018/Robin_Elbers___4225678___On_the_replication_of_CycleGAN.pdf

zrefactorovat history pooling z https://github.com/tensorflow/tensorflow/blob/c985bd0dce0f8a7ccf334c9782d051c81ad00f1d/tensorflow/contrib/gan/python/features/python/random_tensor_pool_impl.py#L50
a benchmarknout

### ideas:
- historical averaging 
    - but nowhere implemented. I'd have to keep X versions of all weights - too much memory consuming - not good idea
- try things from https://ssnl.github.io/better_cycles/report.pdf
- try architecture from https://colab.research.google.com/drive/1Enc-pKlP4Q3cimEBfcQv0B_6hUvjVL3o?sandboxMode=true#scrollTo=5oONZ70XXq7D
- generally, try architectures from different papers and implementations

### todo:
- cycle loss lambda decay
