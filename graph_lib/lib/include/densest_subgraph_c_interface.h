#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    double densest_subgraph64(int64_t n, int64_t* ai, int64_t* aj, int64_t offset, int64_t* R, int64_t* ret_set);
    double densest_subgraph32(int32_t n, int32_t* ai, int32_t* aj, int32_t offset, int32_t* R, int32_t* ret_set);
    double densest_subgraph32_64(int32_t n, int64_t* ai, int32_t* aj, int32_t offset, int32_t* R, int32_t* ret_set);

#ifdef __cplusplus
}
#endif

