#ifndef READ_DATA_HPP
#define READ_DATA_HPP

#include <iostream>

using namespace std;

template<typename vtype, typename itype>
void readList(const char* filename, vtype* m, itype* n, vtype** ei, vtype** ej, double** w);

template<typename vtype, typename itype>
void list_to_CSR(vtype m, itype n, vtype* ei, vtype* ej, double* w, 
        itype* ai, vtype* aj, double* a);

char* readSMAT(const char* filename);

#include "../graph_lib_test/readData.cpp"
#endif
