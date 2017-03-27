using MatrixNetworks
include("spectral.jl")
include("sweepcut.jl")
A = load_matrix_network("minnesota");
v = fiedler_vector(A)[1];
p = sortperm(v,rev=true);
n=2642;
num=2642;
results=zeros(Int64,num);
actual_length=sweep_cut(A,p,num,n,~,1);
@show actual_length
