from sweepcut import sweepcut
from list_sparse import list_sparse
import numpy as np

(m,n,ai,aj) = list_sparse("../../graph/minnesota.smat")

f = open("../../graph/minnesota_ids.smat")
data = f.read()
data = data.split()
nids = int(data[0])
ids = []
for i in range(nids):
    ids += [int(data[i + 1])]
f.close()

values=[]
flag = 1
(actual_length,bestclus)=sweepcut(n,ai,aj,ids,nids,values,flag)
print actual_length,bestclus
