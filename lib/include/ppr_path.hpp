#ifndef PPR_PATH_HPP
#define PPR_PATH_HPP

#include <vector>
#include "sparsehash/dense_hash_map.h"
#include "sparseheap.hpp" // include our heap functions
#include "sparserank.hpp" // include our sorted-list functions
#include "sparsevec.hpp" // include our sparse hashtable functions

using namespace std;

template<typename vtype, typename itype>
struct sparserow{
        vtype m;
        vtype n;
        itype* ai;
        vtype* aj;
        vtype offset;
        itype volume;
};


template<typename vtype, typename itype>
vtype ppr_path(vtype n, itype* ai, vtype* aj, vtype offset, double alpha,
               double eps, double rho, vtype* seedids, vtype nseedids, vtype* xids,
               vtype xlength, double* values, int flag);

template<typename vtype>
struct sweep_info {
    vtype num_sweeps;
    std::vector<vtype> rank_of_best_cond;
    std::vector<double> cond;
    std::vector<double> vol;
    std::vector<double> cut;
    double best_cond_global;
    vtype rank_of_bcond_global;
    double vol_of_bcond_global;
    double cut_of_bcond_global;

    double best_cond_this_sweep;
    vtype back_ind;
    
    sweep_info(size_t initial_size)
    : num_sweeps(0), rank_of_best_cond(initial_size,0), cond(initial_size,0.0), vol(initial_size,0.0),
        cut(initial_size,0.0), best_cond_global(1.0), rank_of_bcond_global(0),
        vol_of_bcond_global(0.0), cut_of_bcond_global(0.0), best_cond_this_sweep(1.0),
        back_ind(0)
    { 

    }
};

template<typename vtype>
struct eps_info {
    vtype num_epsilons;
    
    std::vector<double> epsilons;
    std::vector<double> conds;
    std::vector<double> cuts;
    std::vector<double> vols;
    std::vector<vtype> setsizes;
    std::vector<vtype> stepnums;
        
    void update(vtype stepn, double eps, double cond, double cut, double vol, vtype sets){
        epsilons.push_back(eps);
        conds.push_back(cond);
        cuts.push_back(cut);
        vols.push_back(vol);
        setsizes.push_back(sets);
        stepnums.push_back(stepn);
        num_epsilons++;
        return;
    }
    
    eps_info(size_t init_size)
    : num_epsilons(0)
    {
    }
};


template<typename vtype>
struct rank_record {
    vtype lastval;
    std::vector<vtype> starts;
    std::vector<vtype> ends;
    std::vector<vtype> nodes;
    std::vector<vtype> deg_of_pushed;
    std::vector<size_t> size_of_solvec;
    std::vector<size_t> size_of_r;
    std::vector<double> val_of_push;
    std::vector<double> global_bcond;

    vtype nrank_changes;
    vtype nrank_inserts;
    vtype nsteps; 
    vtype size_for_best_cond; 

    void update_record(vtype start_val, vtype end_val, vtype node_val, vtype deg,
                    size_t r_sz, double val_pushed, double gbcond){
        starts.push_back(start_val);
        ends.push_back(end_val);
        nodes.push_back(node_val);
        deg_of_pushed.push_back(deg);
        size_of_solvec.push_back(nrank_inserts);
        size_of_r.push_back(r_sz);
        val_of_push.push_back(val_pushed);
        global_bcond.push_back(gbcond);
        nsteps++;
        nrank_changes++;
        if (start_val == nrank_inserts){
            nrank_inserts++;
        }
    }
    
    rank_record()
    : nrank_changes(0), nrank_inserts(0), nsteps(0), size_for_best_cond(0)
    {
        lastval = std::numeric_limits<vtype>::max();
    }
};


template<typename vtype, typename itype>
struct rank_map{
    typedef google::dense_hash_map<vtype, vtype> Map;
};

template<typename vtype, typename itype>
vtype ppr_path(vtype n, itype* ai, vtype* aj, vtype offset, double alpha, 
        double eps, double rho, vtype* seedids, vtype nseedids, vtype* xids, 
        vtype xlength);

template<typename vtype, typename itype>
bool resweep(vtype r_end, vtype r_start, sparserow<vtype, itype>* G,
            sparse_max_rank<vtype,double,size_t>& rankinfo, sweep_info<vtype>& swinfo);

template<typename vtype, typename itype>
vtype rank_permute(std::vector<vtype> &cluster, vtype r_end, vtype r_start);

template<typename vtype, typename itype>
void graphdiffseed(sparserow<vtype, itype>* G, sparsevec& set, const double t, const double eps_min,
        const double rho, const vtype max_push_count, eps_info<vtype>& ep_stats, rank_record<vtype>& rkrecord,
        std::vector<vtype>& cluster);

template<typename vtype, typename itype>
void hypercluster_graphdiff_multiple(sparserow<vtype, itype>* G, const std::vector<vtype>& set,
                        double t, double eps, double rho, eps_info<vtype>& ep_stats, rank_record<vtype>& rkrecord,
                         std::vector<vtype>& cluster);

template<typename vtype, typename itype>
void copy_array_to_index_vector(const vtype* v, std::vector<vtype>& vec, vtype n, vtype offset);

template<typename vtype, typename itype>
size_t sr_degree(sparserow<vtype, itype>* s, vtype u);

#include "../ppr_path.cpp"
#endif
