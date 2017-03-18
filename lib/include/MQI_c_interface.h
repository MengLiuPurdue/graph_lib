#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void MQI64(int64_t n, int64_t nR, int64_t* ai, int64_t* aj, int64_t* R);
    void MQI32(int32_t n, int32_t nR, int32_t* ai, int32_t* aj, int32_t* R);
    void MQI32_64(int32_t n, int32_t nR, int64_t* ai, int32_t* aj, int32_t* R);

#ifdef __cplusplus
}
#endif

