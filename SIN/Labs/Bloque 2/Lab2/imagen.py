#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt

data=np.loadtxt('OCR_14x14');
N,L=data.shape;
D=L-1;
I=np.reshape(data[0,:D],(14,14));
plt.imshow(I, cmap='gray_r');
plt.axis('off');
plt.show();

np.random.seed(23);
perm=np.random.permutation(N);
data=data[perm];
for n in range(N):
    I=np.reshape(data[n,:D],(14,14));
    plt.imshow(I, cmap='gray_r');
    
    plt.axis('off');
    plt.show();