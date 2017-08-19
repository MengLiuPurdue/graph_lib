from proxl1PRaccel import proxl1PRaccel
import numpy as np
import math

ai = np.array([0,2,5,6,8],dtype=np.int64)
aj = np.array([0,3,0,1,3,3,1,3],dtype=np.int64)
a = np.array([1,1,1,1,1,1,1,1],dtype=np.float64)
rho = 0.5
alpha = 0.7
v = np.array([1,2],dtype=np.int64)
d = np.array([2.3,1.7,1.9,4.2],dtype=np.float64)
epsilon = 0.0001
maxiter = 1000
n = 4
v_num = 2
ds = np.zeros(n,dtype=np.float64)
dsinv = np.zeros(n,dtype=np.float64)
for i in range(n):
    ds[i] = math.sqrt(d[i])
    dsinv[i]  = 1/ds[i]
(not_converged,grad,p) = proxl1PRaccel(n,ai,aj,a,alpha,rho,v,v_num,d,ds,dsinv,epsilon,maxiter)
print grad,p
