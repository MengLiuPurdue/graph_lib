#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int64_t ppr_path64(int64_t n, int64_t* ai, int64_t* aj, int64_t offset, double alpha, 
        double eps, double rho, int64_t* seedids, int64_t nseedids, int64_t* xids, 
        int64_t xlength);
    uint32_t ppr_path32(uint32_t n, uint32_t* ai, uint32_t* aj, uint32_t offset, double alpha, 
        double eps, double rho, uint32_t* seedids, uint32_t nseedids, uint32_t* xids, 
        uint32_t xlength);
    uint32_t ppr_path32_64(uint32_t n, int64_t* ai, uint32_t* aj, uint32_t offset, double alpha, 
        double eps, double rho, uint32_t* seedids, uint32_t nseedids, uint32_t* xids, 
        uint32_t xlength);

#ifdef __cplusplus
}
#endif
