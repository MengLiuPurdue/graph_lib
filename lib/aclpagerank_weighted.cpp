/*
aclpagerank with C interface

ai,aj,offset - Compressed sparse row representation, with offset for zero based (matlab) on one based arrays (julia)

alpha - value of alpha

eps - value of epsilon

seedids,nseedids - the set of indices for seeds

maxsteps - the max number of steps

xlength - the max number of ids in the solution vector

xids, actual_length - the solution vector

values - the pagerank value vector for xids (already sorted in decreasing order)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <numeric>

#include "include/aclpagerank_weighted.h"
#include "include/aclpagerank_weighted_c_interface.h"

using namespace std;

template<typename vtype>
bool myobject (pair <vtype, double> i, pair <vtype, double> j) { return (i.second>j.second);}

uint32_t aclpagerank_weighted32(
        uint32_t n, uint32_t* ai, uint32_t* aj, double* a, uint32_t offset, 
        double alpha, 
        double eps, 
        uint32_t* seedids, uint32_t nseedids,
        uint32_t maxsteps,
        uint32_t* xids, uint32_t xlength, double* values)
{
    uint32_t actual_length = aclpagerank_weighted <uint32_t, uint32_t> (n, ai, aj, a, offset, alpha, 
                                                   eps, seedids, nseedids, maxsteps, 
                                                   xids, xlength, values);
    return actual_length;
}

int64_t aclpagerank_weighted64(
        int64_t n, int64_t* ai, int64_t* aj, double* a, int64_t offset, 
        double alpha, 
        double eps, 
        int64_t* seedids, int64_t nseedids,
        int64_t maxsteps,
        int64_t* xids, int64_t xlength, double* values)
{
    int64_t actual_length = aclpagerank_weighted <int64_t, int64_t> (n, ai, aj, a, offset, alpha, 
                                                   eps, seedids, nseedids, maxsteps, 
                                                   xids, xlength, values);
    return actual_length;
}

uint32_t aclpagerank_weighted32_64(
        uint32_t n, int64_t* ai, uint32_t* aj, double* a, uint32_t offset, 
        double alpha, 
        double eps, 
        uint32_t* seedids, uint32_t nseedids,
        uint32_t maxsteps,
        uint32_t* xids, uint32_t xlength, double* values)
{
    uint32_t actual_length = aclpagerank_weighted <uint32_t, int64_t> (n, ai, aj, a, offset, alpha, 
                                                   eps, seedids, nseedids, maxsteps, 
                                                   xids, xlength, values);
   return actual_length;
}

template<typename vtype, typename itype>
vtype aclpagerank_weighted(
        vtype n, itype* ai, vtype* aj, double* a, vtype offset,
            //Compressed sparse row representation, with offset for
            //zero based (matlab) or one based arrays (julia)
        double alpha,    //value of alpha
        double eps,    //value of epsilon
        vtype* seedids, vtype nseedids,    //the set indices for seeds
        vtype maxsteps,    //the maximum number of steps
        vtype* xids, vtype xlength, double* values) //the solution vector
{
    sparserow<vtype, itype> r;
    r.m = n;
    r.n = n;
    r.ai = ai;
    r.aj = aj;
    r.a = a;
    r.offset = offset;
    vtype actual_length;
    actual_length=pprgrow<vtype, itype>(&r, alpha, eps, seedids, 
                                   nseedids, maxsteps, xids, 
                                   xlength, values);

    return actual_length;
}

template<typename vtype,typename itype>
double get_weight(sparserow<vtype, itype>* rows, vtype u, vtype v)
{
    double weight;
    for(itype i = rows->ai[u] - rows->offset; i < rows->ai[u + 1] - rows->offset; i ++){
        if(rows->aj[i] == v){
            weight = rows->a[i];
        }
    }
    return weight;
}


template<typename vtype, typename itype>
vtype pprgrow(sparserow<vtype, itype>* rows, double alpha, double eps, 
                vtype* seedids, vtype nseedids, vtype maxsteps, 
                vtype* xids, vtype xlength, double* values)
{
    double* degrees = (double*)malloc(sizeof(double) * rows->m);
    for(size_t i = 0; i < rows->m; i ++){
        double degree = 0;
        for(size_t j = rows->ai[i] - rows->offset; j < rows->ai[i+1] - rows->offset; j ++){
            degree += rows->a[j];
        }
        degrees[i] = degree;
    }
    unordered_map<vtype, double>* x_map = new unordered_map<vtype, double>;
    unordered_map<vtype, double>* r_map = new unordered_map<vtype, double>;
    typename unordered_map<vtype, double>::const_iterator x_iter, r_iter;
    queue<vtype>* Q = new queue<vtype>;
    for(int i = 0; i < nseedids; i ++){
        (*r_map)[seedids[i] - rows->offset] = 1;
        (*x_map)[seedids[i] - rows->offset] = 0;
    }
    for(r_iter = r_map->begin(); r_iter != r_map->end(); ++r_iter){
        if(r_iter->second >= eps * degrees[r_iter->first]){
            Q->push(r_iter->first);
        }
    }
    vtype steps_count = 0;
    vtype j;
    double xj, rj, delta, pushval;
    double sum_iter;
    while(Q->size()>0 && steps_count<maxsteps){
        j = Q->front();
        Q->pop();
        x_iter = x_map->find(j);
        r_iter = r_map->find(j);
        rj = r_iter->second;
        pushval = rj - eps * degrees[j] * 0.5;
        if(x_iter == x_map->end()){
            xj = (1-alpha)*pushval;
            (*x_map)[j] = xj;
        }
        else{
            xj = x_iter->second + (1-alpha)*pushval;
            x_map->at(j) = xj;
        }
        delta = alpha * pushval / degrees[j];
        r_map->at(j) = rj - pushval;
        vtype u;
        double ru_new, ru_old;
        for(itype i = rows->ai[j] - rows->offset; i < rows->ai[j+1] - rows->offset; i++){
            u = rows->aj[i] - rows->offset;
            r_iter = r_map->find(u);
            if(r_iter == r_map->end()){
                ru_old = 0;
                ru_new = rows->a[i] * delta;
                (*r_map)[u] = ru_new;
            }
            else{
                ru_old = r_iter->second;
                ru_new = rows->a[i] * delta + ru_old;
                r_map->at(u) = ru_new;
            }
            if(ru_new > eps * degrees[u] && ru_old <= eps * degrees[u]){
                Q->push(u);
            }
        }
        steps_count ++;
        sum_iter = 0;
        for(r_iter = r_map->begin(); r_iter != r_map->end(); ++r_iter){
            sum_iter += r_iter->second;
        }
        for(x_iter = x_map->begin(); x_iter != x_map->end(); ++x_iter){
            sum_iter += x_iter->second;
        }
        //cout << "step: " << steps_count << " sum_iter: " << sum_iter << endl;
    }
    vtype map_size = x_map->size();
    //cout << "size " << map_size << endl;
    pair <vtype, double>* possible_nodes = new pair <vtype, double>[map_size];
    int i = 0;
    for(x_iter = x_map->begin(); x_iter != x_map->end(); ++x_iter){
        possible_nodes[i].first = x_iter->first;
        possible_nodes[i].second = x_iter->second;
        i++;
    }
    sort(possible_nodes, possible_nodes + map_size, myobject<vtype>);
//    vtype actual_length = sweep_cut(possible_nodes, map_size, xids, xlength, rows, values);
    if(map_size > xlength){
        map_size = xlength;
    }
    for(j = 0; j < map_size; j ++){
        xids[j] = possible_nodes[j].first;
        values[j] = possible_nodes[j].second;
    }
    double sum = 0;
    for(size_t l = 0; l < map_size; l ++){
        sum += values[l];
    }
    
    delete [] possible_nodes;
    delete r_map;
    delete x_map;
    delete Q;

    return map_size;
}

/*template<typename vtype,typename itype>
double get_degree(sparserow<vtype, itype>* rows, vtype id)
{
    double degree = 0;
    for(itype i = rows->ai[id] - rows->offset; i < rows->ai[id + 1] - rows->offset; i ++){
        degree += rows->a[i];
    }
    return degree;
}*/

