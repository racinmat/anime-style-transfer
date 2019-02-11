## todo:
zkusit vyhodit weight los, nechat self-regularizaci
obohatit ade20k animefest videi? 
zkusit pouze animefest videa?
zkusit pouze cosplay videa?

zkusit na jednotlivá anime autoencoder s embeddingem, PCA a pak "šoupáky" jako v https://www.youtube.com/watch?v=wXWKWyALxYM?t=286 ?

projít si training tips: https://github.com/junyanz/pytorch-CycleGAN-and-pix2pix/blob/master/docs/tips.md
vyzoušet cycada https://arxiv.org/pdf/1711.03213.pdf (alternativa k cycleganu)
a taky https://arxiv.org/abs/1812.04948
a i ten původní neural style paper

až do konce ledna 2019 jsou všechny běhy se špatným global stepem, a tedy bez learning rate decay 

zrefactorovat history pooling z https://github.com/tensorflow/tensorflow/blob/c985bd0dce0f8a7ccf334c9782d051c81ad00f1d/tensorflow/contrib/gan/python/features/python/random_tensor_pool_impl.py#L50
a benchmarknout

historical averaging, ale nikde to není implementované - musel bych si pamatovat hodnoty vah pro X modelů - k ničemu

vyzkoušet lepší cycle loss a vůbec věci v tomhle paperu https://ssnl.github.io/better_cycles/report.pdf

vyzkoušet architekturu z https://colab.research.google.com/drive/1Enc-pKlP4Q3cimEBfcQv0B_6hUvjVL3o?sandboxMode=true#scrollTo=5oONZ70XXq7D


možná zkusit discogan
omrknout https://www.cs.ru.nl/bachelors-theses/2018/Robin_Elbers___4225678___On_the_replication_of_CycleGAN.pdf

možná zkusit historical averaging
