from ppr_path import ppr_path
f = open("../../graph/usps_3nn_list.smat")
first_line = f.readline().strip()
import re
first_line = re.split(" ",first_line)
n = long(first_line[0])
m = long(first_line[2])
ei = []
ej = []
w = []

data = f.read()
data = data.split()
for i in range(m):
    ei += [int(data[3 * i])]
    ej += [int(data[3 * i + 1])]
    w += [data[3 * i + 2]]

f.close()
print("read data file, done!")
alpha=0.99
eps=10**(-4)
rho=0.0
xlength=n
seedids=[7575]
nseedids=1
(actual_length,xids)=ppr_path(n,ei,ej,alpha,eps,rho,seedids,nseedids,xlength,0)
print actual_length,xids
