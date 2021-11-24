#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt

w=np.loadtxt('percep_w');
D,C=w.shape;
mw=np.mean(w[1:],axis=1);
for c in range(C):
    wc=w[1:,c];
    pw=np.maximum(0,wc-mw);
    I=np.reshape(pw,(14,14));
    plt.imshow(I, cmap='gray');
    plt.axis('off');
    plt.show();
    nw=np.minimum(0,wc-mw);
    I=np.reshape(nw,(14,14));
    plt.imshow(I, cmap='gray_r');
    plt.axis('off');
    plt.show();