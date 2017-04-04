#ifdef PPR_PATH_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <math.h>
#include "include/sparsehash/dense_hash_map.h"

#include "include/ppr_path.hpp"
#include "include/sparseheap.hpp" // include our heap functions
#include "include/sparserank.hpp" // include our sorted-list functions
#include "include/sparsevec.hpp" // include our sparse hashtable functions

using namespace std;

/*
 * @param n, ai, aj, offset - Compressed sparse row representation, 
 *                            with offset for zero based (matlab) or 
 *                            one based arrays (julia)
 * @param alpha - value of alpha
 * @param eps - value of epsilon
 * @param rho - value of rho
 * @param seedids - indices of seed set
 * @param nseedids - number of indices in seed set
 * @param xids, xlength, values - the solution vector
 * */
template<typename vtype, typename itype>
vtype ppr_path(vtype n, itype* ai, vtype* aj, vtype offset, double alpha, 
        double eps, double rho, vtype* seedids, vtype nseedids, vtype* xids, 
        vtype xlength)
{
    cout << "preprocessing start: " << endl;
    sparserow<vtype, itype> r;
    r.m = n;
    r.n = n;
    r.ai = ai;
    r.aj = aj;
    r.offset = offset;

    vector<vtype> seedset;
    copy_array_to_index_vector<vtype, itype>(seedids, seedset, nseedids, offset);
    cout << "preprocessing end: " << endl;

    eps_info<vtype> ep_stats(1000);
    rank_record<vtype> rkrecord;
    vector<vtype> bestclus;

    cout << "call to hypercluster_graphdiff() start" << endl;

    hypercluster_graphdiff_multiple<vtype, itype>(&r, seedset, alpha, eps, rho, ep_stats, rkrecord, bestclus);

    cout << "call to hypercluster_graphdiff() DONE" << endl;

    xlength = bestclus.size();

    for(size_t i = 0; i < xlength; ++ i){
        xids[i] = bestclus[i] + offset;
    }

    return xlength;
}

/*
 * returns index of the best conductance in the vector swinfo.conductances
 */
template<typename vtype, typename itype>
bool resweep(vtype r_end, vtype r_start, sparserow<vtype, itype>* G,
            sparse_max_rank<vtype,double,size_t>& rankinfo, sweep_info<vtype>& swinfo)
{

    // ensure sweep_info vectors are big enough
    if ( r_start >= swinfo.cut.size() ){
        swinfo.cut.resize((r_start+1)*2);
        swinfo.vol.resize((r_start+1)*2);
        swinfo.cond.resize((r_start+1)*2);
        swinfo.rank_of_best_cond.resize((r_start+1)*2);
    }
    double old_bcond = swinfo.best_cond_global;
    (swinfo.num_sweeps) += (r_start-r_end+1);
    double total_degree = G->volume;
    bool was_there_new_bcond = 0;
    vtype rank_of_best_cond = 0;

//  FAST WAY/*
    vtype gindex = rankinfo.rank_to_index(r_end);
    double deg = (double)sr_degree(G,gindex);
    std::vector<double> neighbors_ranked_less_than(r_start-r_end+1,0.0);
    std::vector<double> oldcut(r_start-r_end+1,0.0);
    std::vector<double> oldvol(r_start-r_end+1,0.0);

    // get rankings of neighbors of the shifted node
    for (vtype nzi = G->ai[gindex] - G->offset; nzi < G->ai[gindex+1] - G->offset; nzi++){
        vtype temp_gindex = G->aj[nzi] - G->offset;
        vtype temp_rank = rankinfo.index_to_rank(temp_gindex);
        if ( (temp_rank < r_end) && (temp_rank >= 0) ){ neighbors_ranked_less_than[0] += 1.0; }
        if ( (temp_rank > r_end) && (temp_rank <= r_start) ){
            neighbors_ranked_less_than[temp_rank-r_end] = 1.0;
        }
    }
    for (vtype j = 1; j <= (r_start-r_end); j++){
        neighbors_ranked_less_than[j] += neighbors_ranked_less_than[j-1];
    }
    
    // get old cut/vol information
    if (r_end == 0){
        oldcut[0] = 0.0;
        oldvol[0] = 0.0;
    }
    else{
        oldcut[0] = swinfo.cut[r_end-1];
        oldvol[0] = swinfo.vol[r_end-1];
        rank_of_best_cond = swinfo.rank_of_best_cond[r_end-1];
    }    
    for (vtype j = 1; j <= (r_start-r_end); j++){
        oldcut[j] = swinfo.cut[r_end-1+j];
        oldvol[j] = swinfo.vol[r_end-1+j];
    }

    // update volumes and cuts from r_end to r_start
    double cur_cond = 1.0;
    for (vtype j = 0; j <= (r_start-r_end); j++){
        double cur_vol = oldvol[j] + deg;
        swinfo.vol[r_end+j] = cur_vol;
        double cur_cut = oldcut[j] + deg - 2.0*neighbors_ranked_less_than[j];
        swinfo.cut[r_end+j] = cur_cut;
        
        if (cur_vol == 0.0 || cur_vol == total_degree) { cur_cond = 1.0; }
        else { cur_cond = cur_cut/std::min(cur_vol,total_degree-cur_vol); }
    }

    // finally, compute conductance values from r_end to r_start
    for (vtype cur_rank = r_end; cur_rank <= r_start; cur_rank++){
        double cur_cut = swinfo.cut[cur_rank];
        double cur_vol = swinfo.vol[cur_rank];
        if (cur_vol == 0.0 || cur_vol == total_degree) { cur_cond = 1.0; }
        else { cur_cond = cur_cut/std::min(cur_vol,total_degree-cur_vol); }
        swinfo.cond[cur_rank] = cur_cond;
    }

    // ... and update 'rank_of_best_cond' for all indices r_end to r_start
    if ( r_start > swinfo.back_ind ) { swinfo.back_ind = r_start; }
    for (vtype cur_rank = r_end; cur_rank <= swinfo.back_ind; cur_rank++){
        if ( swinfo.cond[cur_rank] < swinfo.cond[rank_of_best_cond] ){ rank_of_best_cond = cur_rank; }
        swinfo.rank_of_best_cond[cur_rank] = rank_of_best_cond;
    }
    rank_of_best_cond = swinfo.rank_of_best_cond[swinfo.back_ind];
    cur_cond = swinfo.cond[rank_of_best_cond];
    swinfo.best_cond_this_sweep = cur_cond;


    if (cur_cond < old_bcond){ // if current best_cond_this_sweep improves...
        swinfo.rank_of_bcond_global = rank_of_best_cond;
        swinfo.best_cond_global = cur_cond; // update best_cond_global
        swinfo.vol_of_bcond_global = swinfo.vol[rank_of_best_cond];
        swinfo.cut_of_bcond_global = swinfo.cut[rank_of_best_cond];
        was_there_new_bcond = 1; // signal that a new best_cond_global was found
    }
    return was_there_new_bcond;
} // END resweep()

           

/*
 * reorders an ordered list to reflect an update to the rankings
 */
template<typename vtype, typename itype>
vtype rank_permute(std::vector<vtype> &cluster, vtype r_end, vtype r_start)
{
    vtype temp_val = cluster[r_start];
    for (vtype ind = r_start; ind > r_end; ind--){ cluster[ind] = cluster[ind-1]; }
    cluster[r_end] = temp_val;
    return (r_start-r_end);
}



/**
 *  graphdiffseed inputs:
 *      G   -   adjacency matrix of an undirected graph
 *      set -   seed vector: the indices of a seed set of vertices
 *              around which cluster forms; normalized so
 *                  set[i] = 1/set.size(); )
 *  output:
 *      p = f(tP) * set
 *              with infinity-norm accuracy of eps * f(t)
 *              in the degree weighted norm
 *  parameters:
 *      t   - the value of t
 *      eps - the accuracy
 *      max_push_count - the total number of steps to run
 */
template<typename vtype, typename itype>
void graphdiffseed(sparserow<vtype, itype>* G, sparsevec& set, const double t, const double eps_min,
        const double rho, const vtype max_push_count, eps_info<vtype>& ep_stats, rank_record<vtype>& rkrecord,
        std::vector<vtype>& cluster )
{
    cout << "ppr_all_mex::graphdiffseed()  BEGIN " << endl;
    G->volume = (double)(G->ai[G->m] - G->offset);
    vtype npush = 0;
    vtype nsteps = 0;
    double best_eps = 1.0;
    double cur_eps = 1.0;
    std::vector<double> epsilons;
    std::vector<double> conds;
//    vtype stagnant = 0;
    
    // ***** initialize residual, solution, and bookkeeping vectors
    sparse_max_heap<vtype,double,size_t> r(1000);
    sparse_max_rank<vtype,double,size_t> solvec(1000);
    for (sparsevec::map_type::iterator it=set.map.begin(),itend=set.map.end();
         it!=itend;++it) {
        r.update(it->first,it->second); // "update" handles the heap internally
    }
    sweep_info<vtype> spstats(1000);
    
    cur_eps = r.look_max();

    cout << "ppr_all_mex::graphdiffseed()  variables declared, begin WHILE loop " << endl;

    while ( (npush < max_push_count) && (cur_eps > eps_min) ) {
        // STEP 1: pop top element off of heap
        double rij, rij_temp, rij_res;
        vtype ri = r.extractmax(rij_temp); // heap handles sorting internally
        double degofi = (double)sr_degree(G,ri);
        rij_res = cur_eps*rho;
        r.update(ri, rij_res ); // handles the heap internally
        rij = rij_temp - rij_res;


        // STEP 2: update soln vector
        bool new_bcond = 0;            
        size_t rank_start;
        size_t old_size = solvec.hsize;
        size_t rank_end = solvec.update(ri, rij, rank_start); // handles sorting internally.
                // Sets rank_start to the rank ri had before it was updated.
                // Sets rank_end to the rank ri has after it was updated.

        // STEP 3: update sweeps for new solution vector
        if ( rank_start == old_size ){ // CASE (1): new entry
            new_bcond = resweep<vtype, itype>(rank_end, old_size, G, solvec, spstats);
            rkrecord.update_record(old_size, rank_end, ri, degofi, r.hsize, rij, spstats.best_cond_global);            
        }
        else if( (rank_start < old_size) && (rank_start > rank_end) ){ // CASE (2): existing entry changes rank
            new_bcond = resweep<vtype, itype>(rank_end, rank_start, G, solvec, spstats);
            rkrecord.update_record(rank_start, rank_end, ri, degofi, r.hsize, rij, spstats.best_cond_global);
        } 
        else {
            // CASE (3): no changes to sweep info, just resweep anyway.
            new_bcond = resweep<vtype, itype>(rank_end, rank_start, G, solvec, spstats);
            rkrecord.update_record(rank_start, rank_end, ri, degofi, r.hsize, rij, spstats.best_cond_global);
        }

        // STEP 4: update residual
        double update = t*rij;
        for (vtype nzi=G->ai[ri] - G->offset; nzi < G->ai[ri+1] - G->offset; ++nzi) {
            vtype v = G->aj[nzi] - G->offset;
            //cout << "update " << update << " sr_degree " << sr_degree(G,v) << endl;
            r.update(v,update/(double)sr_degree(G,v)); // handles the heap internally            
        }

        // STEP 5: update cut-set stats, check for convergence
        double cur_max = r.look_max();
        if (cur_max < cur_eps){ // we've reached a lower value of || ||_{inf},
            cur_eps = cur_max;  // so update cut stats for new val of cur_eps
            vtype rank_of_bcond = spstats.rank_of_best_cond[spstats.back_ind];            
            double loc_bcond = spstats.cond[rank_of_bcond];
            double cur_vol = spstats.vol[rank_of_bcond];
            double cur_cut = spstats.cut[rank_of_bcond];
            ep_stats.update(nsteps, cur_eps, loc_bcond,cur_cut, cur_vol, (rank_of_bcond+1) );
        }         
        if ( new_bcond == 1 ){ // new best_cond_global, so update
            best_eps = cur_eps;
            rkrecord.size_for_best_cond = rkrecord.nrank_changes;
        }
        nsteps++;        
        npush+=degofi;
    }//END 'while'
    cout << "WHILE done " << endl;
    
    //reconstruct bestcluster from the record of rank changes, rkrecord
    cluster.resize(rkrecord.nrank_inserts);
    vtype cluster_length = 0;
    vtype num_rank_swaps = 0;
    for (vtype j = 0; j < rkrecord.size_for_best_cond ; j++){
        vtype rs = rkrecord.starts[j];
        vtype re = rkrecord.ends[j];
        vtype rn = rkrecord.nodes[j];
        if (rs == rkrecord.size_of_solvec[j]){ // node rn added for the first time
            cluster[cluster_length] = rn;
            rs = cluster_length;
            cluster_length++;
        }
        num_rank_swaps += rank_permute<vtype, itype>(cluster, re, rs);   
    }    
    cluster.resize(spstats.rank_of_bcond_global+1); // delete nodes outside best cluster

}  // END graphdiffseed()



/** Cluster will contain a list of all the vertices in the cluster
 * @param set - the set of starting vertices to use
 * @param t - scaling factor in f(t*A)
 * @param eps - the solution tolerance eps
 * @param p - the solution vector
 * @param r - the residual vector
 * @param a - vector which supports .push_back to add vertices for the cluster
 * @param stats - a structure for statistics of the computation
 */

template<typename vtype, typename itype>
void hypercluster_graphdiff_multiple(sparserow<vtype, itype>* G, const std::vector<vtype>& set,
                        double t, double eps, double rho, eps_info<vtype>& ep_stats, rank_record<vtype>& rkrecord,
                         std::vector<vtype>& cluster)
{
    // reset data
    sparsevec r; r.map.clear();
    cout << "beginning of hypercluster_graphdiff_multiple() " << endl;
    size_t maxdeg = 0;
    for (size_t i=0; i<set.size(); ++i) { //populate r with indices of "set"
        assert(set[i] >= 0); assert(set[i] < G->n); // assert that "set" contains indices i: 1<=i<=n
        size_t setideg = sr_degree(G,set[i]);
        r.map[set[i]] += 1.0/(double)(set.size()*(double)setideg);
        // r is normalized to be stochastic, then degree-normalized
        printf("i = %lu \t set[i] = %lld \t setideg = %lu \n", i, set[i], setideg);
        maxdeg = std::max(maxdeg, setideg);
    }
    printf("at last, graphdiffseed: t=%f eps=%f \n", t, eps);
    
    const vtype max_npush = std::min( 
        (vtype)std::numeric_limits<int>::max() , (vtype)(1/((1-t)*eps)) );
    graphdiffseed<vtype, itype>(G, r, t, eps, rho, max_npush, ep_stats, rkrecord, cluster);

}  // END hyper_cluster_graphdiff_multiple()
            
template<typename vtype, typename itype>
void copy_array_to_index_vector(const vtype* v, std::vector<vtype>& vec, vtype n, vtype offset)
{
    vec.resize(n);
    
    for (size_t i=0; i<n; ++i) {
        vec[i] = v[i] - offset;
    }
}  // END copy_array_to_index_vector()

template<typename vtype, typename itype>
size_t sr_degree(sparserow<vtype, itype> *s, vtype u) {
    return (s->ai[u+1] - s->ai[u]);
}

#endif
