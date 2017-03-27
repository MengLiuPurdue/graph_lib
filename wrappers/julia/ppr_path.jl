# A julia wrapper for ppr_path
# A - sparse matrix representation of graph
# alpha - value of alpha
# eps - value of epsilon
# rho - value of rho
# seedids,nseedids - the set of indices for seeds
# xlength - the max number of ids in the solution vector
# xids, actual_length - the solution vector
# values - the pagerank value vector for xids (already sorted in decreasing order)
function ppr_path{T}(A::SparseMatrixCSC{T,Int64},alpha::Float64,
                          eps::Float64,rho::Float64,seedids,nseedids,xlength)
    offset = 1;
    n=A.n
    xids=zeros(Int64,xlength);
    actual_length=ccall((:ppr_path64,"../../lib/graph_lib_test/libgraph"),Int64,
                        (Int64,Ptr{Int64},Ptr{Int64},Int64,Cdouble,Cdouble,Cdouble,
                        Ptr{Int64},Int64,Ptr{Int64},Int64),n,A.colptr,A.rowval,
                        offset,alpha,eps,rho,seedids,nseedids,xids,xlength); 
    xids=xids[1:actual_length];
    return actual_length,xids
end
