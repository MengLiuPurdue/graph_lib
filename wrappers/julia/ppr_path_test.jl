include("ppr_path.jl")
include("readSMAT.jl")
A = readSMAT("../../graph/usps_3nn_list.smat")
alpha = 0.99
eps = 0.0001
rho = 0.0
seedids = [7576]
nseedids = 1
xlength=A.n
(actual_length,xids)=ppr_path(A,alpha,eps,rho,seedids,nseedids,xlength)
@show actual_length
@show xids
