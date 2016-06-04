#!/bin/bash

OUTPUT_DIR="." # "build/Debug/"

# http://archive.ics.uci.edu/ml/datasets/Teaching+Assistant+Evaluation
wget -P "$OUTPUT_DIR" http://archive.ics.uci.edu/ml/machine-learning-databases/tae/tae.data

# http://archive.ics.uci.edu/ml/datasets/Iris
wget -P "$OUTPUT_DIR" http://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data

# http://archive.ics.uci.edu/ml/datasets/Wine
wget -P "$OUTPUT_DIR" http://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data

# http://archive.ics.uci.edu/ml/datasets/Statlog+%28Heart%29
wget -P "$OUTPUT_DIR" http://archive.ics.uci.edu/ml/machine-learning-databases/statlog/heart/heart.dat