#ifdef MQI_H

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <stdint.h>
#include <typeinfo>

#include "include/MQI_c_interface.h"

void MQI64(int64_t n, int64_t nR, int64_t* ai, int64_t* aj, int64_t* R)
{
    MQI<int64_t, int64_t>(n, nR, ai, aj, R);
}

void MQI32(int32_t n, int32_t nR, int32_t* ai, int32_t* aj, int32_t* R)
{
    MQI<int32_t, int32_t>(n, nR, ai, aj, R);
}

void MQI32_64(int32_t n, int32_t nR, int64_t* ai, int32_t* aj, int32_t* R)
{
    MQI<int32_t, int64_t>(n, nR, ai, aj, R);
}

using namespace std;


    template<typename vtype, typename itype>
void build_map(itype* ai, vtype* aj, unordered_map<vtype, vtype>* R_map, 
        unordered_map<vtype, vtype>* degree_map, vtype* R, vtype nR)
{
    vtype deg;
    for(vtype i = 0; i < nR; i ++){
        (*R_map)[R[i]] = i;
    }
    for(auto iter = (*R_map).begin(); iter != (*R_map).end(); ++iter){
        vtype u = iter->first;
        deg = get_degree<vtype, itype>(ai, u);
        for(vtype j = ai[u]; j < ai[u+1]; j ++){
            vtype v = aj[j];
            if((*R_map).count(v) > 0){
                deg --;
            }
        }
        (*degree_map)[u] = deg;
    }
}


    template<typename vtype,typename itype>
vtype get_degree(itype* ai, vtype id)
{
    return ai[id + 1] - ai[id];
}

    template<typename vtype, typename itype>
void MQI(vtype n, vtype nR, itype* ai, vtype* aj, vtype* R)
{
    vtype total_iter = 0;
    unordered_map<vtype, vtype> R_map;
    unordered_map<vtype, vtype> degree_map;
    build_map<vtype, itype>(ai, aj, &R_map, &degree_map, R, nR);
    itype nedges = 0;
    itype deg;
    double condOld = 1;
    double condNew;
    itype total_degree = ai[n];
    itype curvol = 0;
    itype curcutsize = 0;
    for(vtype i = 0; i < nR; i ++){
        vtype v = R[i];
        deg = get_degree<vtype, itype>(ai, v);
        curvol += deg;
        for(itype j = ai[v]; j < ai[v + 1]; j ++){
            if(R_map.count(aj[j]) == 0){
                curcutsize ++;
            }
        }
    }
    nedges = curvol - curcutsize + 2 * nR;
    condNew = (double)curcutsize/(double)min(total_degree - curvol, curvol);
    cout << "iter: " << total_iter << " conductance: " << condNew << endl;
    total_iter ++;
    vtype* mincut = (vtype*)malloc(sizeof(vtype) * (nR + 2));
    pair<double, vtype> retData = max_flow<vtype, itype>(ai, aj, curvol, curcutsize, nedges, 
                                                         nR + 2, R_map, degree_map, nR, nR + 1, mincut);
    vtype nRold = nR;
    vtype nRnew; 
    while(condNew < condOld){
        nRnew = nRold - retData.second + 1;
        vtype* Rnew = (vtype*)malloc(sizeof(vtype) * nRnew);
        vtype iter = 0;
        for(auto R_iter = R_map.begin(); R_iter != R_map.end(); ++R_iter){
            vtype u = R_iter->first;
            vtype u1 = R_iter->second;
            if(mincut[u1] == 1){
                deg = get_degree<vtype, itype>(ai, u);
                curvol -= deg;
                for(vtype j = ai[u]; j < ai[u + 1]; j ++){
                    vtype v = aj[j];
                    if(R_map.count(v) > 0){
                        curcutsize ++;
                    }
                    else{
                        curcutsize --;
                    }
                } 
            }
            else{
                Rnew[iter] = u;
                iter ++;
            }
        }
        condOld = condNew;
        nedges = curvol - curcutsize + 2 * nRnew;
        if(nRnew > 0){
            condNew = (double)curcutsize/(double)min(total_degree - curvol, curvol);
            R_map.clear();
            degree_map.clear();
            build_map<vtype, itype>(ai, aj, &R_map, &degree_map, Rnew, nRnew);
            retData = max_flow<vtype, itype>(ai, aj, curvol, curcutsize, nedges, nRnew + 2, 
                                             R_map, degree_map, nRnew, nRnew + 1, mincut);
        }
        free(Rnew);
        nRold = nRnew;
    }
    cout << "iter: " << total_iter << " conductance: " << condNew << endl;
    total_iter ++;

    free(mincut);
}


#endif
