from ppr_path import ppr_path
from list_to_CSR import list_to_CSR
import numpy as np

(m,n,ai,aj) = list_to_CSR("../../graph/usps_3nn.smat")
alpha=0.99
eps=10**(-4)
rho=0.0
xlength=n
f = open("../../graph/usps_3nn_seed.smat")
data = f.read()
data = data.split()
nseedids = int(data[0])
seedids = []
for i in range(nseedids):
    seedids += [data[i + 1]]
f.close()
(actual_length,xids) = ppr_path(n,ai,aj,alpha,eps,rho,seedids,nseedids,xlength)
print actual_length,xids
