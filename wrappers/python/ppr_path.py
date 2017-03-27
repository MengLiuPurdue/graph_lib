# A python wrapper for ppr_path
# n - number of vertices
# ei,ej - edge list
# alpha - value of alpha
# eps - value of epsilon
# rho - value of rho
# seedids,nseedids - the set of indices for seeds
# xlength - the max number of ids in the solution vector
# xids, actual_length - the solution vector
# fun_id - 0 for aclpagerank64, 1 for aclpagerank32 and 2 for aclpagerank32_64

from operator import itemgetter
import numpy as np
from numpy.ctypeslib import ndpointer
import ctypes
import platform

def ppr_path(n,ei,ej,alpha,eps,rho,seedids,nseedids,xlength,fun_id):
    #sort edge list
    edge_tuples=[]
    edge_num=len(ei)
    for i in range(0,edge_num):
        edge_tuples=edge_tuples+[(ei[i],ej[i])]
    edge_tuples.sort(key=itemgetter(0,1))
    
    #load library
    lib=ctypes.cdll.LoadLibrary("../../lib/graph_lib_test/./libgraph.dylib")
    
    #define proper data type
    if fun_id == 0:
        itype=np.int64
        vtype=np.int64
        ctypes_itype=ctypes.c_int64
        ctypes_vtype=ctypes.c_int64
        fun=lib.ppr_path64
    elif fun_id == 1:
        itype=np.uint32
        vtype=np.uint32
        ctypes_itype=ctypes.c_uint32
        ctypes_vtype=ctypes.c_uint32
        fun=lib.ppr_path32
    elif fun_id == 2:
        itype=np.int64
        vtype=np.uint32
        ctypes_itype=ctypes.c_int64
        ctypes_vtype=ctypes.c_uint32
        fun=lib.ppr_path32_64
    else:
        print "please specify a C funtion: 0 for ppr_path64, 1 for ppr_path32 and 2 for ppr_path32_64"
    
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
    
    #call C function
    seedids=np.array(seedids,dtype=vtype)
    xids=np.zeros(xlength,dtype=vtype)
    fun.restype=ctypes_vtype
    fun.argtypes=[ctypes_vtype,ndpointer(ctypes_itype, flags="C_CONTIGUOUS"),
                  ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                  ctypes_vtype,ctypes.c_double,ctypes.c_double,ctypes.c_double,
                  ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                  ctypes_vtype,
                  ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                  ctypes_vtype]
    actual_length=fun(n,ai,aj,0,alpha,eps,rho,seedids,nseedids,xids,xlength)
    actual_xids=np.empty(actual_length,dtype=vtype)
    actual_xids[:]=[xids[i] for i in range(actual_length)]
    
    return (actual_length,actual_xids)
