#!/usr/bin/python
from __future__ import print_function
import numpy as np

data=np.loadtxt('OCR_14x14');
w=np.loadtxt('percep_w');
N,L=data.shape;
D=L-1;
labs=np.unique(data[:,L-1]);
C=labs.size;
for n in range(N):
    for c in range(C):
        xn=np.concatenate(([1],data[n,:D]));
        print('g_%d(x_%d)=%.0f ' % (c,n,np.dot(w[:,c],xn)),end='');
    print('');