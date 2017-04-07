#ifndef MYMETHOD_H
#define MYMETHOD_H

template<typename vtype, typename itype>
struct sparserow{
        vtype m;
        vtype n;
        itype* ai;
        vtype* aj;
        double* a;
        vtype offset;
};

template<typename vtype, typename itype>
vtype pprgrow(sparserow<vtype, itype>* r, double alpha, double eps, 
                vtype* seedids, vtype nseedids, vtype maxsteps, 
                vtype* xids, vtype xlength, double* values);

template<typename vtype, typename itype>
double get_degree(sparserow<vtype, itype>* rows, vtype id);

template<typename vtype, typename itype>
vtype aclpagerank_weighted(
        vtype n, itype* ai, vtype* aj, double* a, vtype offset,
            //Compressed sparse row representation, with offset for
            //zero based (matlab) or one based arrays (julia)
        double alpha,    //value of alpha
        double eps,    //value of epsilon
        vtype* seedids, vtype nseedids,    //the set indices for seeds
        vtype maxsteps,    //the maximum number of steps
        vtype* xids, vtype xlength, double* values); //the solution vector

//int32_t sweep_cut(node* possible_nodes, int32_t array_size, int32_t* xids, int32_t xlength, sparserow* rows, double* values);


#endif
