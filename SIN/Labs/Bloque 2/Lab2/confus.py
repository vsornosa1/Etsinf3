import numpy as np

def confus(truelabs,recolabs):
    N=truelabs.size; labs=np.unique(np.concatenate([truelabs,recolabs]));
    C=labs.size; 
    m=np.zeros((C,C));
    for n in range(N):
        tc=np.where(labs==truelabs[n])[0][0];
        rl=np.where(labs==recolabs[n])[0][0];
        m[tc,rl]+=1;
    a=0;
    for c in range(C):
        a+=m[c,c];
    nerr=N-a;
    return nerr,m; 
