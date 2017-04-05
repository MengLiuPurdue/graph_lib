#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "../include/aclpagerank.h"
#include "../include/aclpagerank_c_interface.h"
#include "../include/readData.hpp"

using namespace std;

int main()
{
    cout << "test aclpagerank on file Unknown.smat with 0 offset" << endl;

    //Read and convert data
    int64_t* ei = NULL;
    int64_t* ej = NULL;
    double* w = NULL;
    int64_t m = 0;
    int64_t n = 0;
    string filename = "../../graph/Unknown.smat";
    cout << "read data file" << endl;
    readList<int64_t, int64_t>(filename.c_str(), &m, &n, &ei, &ej, &w);
    cout << "read data file, done!" << endl;
    int64_t* ai = (int64_t*)malloc(sizeof(int64_t) * (m + 1));
    if(ai == NULL){
        return EXIT_FAILURE;
    }
    int64_t* aj = (int64_t*)malloc(sizeof(int64_t) * n);
    double* a = (double*)malloc(sizeof(double) * n);
    cout << "convert edge list to CSR" << endl;
    list_to_CSR<int64_t, int64_t>(m, n, ei, ej, w, ai, aj, a);
    cout << "convert edge list to CSR, done! " << m << " " << ai[1] << endl;
    free(ei);
    free(ej);
    free(w);
    
    //Read seed
    filename = "../../graph/Unknown_seed.smat";
    char* read_file = readSMAT(filename.c_str());
    stringstream ss;
    ss << read_file;
    free(read_file);
    int64_t nseedids;
    ss >> nseedids;
    int64_t* seedids = (int64_t*)malloc(sizeof(int64_t) * nseedids);
    for(size_t i = 0; i < nseedids; i ++){
       ss >> seedids[i];
    }
    ss.str("");
	double alpha = 0.99;
	double eps = pow(10,-7);
    int64_t xlength = 100;
    int64_t maxstep = (size_t)1/(eps*(1-alpha));
	int64_t* xids = (int64_t*)malloc(sizeof(int64_t)*m);
	double* values = (double*)malloc(sizeof(double)*m);

    //Begin calling C function
    cout << "calling C function " << endl;
	int64_t actual_length = aclpagerank64(m,ai,aj,0,alpha,eps,seedids,
            nseedids,maxstep,xids,xlength,values);
	cout << "actual length" << endl;
    cout<<actual_length<<endl;
    cout << "nonzero pagerank sets and values" << endl;
    for(size_t i = 0; i < actual_length; ++ i){
        cout << xids[i] << " ";
    }
    cout << endl;
    for(size_t i = 0; i < actual_length; ++ i){
        cout << values[i] << " ";
    }
    cout << endl;
    free(ai);
    free(aj);
    free(a);
    free(seedids);

    //Check the output
    cout << "compare the output with correct results" << endl;
    filename = "correct_output/aclpagerank/Unknown_set.smat";
    read_file = readSMAT(filename.c_str());
    ss << read_file;
    free(read_file);
    int64_t correct_length;
    ss >> correct_length;
    int64_t* correct_xids = (int64_t *)malloc(sizeof(int64_t) * correct_length);
    for(size_t i = 0; i < correct_length; i ++){
        ss >> correct_xids[i];
    }
    ss.str("");
    filename = "correct_output/aclpagerank/Unknown_values.smat";
    read_file = readSMAT(filename.c_str());
    ss << read_file;
    free(read_file);
    ss >> correct_length;
    double* correct_values = (double *)malloc(sizeof(double) * correct_length);
    for(size_t i = 0; i < correct_length; i ++){
        ss >> correct_values[i];
    }
    ss.str("");

    if(actual_length != correct_length){
        cout << "output length is not correct!" << endl;
        return EXIT_FAILURE;
    }
    else{
        for(size_t i = 0; i < correct_length; i ++){
            if(xids[i] != correct_xids[i] && fabs(values[i] - correct_values[i]) < pow(10, -5)){
                cout << "output is not correct!" << endl;
                return EXIT_FAILURE;
            }
        }
    }
    cout << "output is correct!" << endl;
    free(correct_xids);
    free(correct_values);
    free(values);
    free(xids);
	return EXIT_SUCCESS;
}
