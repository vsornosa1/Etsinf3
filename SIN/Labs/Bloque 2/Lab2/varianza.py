#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt

w=np.loadtxt('percep_w');
sw=np.std(w[1:],axis=1);
I=np.reshape(sw,(14,14));
plt.imshow(I, cmap='gray');
plt.axis('off');
plt.show();