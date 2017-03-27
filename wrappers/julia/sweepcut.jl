# A julia wrapper for sweep cut procedure
# A - the sparse matrix representing the symmetric graph
# ids - the order of vertices given
# results - the best set with the smallest conductance
# actual_length - the number of vertices in the best set
# num - the number of vertices given
# values - A vector scoring each vertex (e.g. pagerank value). 
#          This will be sorted and turned into one of the other inputs.
# fun_id - 0 for sweepcut_with_sorting and 1 for sweepcut_without_sorting
function sweep_cut{T}(A::SparseMatrixCSC{T,Int64},ids,num,n,values,fun_id)
    offset=1;
    results=zeros(Int64,num);
    if fun_id == 1
        actual_length=ccall((:sweepcut_without_sorting64,"libgraph"),Int64,(Ptr{Int64},
                Ptr{Int64},Int64,Int64,Ptr{Int64},Ptr{Int64},Int64),ids,results,num,n,A.colptr,
                A.rowval,offset);
    elseif fun_id == 0
        actual_length=ccall((:sweepcut_with_sorting64,"libgraph"),Int64,(Ptr{Cdouble},Ptr{Int64},
                Ptr{Int64},Int64,Int64,Ptr{Int64},Ptr{Int64},Int64),values,ids,results,num,n,A.colptr,
                A.rowval,offset);
    else
        error("Please specify your function (0 for sweepcut_with_sorting and 1 for sweepcut_without_sorting)");
    end
    results=results[1:actual_length]
    return actual_length, results
end
