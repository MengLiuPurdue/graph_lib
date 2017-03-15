using MatrixNetworks
include("spectral.jl")
A = load_matrix_network("minnesota");
v = fiedler_vector(A)[1];
p = sortperm(v,rev=true);
n=2642;
num=2642;
results=zeros(Int64,num);
actual_length=julia_wrapper(A,p,num,n,~,1);
@show actual_length
