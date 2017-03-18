#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <stdint.h>
#include <typeinfo>
#include <sstream>
#include <stdio.h>

#include "../include/MQI.h"
#include "../include/MQI_c_interface.h"


using namespace std;

int main()
{
    FILE *rptr = fopen("../../graph/test.smat", "r");
    fseek(rptr, 0, SEEK_END);
    size_t fsize = ftell(rptr);
    char *read_file = (char *)malloc(sizeof(char) * fsize);
    fseek(rptr, 0, SEEK_SET);
    fread(read_file, sizeof(char), fsize, rptr);
    fclose(rptr);
    stringstream ss;
    ss << read_file;
    free(read_file);
    int64_t ai[380];
    int64_t aj[1828];
    for(int i=0; i < 380; i ++){
        ss >> ai[i];
    }
    for(int i=0; i < 1828; i ++){
        ss >> aj[i];
    }
    int64_t R[] = {9,10,11,12,49,67,68,69,70,71,72,73,74,75,80,81,121,122,123,169,248,285,363};
    for(int i = 0; i < 23; i ++){
        R[i] --;
    }
    int64_t nR = 23;
    int64_t n = 380;
    MQI64(n, nR, ai, aj, R);
    return EXIT_SUCCESS;
}

