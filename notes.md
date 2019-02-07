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

todo: fixnout to, že global_step je inkrementuje pro každou train operaci, takže min. 2x rychlejší

todo: udělat benchmarky:
původní graf vs. input cache v BaseNetworku DONE
spojený training
formát dat NHWC vs. NCHW
současné načítání dat vs. fronty
upgrade tensorflow až bude 1.13 stable + XLA kompilace
kouknout na http://on-demand.gputechconf.com/gtc-eu/2017/presentation/23363-chris-fregly-optimizing-profiling-and-deploying-tensorflow-ai-models-in-production-with-gpus.pdf

1.
2019-02-05--00-27-0 - input cache pro BaseNetwork
2019-02-05--23-36-0 - bez input cache pro BaseNetwork
s input cache o 27% rychlejší
2019-02-06--23-40-0 - s optimalizovanou input pipeline, sice feed_dict, ale history pooling v grafu <- 2x méně global_step, protože každá neinkrementuji global_step v train ops 
