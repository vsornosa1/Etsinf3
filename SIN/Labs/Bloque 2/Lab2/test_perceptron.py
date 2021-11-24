#!/usr/bin/python
import numpy as np
from perceptron import perceptron

data=np.array([[0, 0, 0], [1, 1, 1]]);
w,E,k=perceptron(data);
print(w);
print('E=%d k=%d' % (E,k));