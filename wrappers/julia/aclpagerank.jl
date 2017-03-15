# A julia wrapper for aclpagerank
# A - sparse matrix representation of graph
# alpha - value of alpha
# eps - value of epsilon
# seedids,nseedids - the set of indices for seeds
# maxsteps - the max number of steps
# xlength - the max number of ids in the solution vector
# xids, actual_length - the solution vector
# values - the pagerank value vector for xids (already sorted in decreasing order)
# dataType - the type of data, 0 for 32bit and 1 for 64bit
function julia_wrapper{T}(A::SparseMatrixCSC{T,Int64},alpha::Float64,
                          eps::Float64,seedids,nseedids,maxsteps,xlength,dataType)
    n=A.n;
    offset=1;
    if dataType == 0
        xids=zeros(Cuint,xlength);
        values=zeros(Cdouble,xlength);
        actual_length=ccall((:aclpagerank32,"libgraph"),Cuint,
                            (Cuint,Ptr{Cuint},Ptr{Cuint},Cuint,Cdouble,Cdouble,
                            Ptr{Cuint},Cuint,Cuint,Ptr{Cuint},Cuint,Ptr{Cdouble}),n,A.colptr,
                            A.rowval,offset,alpha,eps,seedids,nseedids,maxsteps,xids,xlength,values);
    elseif dataType == 1
        xids=zeros(Int64,xlength);
        values=zeros(Cdouble,xlength);
        actual_length=ccall((:aclpagerank64,"libgraph"),Int64,
                            (Int64,Ptr{Int64},Ptr{Int64},Int64,Cdouble,Cdouble,
                            Ptr{Int64},Int64,Int64,Ptr{Int64},Int64,Ptr{Cdouble}),n,A.colptr,
                            A.rowval,offset,alpha,eps,seedids,nseedids,maxsteps,xids,xlength,values);   
    else
        error("Please specify your data type (0 for 32bit and 1 for 64bit)");
    end
    values=values[1:actual_length];
    xids=xids[1:actual_length];
    return actual_length,xids,values
end
