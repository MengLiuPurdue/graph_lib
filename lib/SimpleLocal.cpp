#ifdef SIMPLELOCAL_H

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <stdint.h>
#include <typeinfo>

using namespace std;

    template<typename vtype, typename itype>
void build_map(itype* ai, vtype* aj, vtype offset, unordered_map<vtype, vtype>& R_map,
        unordered_map<vtype, vtype>& degree_map, vtype* R, vtype nR)
{
    vtype deg;
    for(vtype i = 0; i < nR; i ++){
        R_map[R[i] - offset] = i;
    }
    for(auto iter = R_map.begin(); iter != R_map.end(); ++iter){
        vtype u = iter->first;
        deg = get_degree<vtype, itype>(ai, u);
        for(vtype j = ai[u] - offset; j < ai[u+1] - offset; j ++){
            vtype v = aj[j] - offset;
            if(R_map.count(v) > 0){
                deg --;
            }
        }
        degree_map[u] = deg;
    }
}

    template<typename vtype, typename itype>
vtype SimpleLocal(vtype n, vtype nR, itype* ai, vtype* aj, vtype offset, vtype* R, vtype* ret_set, double delta)
{
    vtype set_size;
    unordered_map<vtype, vtype> R_map;
    unordered_map<vtype, vtype> S_map;
    build_map<vtype, itype>(ai, aj, offset, R_map, degree_map, R, nR);
    itype total_degree = ai[n] - offset;
    pair<itype, itype> set_stats = get_stats<vtype, itype>(ai, aj, offset, R_map, nR);
    itype curvol = set_stats.first;
    itype curcutsize = set_stats.second;
    double alpha = (double)curcutsize/(double)min(total_degree - curvol, curvol);
    pair<double, vector<vtype>> ret_F_S = 3StageFlow(ai, aj, R_map, alpha, delta);
    double F = ret_F_S.first;
    vtype* S = &ret_F_S.second;
    while(F < alpha * curvol){
        for(vtype i = 0; i < nS; i ++){
            S_map[S[i] - offset] = i;
            set_stats = get_stats<vtype, itype>(ai, aj, offset, S_map, nS);
            curvol = set_stats.first;
            curcutsize = set_stats.second;
            alpha = (double)curcutsize/(double)min(total_degree - curvol, curvol);
            ret_set = S;
            ret_F_S = 3StageFlow(ai, aj, R_map, alpha, delta);
        }
    }
    return set_size;
}

    template<typename vtype, typename itype>
pair<itype, itype> get_stats(itype* ai, vtype* aj, vtype offset, unordered_map<vtype, vtype>& R_map, vtype nR)
{
    itype curvol = 0;
    itype curcutsize = 0;
    for(auto R_iter = R_map.begin(); R_iter != R_map.end(); ++ R_iter){
        vtype v = R_iter->first;
        itype deg = get_degree<vtype, itype>(ai, v);
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

    template<typename vtype, typename itype>
pair<double, vector<vtype>> ret_F_S = 3StageFlow(itype* ai, vtype* aj, unordered_map<vtype, vtype>& R_map, double alpha, double delta)
{
}

#endif
