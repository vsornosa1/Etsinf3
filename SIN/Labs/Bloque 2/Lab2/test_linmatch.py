#!/usr/bin/python
import numpy as np
from linmach import linmach

w=np.array([[-4,-36],[4,12]]);
for x in range(1,9):
    y=np.array([1, x]);
    print('c(%d)=%d' % (x,linmach(w,y)))