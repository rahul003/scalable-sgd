#!/bin/bash  
rsync -r -t -v ../scalable-sgd/ h_rahul@stampede.tacc.utexas.edu:scalable-sgd/ --exclude=data --exclude=.git --exclude=libs --exclude=bin --exclude=build