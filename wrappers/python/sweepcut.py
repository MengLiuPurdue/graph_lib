# A python wrapper for sweep cut procedure
# A - the sparse matrix representing the symmetric graph
# ids - the order of vertices given
# results - the best set with the smallest conductance
# actual_length - the number of vertices in the best set
# num - the number of vertices given
# values - A vector scoring each vertex (e.g. pagerank value). 
#          This will be sorted and turned into one of the other inputs.
# fun_id[0] - 0 for 64, 1 for 32 and 2 for 32_64
# fun_id[1] - 0 for sweepcut_with_sorting and 1 for sweepcut_without_sorting

from operator import itemgetter
import numpy as np
from numpy.ctypeslib import ndpointer
import ctypes
import platform

def sweepcut(n,ei,ej,ids,num,values,fun_id):
    #sort edge list
    edge_tuples=[]
    edge_num=len(ei)
    for i in range(0,edge_num):
        edge_tuples=edge_tuples+[(ei[i],ej[i])]
    edge_tuples.sort(key=itemgetter(0,1))
    
    #load library
    lib=ctypes.cdll.LoadLibrary("../../lib/graph_lib_test/./libgraph.dylib")
    
    #define proper data type and choose proper C function
    if fun_id[0] == 0:
        itype=np.int64
        vtype=np.int64
        ctypes_itype=ctypes.c_int64
        ctypes_vtype=ctypes.c_int64
        if fun_id[1] == 0:
            fun=lib.sweepcut_with_sorting64
        elif fun_id[1] == 1:
            fun=lib.sweepcut_without_sorting64
        else:
            print "please specify a C funtion"
    elif fun_id[0] == 1:
        itype=np.uint32
        vtype=np.uint32
        ctypes_itype=ctypes.c_uint32
        ctypes_vtype=ctypes.c_uint32
        if fun_id[1] == 0:
            fun=lib.sweepcut_with_sorting32
        elif fun_id[1] == 1:
            fun=lib.sweepcut_without_sorting32
        else:
            print "please specify a C funtion"
    elif fun_id[0] == 2:
        itype=np.int64
        vtype=np.uint32
        ctypes_itype=ctypes.c_int64
        ctypes_vtype=ctypes.c_uint32
        if fun_id[1] == 0:
            fun=lib.sweepcut_with_sorting32_64
        elif fun_id[1] == 1:
            fun=lib.sweepcut_without_sorting32_64
        else:
            print "please specify a C funtion"
    else:
        print "please specify a C funtion"
    
    if platform.architecture() == ('64bit', ''):
        float_type=np.float64
    else:
        float_type=np.float32
        
    #convert edge list to CSR
    ai=np.zeros(n+1,dtype=itype)
    aj=np.zeros(edge_num,dtype=vtype)
    i=0
    for item in edge_tuples:
        ai[item[0]+1]=ai[item[0]+1]+1
        aj[i]=item[1]
        i=i+1
    for i in range(1,n+1):
        ai[i]=ai[i-1]+ai[i]

    #define proper fun.argtype
    if fun_id[1] == 0:
        fun.argtypes=[ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
                      ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                      ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                      ctypes_vtype,ctypes_vtype,
                      ndpointer(ctypes_itype, flags="C_CONTIGUOUS"),
                      ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                      ctypes_vtype]
    elif fun_id[1] == 1:
        fun.argtypes=[ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                      ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                      ctypes_vtype,ctypes_vtype,
                      ndpointer(ctypes_itype, flags="C_CONTIGUOUS"),
                      ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                      ctypes_vtype]
    else:
        print "please specify a C funtion"
    
    #call C function
    np_ids=np.zeros(num,dtype=vtype)
    for i in range(num):
        np_ids[i]=ids[i]
    values=np.zeros(num,dtype=float_type)
    results=np.zeros(num,dtype=vtype)
    fun.restype=ctypes_vtype
    if fun_id[1] == 0:
        actual_length=fun(values,np_ids,results,num,n,ai,aj,0)
    elif fun_id[1] == 1:
        actual_length=fun(np_ids,results,num,n,ai,aj,0)
    else:
        print "please specify a C funtion"

    actual_results=np.empty(actual_length,dtype=vtype)
    actual_results[:]=[results[i] for i in range(actual_length)]

    return (actual_length,actual_results)
