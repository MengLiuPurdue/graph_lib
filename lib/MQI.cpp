#ifdef MQI_H

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <stdint.h>
#include <typeinfo>

using namespace std;

    template<typename vtype,typename itype>
vtype get_degree(itype* ai, vtype id)
{
    return ai[id + 1] - ai[id];
}

    template<typename vtype, typename itype>
void MQI(vtype n, vtype nR, itype* ai, vtype* aj, vtype* R)
{
    unordered_map<vtype, vtype> R_map;
    itype nedges = 0;
    itype deg;
    for(vtype i = 0; i < nR; i ++){
        R_map[R[i]];
    }
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
    cout << nedges << endl;
    condNew = (double)curcutsize/(double)min(total_degree - curvol, curvol);
    cout << curvol << " " << curcutsize << " " << condNew << endl;
    vtype* mincut = (vtype*)malloc(sizeof(vtype) * (n + 2));
    pair<double, vtype> retData = max_flow<vtype, itype>(ai, aj, nR, curcutsize, nedges, n + 2, R_map, n, n + 1, mincut);
    cout << retData.first << endl;
    cout << retData.second << endl;
    while(condNew < condOld){
        for(auto R_iter = R_map.begin(); R_iter != R_map.end(); ++R_iter){
            vtype u = R_iter->first;
            if(mincut[u] == 1){
                R_map.erase(u);
                nR --;
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
        }
        condOld = condNew;
        nedges = curvol - curcutsize + 2 * nR;
        if(nR > 0){
            condNew = (double)curcutsize/(double)min(total_degree - curvol, curvol);
            retData = max_flow<vtype, itype>(ai, aj, nR, curcutsize, nedges, n + 2, R_map, n, n + 1, mincut);
        }
        cout << condNew << endl;
    }

    free(mincut);
}


#endif
