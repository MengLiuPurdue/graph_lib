/**
 * A sweepcut procedure with C interface. It takes a set of indices and a CSR based graph as the input. 
 * Then it outputs the best cluster with the lowest conductance by using sweepcut algorithm. This file
 * implements two kinds of sweepcut procedure. The first one will first sort the given vertices in the 
 * dereasing order of the pagerank value. The second one will not.
 *
 * INPUT:
 *     n        - the number of vertices in the graph
 *     ai,aj    - Compressed sparse row representation
 *     offset   - offset for zero based arrays (matlab) or one based arrays (julia)
 *     alpha    - value of alpha
 *     eps      - value of epsilon
 *     ids      - the order of vertices given
 *     results  - the best set with the smallest conductance
 *     num      - the number of vertices given
 *     values   - A vector scoring each vertex (e.g. pagerank value).
 *
 * OUTPUT:
 *     actual_length - the number of nonzero entries in the best set with the lowest conductance
 *
 * COMPILE:
 *     make sweepcut
 *
 * EXAMPLE:
 *     Use functions from readData.hpp to read a graph and seed from files.
 *     int64_t* bestclus = (int64_t*)malloc(sizeof(int64_t) * nids);
 *     int64_t offset = 0;
 *     int64_t actual_length = sweepcut_without_sorting64(ids, bestclus, nids, m, ai, aj, offset);
 */


#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <stdint.h>

#include "include/sweepcut_c_interface.h"
#include "include/sweepcut.h"

using namespace std;

template<typename vtype>
bool myobject (pair <vtype, double> i, pair <vtype, double> j) { return (i.second>j.second);}

int64_t sweepcut_without_sorting64(int64_t* ids, int64_t* results, int64_t num, 
        int64_t n, int64_t* ai, int64_t* aj, int64_t offset)
{
    return sweepcut_without_sorting<int64_t, int64_t>(ids, results, num, n, ai, aj, offset);
} 

uint32_t sweepcut_without_sorting32(uint32_t* ids, uint32_t* results, uint32_t num, 
        uint32_t n, uint32_t* ai, uint32_t* aj, uint32_t offset)
{
    return sweepcut_without_sorting<uint32_t, uint32_t>(ids, results, num, n, ai, aj, offset);
} 

uint32_t sweepcut_without_sorting32_64(uint32_t* ids, uint32_t* results, uint32_t num, 
        uint32_t n, int64_t* ai, uint32_t* aj, uint32_t offset)
{
    return sweepcut_without_sorting<uint32_t, int64_t>(ids, results, num, n, ai, aj, offset);
} 


int64_t sweepcut_with_sorting64(double* value, int64_t* ids, int64_t* results, int64_t num, 
        int64_t n, int64_t* ai, int64_t* aj, int64_t offset)
{
    return sweepcut_with_sorting<int64_t, int64_t>(value, ids, results, num, n, ai, aj, offset);
} 

uint32_t sweepcut_with_sorting32(double* value, uint32_t* ids, uint32_t* results, uint32_t num, 
        uint32_t n, uint32_t* ai, uint32_t* aj, uint32_t offset)
{
    return sweepcut_with_sorting<uint32_t, uint32_t>(value, ids, results, num, n, ai, aj, offset);
} 

uint32_t sweepcut_with_sorting32_64(double* value, uint32_t* ids, uint32_t* results, uint32_t num, 
        uint32_t n, int64_t* ai, uint32_t* aj, uint32_t offset)
{
    return sweepcut_with_sorting<uint32_t, int64_t>(value, ids, results, num, n, ai, aj, offset);
} 

/**
 * The sweepcut procedure which will first sort the given vertices in the 
 * dereasing order of the pagerank value.
 *
 * INPUT:
 *     rows     - a self defined struct which contains all the info of a CSR based graph
 *     alpha    - value of alpha
 *     eps      - value of epsilon
 *     ids      - the order of vertices given
 *     results  - the best set with the smallest conductance
 *     num      - the number of vertices given
 *     values   - A vector scoring each vertex (e.g. pagerank value).
 *
 * OUTPUT:
 *     actual_length - the number of nonzero entries in the best set with the lowest conductance
 */
template<typename vtype, typename itype>
vtype sweepcut_with_sorting(double* value, vtype* ids, vtype* results, vtype num, vtype n,
        itype* ai, vtype* aj, vtype offset)
{
    pair<vtype, double>* possible_nodes = new pair<vtype, double>[num];
    for(vtype i = 0; i < num; i ++){
        possible_nodes[i].first = ids[i];
        possible_nodes[i].second = value[i];
    }
    sort(possible_nodes, possible_nodes + num, myobject<vtype>);
    vtype newids[num];
    for(vtype i = 0; i < num; i ++){
        newids[i]=possible_nodes[i].first;
    }
    sparserow<vtype, itype> r;
    r.m = n;
    r.n = n;
    r.ai = ai;
    r.aj = aj;
    r.offset = offset;
    vtype actual_length = sweep_cut<vtype, itype>(&r, newids, results, num);

    return actual_length;
}

/**
 * The sweepcut procedure which won't sort the given vertices in the 
 * dereasing order of the pagerank value.
 *
 * INPUT:
 *     rows     - a self defined struct which contains all the info of a CSR based graph
 *     alpha    - value of alpha
 *     eps      - value of epsilon
 *     ids      - the order of vertices given
 *     results  - the best set with the smallest conductance
 *     num      - the number of vertices given
 *     values   - A vector scoring each vertex (e.g. pagerank value).
 *
 * OUTPUT:
 *     actual_length - the number of nonzero entries in the best set with the lowest conductance
 */

template<typename vtype, typename itype>
vtype sweepcut_without_sorting(vtype* ids, vtype* results, vtype num, vtype n,
        itype* ai, vtype* aj, vtype offset)
{
    sparserow<vtype, itype> r;
    r.m = n;
    r.n = n;
    r.ai = ai;
    r.aj = aj;
    r.offset = offset;
    vtype actual_length = sweep_cut<vtype, itype>(&r, ids, results, num);

    return actual_length;
}

template<typename vtype, typename itype>
vtype sweep_cut(sparserow<vtype, itype>* rows, vtype* ids, vtype* results, vtype num)
{
    unordered_map<vtype, size_t> rank;
    for(vtype i = 0; i < num; i ++){
        rank[ids[i] - rows->offset] = i + 1;
    }
    itype total_degree = rows->ai[rows->m] - rows->offset;
    vtype deg, cut_change, neighbor, min_id;
    double cur_cond;
    vtype curcutsize = 0;
    itype curvolume = 0;
    double min_cond = -1;
    for(vtype i = 0; i < num; i ++){
        vtype v = ids[i] - rows->offset;
        deg = get_degree<vtype, itype>(rows, v);
        cut_change = deg;
        for(vtype j = rows->ai[v] - rows->offset; j < rows->ai[v+1] - rows->offset; j ++){
            neighbor = rows->aj[j] - rows->offset;
            if(rank.count(neighbor) > 0 && rank[neighbor] < rank[v]){
                cut_change -= 2;
            }
        }
        curcutsize += cut_change;
        curvolume += deg;
        if(curvolume == 0 || total_degree - curvolume == 0){
            cur_cond = 1;
        }
        else{
            cur_cond = (double)curcutsize / (double)min(curvolume, total_degree - curvolume);
        }
        if(min_cond == -1 || cur_cond < min_cond){
            min_cond = cur_cond;
            min_id = i;
        }    
    }

    for(vtype j = 0; j <= min_id; j ++){
        results[j] = ids[j];
    }

    return min_id + 1;
}

template<typename vtype,typename itype>
vtype get_degree(sparserow<vtype, itype>* rows, vtype id)
{
    return rows->ai[id + 1] - rows->ai[id];
}
