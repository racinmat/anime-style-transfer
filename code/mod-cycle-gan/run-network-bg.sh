#!/bin/bash

#screen -dmS python -L screenlog.0 ./run-network.sh
# now using nohup
#ampersand na konci je na puštění ve forknutým procesu, &> je všechen output (stdout + stderr) přesměrovaný do souboru log.log a nohup říká, ať proces neumře, pokud skončí session, ve které to bylo spuštěný
nohup python3 train.py "$@" &> log.log &
