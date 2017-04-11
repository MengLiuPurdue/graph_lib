#include "include/ppr_path_c_interface.h"
#include "include/ppr_path.hpp"
#include <stdint.h>


int64_t ppr_path64(int64_t n, int64_t* ai, int64_t* aj, int64_t offset, double alpha, 
    double eps, double rho, int64_t* seedids, int64_t nseedids, int64_t* xids, 
    int64_t xlength, struct path_info ret_path_results, struct rank_info ret_rank_results)
{
    return ppr_path<int64_t, int64_t>(n, ai, aj, offset, alpha, eps, rho, seedids, nseedids, xids, xlength, ret_path_results, ret_rank_results);
}
uint32_t ppr_path32(uint32_t n, uint32_t* ai, uint32_t* aj, uint32_t offset, double alpha, 
    double eps, double rho, uint32_t* seedids, uint32_t nseedids, uint32_t* xids, 
    uint32_t xlength, struct path_info ret_path_results, struct rank_info ret_rank_results)
{
    return ppr_path<uint32_t, uint32_t>(n, ai, aj, offset, alpha, eps, rho, seedids, nseedids, xids, xlength,ret_path_results, ret_rank_results);
}
uint32_t ppr_path32_64(uint32_t n, int64_t* ai, uint32_t* aj, uint32_t offset, double alpha, 
    double eps, double rho, uint32_t* seedids, uint32_t nseedids, uint32_t* xids, 
    uint32_t xlength, struct path_info ret_path_results, struct rank_info ret_rank_results)
{
    return ppr_path<uint32_t, int64_t>(n, ai, aj, offset, alpha, eps, rho, seedids, nseedids, xids, xlength,ret_path_results, ret_rank_results);
}
