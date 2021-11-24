#!/usr/bin/python
import numpy as np
from perceptron import perceptron

data=np.loadtxt('OCR_14x14');
N,L=data.shape;
D=L-1;
labs=np.unique(data[:,L-1]);
C=labs.size;
np.random.seed(23);
perm=np.random.permutation(N);
data=data[perm];
NTr=int(round(.7*N));
train=data[:NTr,:];
w,E,k=perceptron(train);
np.savetxt('percep_w',w,fmt='%.2f');
print(w);