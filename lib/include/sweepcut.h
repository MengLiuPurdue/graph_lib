#ifndef MYMETHOD_H
#define MYMETHOD_H

template<typename vtype, typename itype>
struct sparserow{
        vtype m;
        vtype n;
        itype* ai;
        vtype* aj;
        vtype offset;
};

template<typename vtype, typename itype>
vtype sweepcut_without_sorting(vtype* ids, vtype* results, vtype num, vtype n,
        itype* ai, vtype* aj, vtype offset);

template<typename vtype, typename itype>
vtype sweepcut_with_sorting(double* value, vtype* ids, vtype* results, vtype num, vtype n,
        itype* ai, vtype* aj, vtype offset);

template<typename vtype, typename itype>
vtype sweep_cut(sparserow<vtype, itype>* rows, vtype* ids, vtype* results, vtype num);

template<typename vtype,typename itype>
vtype get_degree(sparserow<vtype, itype>* rows, vtype id);


#endif
