## todo:
kusit vyhodit weight los, nechat self-regularizaci
obohatit ade20k animefest videi? 
zkusit pouze animefest videa?
zkusit pouze cosplay videa?

zkusit na jednotlivá anime autoencoder s embeddingem, PCA a pak "šoupáky" jako v https://www.youtube.com/watch?v=wXWKWyALxYM?t=286 ?

projít si training tips: https://github.com/junyanz/pytorch-CycleGAN-and-pix2pix/blob/master/docs/tips.md
vyzoušet cycada https://arxiv.org/pdf/1711.03213.pdf (alternativa k cycleganu)
a taky https://arxiv.org/abs/1812.04948
a i ten původní neural style paper

až do konce ledna 2019 jsou všechny běhy se špatným global stepem, a tedy bez learning rate decay 

todo: udělat benchmarky:
původní graf vs. input cache v BaseNetworku
formát dat NHWC vs. NCHW
současné načítání dat vs. fronty