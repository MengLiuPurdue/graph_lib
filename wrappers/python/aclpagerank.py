# A python wrapper for aclpagerank
# n - number of vertices
# ei,ej - edge list
# alpha - value of alpha
# eps - value of epsilon
# seedids,nseedids - the set of indices for seeds
# maxsteps - the max number of steps
# xlength - the max number of ids in the solution vector
# xids, actual_length - the solution vector
# values - the pagerank value vector for xids (already sorted in decreasing order)
# fun_id - 0 for aclpagerank64, 1 for aclpagerank32 and 2 for aclpagerank32_64

from operator import itemgetter
import numpy as np
from numpy.ctypeslib import ndpointer
import ctypes
import platform

def python_wrapper(n,ei,ej,alpha,eps,seedids,nseedids,maxsteps,xlength,fun_id):
    #sort edge list
    edge_tuples=[]
    edge_num=len(ei)
    for i in range(0,edge_num):
        edge_tuples=edge_tuples+[(ei[i],ej[i])]
    edge_tuples.sort(key=itemgetter(0,1))
    
    #load library
    lib=ctypes.cdll.LoadLibrary("./libaclpagerank.dylib")
    
    #define proper data type
    if fun_id == 0:
        itype=np.int64
        vtype=np.int64
        ctypes_itype=ctypes.c_int64
        ctypes_vtype=ctypes.c_int64
        fun=lib.aclpagerank64
    elif fun_id == 1:
        itype=np.uint32
        vtype=np.uint32
        ctypes_itype=ctypes.c_uint32
        ctypes_vtype=ctypes.c_uint32
        fun=lib.aclpagerank32
    elif fun_id == 2:
        itype=np.int64
        vtype=np.uint32
        ctypes_itype=ctypes.c_int64
        ctypes_vtype=ctypes.c_uint32
        fun=lib.aclpagerank32_64
    else:
        print "please specify a C funtion: 0 for aclpagerank64, 1 for aclpagerank32 and 2 for aclpagerank32_64"
    
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
    values=np.zeros(xlength,dtype=float_type)
    fun.restype=ctypes_vtype
    fun.argtypes=[ctypes_vtype,ndpointer(ctypes_itype, flags="C_CONTIGUOUS"),
                  ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                  ctypes_vtype,ctypes.c_double,ctypes.c_double,
                  ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                  ctypes_vtype,ctypes_vtype,
                  ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                  ctypes_vtype,ndpointer(ctypes.c_double, flags="C_CONTIGUOUS")]
    actual_length=fun(n,ai,aj,0,alpha,eps,seedids,nseedids,maxsteps,xids,xlength,values)
    actual_values=np.empty(actual_length,dtype=float_type)
    actual_xids=np.empty(actual_length,dtype=vtype)
    actual_values[:]=[values[i] for i in range(actual_length)]
    actual_xids[:]=[xids[i] for i in range(actual_length)]
    
    return (actual_length,actual_xids,actual_values)
