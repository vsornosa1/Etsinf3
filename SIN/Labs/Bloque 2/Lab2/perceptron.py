import numpy as np

def perceptron(data,b=0.1,a=1.0,K=200):
    (N,L)=data.shape;
    D=L-1; 
    labs=np.unique(data[:,L-1]);
    C=labs.size;
    w = np.zeros((L,C));
    for k in range(1,K+1):
        E=0;
        for n in range(N):
             xn=np.concatenate(([1],data[n,:D]));
             cn=np.where(labs==data[n,L-1])[0][0];
             er=0;
             g=np.dot(w[:,cn],xn);
             for c in range(C):
                 if c != cn and np.dot(w[:,c],xn) + b > g:
                     w[:,c] = w[:,c] - a*xn;
                     er=1;
             if er==1:
                w[:,cn] = w[:,cn] + a*xn;
                E=E+1;
        if E==0: 
            break;
    return w,E,k;
