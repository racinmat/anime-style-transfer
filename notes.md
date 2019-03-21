### long-term ideas:

try to use autoencoder with embedding on individual anime, then PCA and the widget like in https://www.youtube.com/watch?v=wXWKWyALxYM?t=286

try to go through tips in: https://github.com/junyanz/pytorch-CycleGAN-and-pix2pix/blob/master/docs/tips.md
try the StyleGAN paper https://arxiv.org/abs/1812.04948
and also try the original neural style paper
take a look at DiscoGAN
take a look at https://www.cs.ru.nl/bachelors-theses/2018/Robin_Elbers___4225678___On_the_replication_of_CycleGAN.pdf
take a look at CartoonGAN
take a look at Deep Analogy (Visual Attribute Transfer through Deep Image Analogy)
take a look at CNNMRF https://arxiv.org/pdf/1601.04589.pdf


### ideas:
- historical averaging 
    - but nowhere implemented. I'd have to keep X versions of all weights - too much memory consuming - not good idea
- try things from https://ssnl.github.io/better_cycles/report.pdf
- try architecture from https://colab.research.google.com/drive/1Enc-pKlP4Q3cimEBfcQv0B_6hUvjVL3o?sandboxMode=true#scrollTo=5oONZ70XXq7D
- generally, try architectures from different papers and implementations

### todo:
- add the hyper-parameter tuning and orchestration
- use https://github.com/automl/ConfigSpace and SMAC
- integrate TFGAN's inception score and FID
- create original neural style transfer - should work and learn more quickly
- compare PCA, autoencoder and variational autoencoder for individual animes
- compare random sampling style image from anime with selecting components from dimension reduction as style image

### lecture notes
video for CartoonGAN https://www.youtube.com/watch?v=r4zzhN8Aibw&ab_channel=ZivZone to lecture - faces from it
mention in lecture https://github.com/luanfujun/deep-painterly-harmonization (the most funny of them)

for installing lua:
install lua from https://github.com/rjpcomputing/luaforwindows/releases
for installing torch:
install cmake manually by `conda install -c anaconda cmake`
clone https://github.com/BTNC/distro-win.git
then follow win instructions, but for beware problems with outputting things to files
download zips manually, there is problem with them