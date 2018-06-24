#!/bin/bash

nohup tensorboard --logdir=./../../checkpoint &> log.log &
