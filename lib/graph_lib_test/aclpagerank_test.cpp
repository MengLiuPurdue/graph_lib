#include <stdlib.h>
#include <iostream>
#include "../include/aclpagerank.h"
#include "../include/aclpagerank_c_interface.h"

using namespace std;

int main()
{
	uint32_t ai[5] = {0,2,5,7,8};
	uint32_t aj[8] = {1,2,0,2,3,0,1,1};
	uint32_t seedids[1] = {0};
	uint32_t nseedids = 1;
	double alpha = 0.5;
	double eps = 0.0001;
	uint32_t maxstep = (size_t)1/(eps*(1-alpha));
	uint32_t xlength = 4;
	uint32_t n = 4;
	uint32_t* xids = (uint32_t*)malloc(sizeof(uint32_t)*n);
	double* values = (double*)malloc(sizeof(double)*n);
	uint32_t actual_length = aclpagerank32(n,ai,aj,0,alpha,eps,seedids,nseedids,maxstep,xids,xlength,values);

	cout << "actual length is:" << endl;
    cout<<actual_length<<endl;
    cout << "xids:" << endl;
    for(int i = 0; i < actual_length; i ++){
        cout << xids[i] << " ";
    }
    cout << endl;
	return EXIT_SUCCESS;
}
