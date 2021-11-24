#!/usr/bin/python
import numpy as np;
from perceptron import perceptron;
from linmach import linmach;
from confus import confus

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
M=N-NTr;
test=data[NTr:,:];
print('#    b    E   k  Ete');
print('#------- --- --- ---');
for b in [.1,1,10,100,1000,10000,100000]:
    w,E,k=perceptron(train,b); rl=np.zeros((M,1));
    for n in range(M):
        rl[n]=labs[linmach(w,np.concatenate(([1],test[n,:D])))];
    nerr,m=confus(test[:,L-1].reshape(M,1),rl);
    print('%8.1f %3d %3d %3d' % (b,E,k,nerr));