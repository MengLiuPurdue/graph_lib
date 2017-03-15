% A matlab wrapper for sweep cut procedure
% A - the sparse matrix representing the symmetric graph
% ids - the order of vertices given
% results - the best set with the smallest conductance
% actual_length - the number of vertices in the best set
% num - the number of vertices given
% values - A vector scoring each vertex (e.g. pagerank value). 
%          This will be sorted and turned into one of the other inputs.
% fun_id - 0 for sweepcut_with_sorting and 1 for sweepcut_without_sorting
function [actual_length,results]=matlab_wrapper(A,ids,num,values,fun_id)
[ajPtr,aiPtr,~]=sparse_arrays_ptr(A);
[n,~]=size(A);
switch computer
    case {'PCWIN64','GLNXA64','MACI64'}
        indtype = 'int64Ptr';
    case {'PCWIN','GLNX86'}
        indtype = 'uint32Ptr';
    otherwise
        error('Unsupported system');
end
valuesPtr = libpointer('doublePtr',values);
results = zeros(num,1);
resultsPtr = libpointer(indtype,results);
idsPtr = libpointer(indtype,ids);
offset=0;
loadlibrary('libsweepcut','c_interface.h')
if strcmp(indtype,'int64Ptr')
    if fun_id == 1
        actual_length = calllib('libsweepcut','sweepcut_without_sorting64',idsPtr,...
                                resultsPtr,num,n,aiPtr,ajPtr,offset);
    elseif fun_id == 0
        actual_length = calllib('libaclpagerank','sweepcut_with_sorting64',valuesPtr,...
                                idsPtr,resultsPtr,num,n,aiPtr,ajPtr,offset);
    else
        error('Please specify your function (0 for sweepcut_with_sorting and 1 for sweepcut_without_sorting)');
    end
else
    if fun_id == 1
        actual_length = calllib('libsweepcut','sweepcut_without_sorting32',idsPtr,...
                                resultsPtr,num,n,aiPtr,ajPtr,offset);
    elseif fun_id == 0
        actual_length = calllib('libaclpagerank','sweepcut_with_sorting32',valuesPtr,...
                                idsPtr,resultsPtr,num,n,aiPtr,ajPtr,offset);
    else
        error('Please specify your function (0 for sweepcut_with_sorting and 1 for sweepcut_without_sorting)');
    end
end    
results = get(resultsPtr,'Value');
results = results(1:actual_length);
unloadlibrary libsweepcut;