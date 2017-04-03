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

def ppr_path(n,ai,aj,alpha,eps,rho,seedids,nseedids,xlength):
    
    if platform.architecture() == ('64bit', ''):
        float_type = np.float64
    else:
        float_type = np.float32
        
    dt = np.dtype(ai[0])
    (itype, ctypes_itype) = (np.int64, ctypes.c_int64) if dt.name == 'int64' else (np.uint32, ctypes.c_uint32)    
    dt = np.dtype(aj[0])
    (vtype, ctypes_vtype) = (np.int64, ctypes.c_int64) if dt.name == 'int64' else (np.uint32, ctypes.c_uint32)

    #load library
    lib=ctypes.cdll.LoadLibrary("../../lib/graph_lib_test/./libgraph.dylib")

    if (vtype, itype) == (np.int64, np.int64):
        fun = lib.ppr_path64
    elif (vtype, itype) == (np.int32, np.int64):
        fun = lib.ppr_path32_64
    else:
        fun = lib.ppr_path32

    #call C function
    seedids = np.array(seedids,dtype=vtype)
    xids = np.zeros(xlength,dtype=vtype)
    fun.restype = ctypes_vtype
    fun.argtypes = [ctypes_vtype,ndpointer(ctypes_itype, flags="C_CONTIGUOUS"),
                   ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                   ctypes_vtype,ctypes.c_double,ctypes.c_double,ctypes.c_double,
                   ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                   ctypes_vtype,
                   ndpointer(ctypes_vtype, flags="C_CONTIGUOUS"),
                   ctypes_vtype]
    print("Calling C function")
    actual_length = fun(n,ai,aj,0,alpha,eps,rho,seedids,nseedids,xids,xlength)
    actual_xids = np.empty(actual_length,dtype=vtype)
    actual_xids[:] = [xids[i] for i in range(actual_length)]
    
    return (actual_length,actual_xids)
