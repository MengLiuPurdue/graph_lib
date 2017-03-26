#include <stdlib.h>
#include <iostream>
#include "../include/ppr_path.hpp"
#include "../include/ppr_path_c_interface.h"
#include <stdint.h>
#include <sstream>

using namespace std;

int main()
{
    cout << "test on file ppr_path_test.smat" << endl;
    FILE *rptr = fopen("../../graph/ppr_path_test.smat", "r");
    fseek(rptr, 0, SEEK_END);
    size_t fsize = ftell(rptr);
    char *read_file = (char *)malloc(sizeof(char) * fsize);
    fseek(rptr, 0, SEEK_SET);
    fread(read_file, sizeof(char), fsize, rptr);
    fclose(rptr);
    stringstream ss;
    ss << read_file;
    free(read_file);
    int64_t nai = 9299;
    int64_t naj = 42512;
    int64_t ai[nai];
    int64_t aj[naj];
    for(int i=0; i < nai; i ++){
        ss >> ai[i];
    }
    for(int i=0; i < naj; i ++){
        ss >> aj[i];
    }
	int64_t seedids[1] = {7575};
	int64_t nseedids = 1;
	double alpha = 0.99;
	double eps = 0.0001;
	int64_t xlength = 9298;
	int64_t n = 9298;
	int64_t* xids = (int64_t*)malloc(sizeof(int64_t)*n);
	double* values = (double*)malloc(sizeof(double)*n);
	int64_t actual_length = ppr_path64(n,ai,aj,0,alpha,eps,0.0,seedids,nseedids,xids,xlength,values);

	cout<<actual_length<<endl;
    cout << "best set" << endl;
    for(size_t i = 0; i < actual_length; ++ i){
        cout << xids[i] << endl;
    }
	return EXIT_SUCCESS;
}
