#include <unordered_map>

using namespace std;

template<typename vtype, typename itype>
class graph{
    itype m; //number of edges
    vtype n; //number of vertices
    itype* ai; //Compressed sparse row representation
    vtype* aj; //Compressed sparse row representation
    double* a; //Compressed sparse row representation
    vtype offset; //offset for zero based arrays (matlab) or one based arrays (julia)
    double* degrees; //degrees of vertices
public:
    //declare constructors
    graph<vtype,itype>(itype,vtype,itype*,vtype*,double*,vtype,double*);
    
    //common functions
    double get_degree_weighted(vtype id);
    vtype get_degree_unweighted(vtype id);
    pair<itype, itype> get_stats(unordered_map<vtype, vtype>& R_map, vtype nR);
    
    //declare routines
    //functions in aclpagerank.cpp
    vtype aclpagerank(double alpha, double eps, vtype* seedids, vtype nseedids,
                      vtype maxsteps, vtype* xids, vtype xlength, double* values);
    vtype pprgrow(double alpha, double eps, vtype* seedids, vtype nseedids,
                  vtype maxsteps, vtype* xids, vtype xlength, double* values);
    //functions in aclpagerank_weighted.cpp
    vtype aclpagerank_weighted(double alpha, double eps, vtype* seedids, vtype nseedids,
                      vtype maxsteps, vtype* xids, vtype xlength, double* values);
    vtype pprgrow_weighted(double alpha, double eps, vtype* seedids, vtype nseedids,
                  vtype maxsteps, vtype* xids, vtype xlength, double* values);
    //functions in sweepcut.cpp
    vtype sweepcut_with_sorting(double* value, vtype* ids, vtype* results,
                                vtype num, double* ret_cond);
    vtype sweepcut_without_sorting(vtype* ids, vtype* results, vtype num,
                                   double* ret_cond);
    vtype sweep_cut(vtype* ids, vtype* results, vtype num, double* ret_cond);
    //functions in MQI.cpp
    vtype MQI(vtype nR, vtype* R, vtype* ret_set);
    void build_map(unordered_map<vtype, vtype>& R_map,unordered_map<vtype, vtype>& degree_map,
                   vtype* R, vtype nR);
    //functions in proxl1PRaccel.cpp
    vtype proxl1PRaccel(double alpha, double rho, vtype* v, vtype v_nums, double* d,
                        double* ds, double* dsinv, double epsilon, double* grad, double* p,
                        vtype maxiter,double max_time);
};

template<typename vtype, typename itype>
graph<vtype,itype>::graph(itype _m, vtype _n, itype* _ai, vtype* _aj, double* _a,
                          vtype _offset, double* _degrees)
{
    m = _m;
    n = _n;
    ai = _ai;
    aj = _aj;
    a = _a;
    offset = _offset;
    degrees = _degrees;
}

template<typename vtype,typename itype>
double graph<vtype,itype>::get_degree_weighted(vtype id)
{
    double d = 0;
    for(vtype j = ai[id] - offset; j < ai[id+1] - offset; j ++){
        d += a[j];
    }
    return d;
}

template<typename vtype,typename itype>
vtype graph<vtype,itype>::get_degree_unweighted(vtype id)
{
    return ai[id + 1] - ai[id];
}

template<typename vtype, typename itype>
pair<itype, itype> graph<vtype,itype>::get_stats(unordered_map<vtype, vtype>& R_map, vtype nR)
{
    itype curvol = 0;
    itype curcutsize = 0;
    for(auto R_iter = R_map.begin(); R_iter != R_map.end(); ++ R_iter){
        vtype v = R_iter->first;
        itype deg = get_degree_unweighted(v);
        curvol += deg;
        for(itype j = ai[v] - offset; j < ai[v + 1] - offset; j ++){
            if(R_map.count(aj[j] - offset) == 0){
                curcutsize ++;
            }
        }
    }
    
    pair<itype, itype> set_stats (curvol, curcutsize);
    return set_stats;
}
