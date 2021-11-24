#!/usr/bin/python
import math;
import numpy as np
from linmach import linmach
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
M=N-NTr;
test=data[NTr:,:];
w=np.loadtxt('percep_w');
rl=np.zeros((M,1));

for m in range(M):
    tem=np.concatenate(([1],test[m,:D]));
    rl[m]=labs[linmach(w,tem)];
    
ner,m=confus(test[:,L-1].reshape(M,1),rl);
print('ner=%d'%ner);
print(m);
per=ner/M;
print('per=%.3f'%per);
r=1.96*math.sqrt(per*(1-per)/M);
print('r = %.3f' % r);
print('I=[%.3f, %.3f]'%(per-r,per+r));