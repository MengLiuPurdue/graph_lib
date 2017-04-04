#ifdef READ_DATA_HPP

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "../include/readData.hpp"

using namespace std;

template<typename vtype>
struct edge{
    vtype u;
    vtype v;
    double w;
};

template<typename vtype>
bool myobject(edge<vtype> i, edge<vtype> j)
{
    if(i.u < j.u){
        return true;
    }
    else if(i.u == j.u){
        return (i.v < j.v);
    }
    else{
        return false;
    }
}

template<typename vtype, typename itype>
void list_to_CSR(vtype m, itype n, vtype* ei, vtype* ej, double* w, 
        itype* ai, vtype* aj, double* a)
{
    edge<vtype>* edge_list = (edge<vtype>*)malloc(sizeof(edge<vtype>) * n);
    for(itype i = 0; i < n; i ++){
        edge_list[i].u = ei[i];
        edge_list[i].v = ej[i];
        edge_list[i].w = w[i];
    }
    sort(edge_list, edge_list + n, myobject<vtype>);
    ai[0] = 0;
    for(itype i = 0; i < n; i ++){
        ai[edge_list[i].u + 1] ++;
        aj[i] = edge_list[i].v;
        a[i] = edge_list[i].w;
    }
    for(vtype i = 1; i <= m; i ++){
        ai[i] += ai[i-1];
    }
}

char* readSMAT(const char* filename)
{
    FILE *rptr = fopen(filename, "r");
    fseek(rptr, 0, SEEK_END);
    size_t fsize = ftell(rptr);
    char *read_file = (char *)malloc(sizeof(char) * fsize);
    fseek(rptr, 0, SEEK_SET);
    fread(read_file, sizeof(char), fsize, rptr);
    fclose(rptr);

    return read_file;
}

template<typename vtype, typename itype>
void readList(const char* filename, vtype* m, itype* n, vtype** ei, vtype** ej, double** w)
{
    char* read_file = readSMAT(filename);
    stringstream ss;
    ss << read_file;
    free(read_file);
    ss >> (*m);
    ss >> (*m);
    ss >> (*n);
    *ei = (vtype*)malloc(sizeof(vtype) * (*n));
    *ej = (vtype*)malloc(sizeof(vtype) * (*n));
    *w = (double*)malloc(sizeof(double) * (*n));
    for(itype i = 0; i < (*n); i ++){
        ss >> (*ei)[i];
        ss >> (*ej)[i];
        ss >> (*w)[i];
    }
    ss.str("");
}

#endif
