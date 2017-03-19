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
    cout << "test on graph 1" << endl;
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
    int64_t n = 379;
    int64_t ret_set[23];
    int64_t actual_length = MQI64(n, nR, ai, aj, 0, R, ret_set);
    cout << "output set" << endl;
    for(int i = 0; i < actual_length; i ++){
        cout << ret_set[i] << " ";
    }
    cout << endl << "total number of vertices is " << actual_length << endl;

    cout << endl;
    cout << "test on graph 2" << endl;
    rptr = fopen("../../graph/test1.smat", "r");
    fseek(rptr, 0, SEEK_END);
    fsize = ftell(rptr);
    read_file = (char *)malloc(sizeof(char) * fsize);
    fseek(rptr, 0, SEEK_SET);
    fread(read_file, sizeof(char), fsize, rptr);
    fclose(rptr);
    stringstream ss1;
    ss1 << read_file;
    free(read_file);
    int64_t ai1[2643];
    int64_t aj1[6606];
    for(int i=0; i < 2643; i ++){
        ss1 >> ai1[i];
        ai1[i] ++;
    }
    for(int i=0; i < 6606; i ++){
        ss1 >> aj1[i];
        aj1[i] ++;
    }
    int64_t R1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,74,75,76,77,78,79,80,81,82,83,84,85,86,88,89,90,91,92,93,94,95,96,98,99,100,101,103,104,105,106,107,109,113,115,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,140,141,142,143,144,145,146,148,149,150,151,152,153,156,158,159,160,161,162,163,165,166,167,169,170,172,173,174,177,178,179,180,181,186,188,189,192,193,196,197,198,202,209,210,211,212,213,216,218,219,220,222,224,226,227,228,229,232,233,245,246,247,248,249,250,254,255,258,262,263,266,270,271,272,273,274,276,277,278,279,280,286,287,288,291,292,300,304,324,328};
/*    for(int i = 0; i < 216; i ++){
        R1[i] --;
    }*/
    int64_t nR1 = 216;
    int64_t n1 = 2642;
    int64_t ret_set1[216];
    int64_t actual_length1 = MQI64(n1, nR1, ai1, aj1, 1, R1, ret_set1);
    cout << "output set" << endl;
    for(int i = 0; i < actual_length1; i ++){
        cout << ret_set1[i] << " ";
    }
    cout << endl << "total number of vertices is " << actual_length1 << endl;

    return EXIT_SUCCESS;
}

