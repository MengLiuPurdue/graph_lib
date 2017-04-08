#ifdef MQI_H

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


    template<typename vtype,typename itype>
vtype get_degree(itype* ai, vtype id)
{
    return ai[id + 1] - ai[id];
}

/*
 * This is an implementation of the MQI algorithm from Lang and Rao (2004). 
 * The goal is to find the best subset of a seed set with the smallest conductance.
 * n - number of nodes in the graph
 * nR - number of nodes in the original seed set
 * ai,aj,offset - the CSR representation of graph with offset for zero based (matlab) and one based arrays (julia)
 * R - the seed set
 * ret_set - the best subset
 * actual_length - the number of nodes in ret_set
 */
    template<typename vtype, typename itype>
vtype MQI(vtype n, vtype nR, itype* ai, vtype* aj, vtype offset, vtype* R, vtype* ret_set)
{
    vtype total_iter = 0;
    unordered_map<vtype, vtype> R_map;
    unordered_map<vtype, vtype> degree_map;
    build_map<vtype, itype>(ai, aj, offset, R_map, degree_map, R, nR);
    itype nedges = 0;
    double condOld = 1;
    double condNew;
    itype total_degree = ai[n] - offset;
    pair<itype, itype> set_stats = get_stats<vtype, itype>(ai, aj, offset, R_map, nR);
    itype curvol = set_stats.first;
    itype curcutsize = set_stats.second;
    nedges = curvol - curcutsize + 2 * nR;
    //cout << "deg " << total_degree << " cut " << curcutsize << " vol " << curvol << endl;
    condNew = (double)curcutsize/(double)min(total_degree - curvol, curvol);
    cout << "iter: " << total_iter << " conductance: " << condNew << endl;
    total_iter ++;
    vtype* mincut = (vtype*)malloc(sizeof(vtype) * (nR + 2));
    pair<double, vtype> retData = max_flow<vtype, itype>(ai, aj, offset, curvol, curcutsize, nedges,
            nR + 2, R_map, degree_map, nR, nR + 1, mincut);
    vtype nRold = nR;
    vtype nRnew; 
    while(condNew < condOld){
        nRnew = nRold - retData.second + 1;
        //cout << nRnew << " " << nedges << endl;
        vtype* Rnew = (vtype*)malloc(sizeof(vtype) * nRnew);
        vtype iter = 0;
        for(auto R_iter = R_map.begin(); R_iter != R_map.end(); ++R_iter){
            vtype u = R_iter->first;
            vtype u1 = R_iter->second;
            if(mincut[u1] == 0){
                Rnew[iter] = u + offset;
                iter ++;
            }
        }
        condOld = condNew;
        R_map.clear();
        degree_map.clear();
        build_map<vtype, itype>(ai, aj, offset, R_map, degree_map, Rnew, nRnew);
        set_stats = get_stats<vtype, itype>(ai, aj, offset, R_map, nRnew);
        curvol = set_stats.first;
        curcutsize = set_stats.second;
        nedges = curvol - curcutsize + 2 * nRnew;
        if(nRnew > 0){
            condNew = (double)curcutsize/(double)min(total_degree - curvol, curvol);
            //cout << "here" << nedges << " " << curvol << " " << curcutsize << endl;
            retData = max_flow<vtype, itype>(ai, aj, offset, curvol, curcutsize, nedges, nRnew + 2,
                    R_map, degree_map, nRnew, nRnew + 1, mincut);
        }
        free(Rnew);
        nRold = nRnew;
        cout << "iter: " << total_iter << " conductance: " << condNew << endl;
        total_iter ++;
    }

    free(mincut);
    vtype j = 0;
    for(auto R_iter = R_map.begin(); R_iter != R_map.end(); ++ R_iter){
        ret_set[j] = R_iter->first + offset;
        j ++;
    }
    return nRnew;
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
#endif
