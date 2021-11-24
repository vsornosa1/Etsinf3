import math
import numpy as np

def linmach(w,x):
    C = w.shape[1];
    cstar=1;
    maximo=float('-inf');
    for c in range(C):
        g=np.dot(w[:,c],x);
        if g>maximo:
            maximo=g;
            cstar=c;
    return cstar;
