#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "../include/proxl1PRaccel_c_interface.h"
#include "../include/readData.hpp"

using namespace std;

int main()
{
    int64_t ai[5] = {0,2,5,6,8};
    int64_t aj[8] = {0,3,0,1,3,3,1,3};
    double a[8] = {1,1,1,1,1,1,1,1};
    double rho = 0.5;
    double alpha = 0.7;
    int64_t v[2] = {1,2};
    double d[4] = {2.3,1.7,1.9,4.2};
    double epsilon = 0.0001;
    int64_t maxiter = 1000;
    double* p = new double[4]();
    double* grad = new double[4]();
    int64_t n = 4;
    int64_t v_num = 2;
    double* ds = new double[n];
    double* dsinv = new double[n];
    for(int i = 0; i < n; i ++){
        ds[i] = sqrt(d[i]);
        dsinv[i] = 1/ds[i];
    }
    cout << proxl1PRaccel64(n,ai,aj,a,alpha,rho,v,v_num,d,ds,dsinv,epsilon,grad,p,maxiter) << endl;
    cout << "p" << endl;
    for(int i = 0; i < 4; i ++){
        cout << p[i] << endl;
    }
    cout << "final grad" << endl;
    for(int i = 0; i < 4; i ++){
        cout << grad[i] << endl;
    }
    return EXIT_SUCCESS;
}
