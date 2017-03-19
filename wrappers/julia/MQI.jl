# A julia wrapper for MQI
# A - sparse matrix representation of graph
# n - number of nodes in the graph
# R - seed set
# nR - number of nodes in seed set
# actual_length - number of nodes in the optimal subset
# ret_set - optimal subset with the smallest conductance
function julia_wrapper{T}(A::SparseMatrixCSC{T,Int64},n,nR,R)
    ret_set=zeros(Int64,nR);
    offset = 1;
    actual_length=ccall((:MQI64,"../../lib/graph_lib_test/libgraph"),Int64,
                        (Int64,Int64,Ptr{Int64},Ptr{Int64},Int64,Ptr{Int64},Ptr{Int64}),n,nR,A.colptr,
                        A.rowval,offset,R,ret_set); 
    ret_set = ret_set[1:actual_length]
    return actual_length,ret_set
end
