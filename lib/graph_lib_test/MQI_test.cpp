#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <stdint.h>
#include <typeinfo>

#include "MQI.h"

using namespace std;

int main()
{
    int64_t ai[] = {0,3,5,8,12,14};
    int64_t aj[] = {1,2,3,0,3,0,3,4,0,1,2,4,2,3};
    int64_t R[] = {3,4};
    int64_t nR = 2;
    int64_t n = 5;
    MQI<int64_t, int64_t>(n, nR, ai, aj, R);

    return EXIT_SUCCESS;
}

