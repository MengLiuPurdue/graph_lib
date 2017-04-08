
% A matlab wrapper for aclpagerank
% A - sparse matrix representation of graph
% alpha - value of alpha
% eps - value of epsilon
% seedids,nseedids - the set of indices for seeds
% maxsteps - the max number of steps
% xlength - the max number of ids in the solution vector
% xids, actual_length - the solution vector
% values - the pagerank vector for xids (already sort decreasing order)
function [actual_length,xids]=ppr_path(A,seedids, ...
                                        alpha,eps,rho,xlength)
[ajPtr,aiPtr,~]=sparse_arrays_ptr(A);
[n,~]=size(A);
[nseedids,~]=size(seedids);
x=zeros(xlength,1);
switch computer
    case {'PCWIN64','GLNXA64','MACI64'}
        indtype = 'int64Ptr';
    case {'PCWIN','GLNX86'}
        indtype = 'uint32Ptr';
    otherwise
        error('Unsupported system');
end
xPtr = libpointer(indtype,x);
seedPtr = libpointer(indtype,seedids);
loadlibrary('../../lib/graph_lib_test/libgraph','../../lib/include/ppr_path_c_interface.h')
if strcmp(indtype,'int64Ptr')
    actual_length = calllib('libgraph','ppr_path64',n,aiPtr, ...
                            ajPtr,0,alpha,eps,rho,seedPtr,nseedids, ...
                            xPtr,xlength);
else
    actual_length = calllib('libgraph','ppr_path32',n,aiPtr, ...
                            ajPtr,0,alpha,eps,rho,seedPtr,nseedids, ...
                            xPtr,xlength);
end    
xids=get(xPtr,'Value');
xids=xids(1:actual_length);
unloadlibrary libgraph;