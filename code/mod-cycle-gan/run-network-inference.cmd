::python transform.py --extract --inpath=../../dataset-sources/anime/images/hidamari-sketch-sess-1/*.png --outdir=../../data/images/hidamari-sketch-sess-1 --includein=0 --rundir=20180903-0020-0 --Xname=hidamari-sketch --Yname=shinkai --batchsize=16
::above is with extraction
python transform.py --inpath=../../dataset-sources/anime/images/hidamari-sketch-sess-1/*.png --outdir=../../data/images/hidamari-sketch-sess-1_out --includein=0 --rundir=20180903-0020-0 --Xname=hidamari-sketch --Yname=shinkai --batchsize=16 > output.log 2>&1
