#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    uint32_t proxl1PRaccel32(uint32_t n, uint32_t* ai, uint32_t* aj, double* a, double alpha,
                             double rho, uint32_t* v, uint32_t v_num, double* d, double epsilon, double* grad,
                             double* p, uint32_t maxiter);
    
    int64_t proxl1PRaccel64(int64_t n, int64_t* ai, int64_t* aj, double* a, double alpha,
                            double rho, int64_t* v, int64_t v_num, double* d, double epsilon, double* grad,
                            double* p, int64_t maxiter);

    uint32_t proxl1PRaccel32_64(uint32_t n, int64_t* ai, uint32_t* aj, double* a, double alpha,
                                double rho, uint32_t* v, uint32_t v_num, double* d, double epsilon, double* grad,
                                double* p, uint32_t maxiter);
#ifdef __cplusplus
}
#endif



