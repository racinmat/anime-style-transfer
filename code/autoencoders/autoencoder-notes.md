#### runs
- 2019-03-02--15-09 baseline, home checkerboard artifacts
- 2019-03-02--15-50 10^-8 latent space regularization, nothing changed 
- 2019-03-02--17-45 encoder - maxpooling, decoder - overlapping transposed convolution
- 2019-03-02--18-32 encoder - convolution with overlap instead of maxpooling, decoder same
- 2019-03-03--11-05 z size = 30
- 2019-03-03--12-08 bigger networks capacity

### notes

init learning rate should be max. 1e-4, larger are more problematic and won't converge even with decay
relu and elu does not make difference
kernel size 3 converges better than 9
scaling kernel size 5 converges better than 7
does not make sense to freeze weights, they may be initialized, but not frozen for training
initializing weights by shallower autoencoder gets you a bit better and faster convergence

anime-2019-03-13--18-50 and anime-2019-03-13--20-10 are same networks with same parameters, 
but anime-2019-03-13--20-10 has shallower layer weights loaded from logs/anime-2019-03-13--02-06/model.h5