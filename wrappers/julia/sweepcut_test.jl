include("sweepcut.jl")
include("readSMAT.jl")
include("readSeed.jl")
A=readSMAT("../../graph/minnesota.smat");
ids=readSeed("../../graph/minnesota_ids.smat")
(actual_length,results)=sweep_cut(A,ids,~,1);
@show actual_length
@show results