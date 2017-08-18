/**
 * INPUT:
 *     alpha     - teleportation parameter between 0 and 1
 *     rho       - l1-reg. parameter
 *     v         - seed node
 *     v_num     - number of seed nodes
 *     ai,aj,a   - Compressed sparse row representation of A
 *     d         - vector of node strengths
 *     epsilon   - accuracy for termination criterion
 *     n         - size of A

 * OUTPUT:
 *     p              - PageRank vector as a row vector
 *     not_converged  - flag indicating that maxiter has been reached
 *     grad           - last gradient
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <stdint.h>
#include <cmath>

#include "include/proxl1PRaccel_c_interface.h"

using namespace std;

    template<typename vtype>
double find_max(double* grad, double* ds, vtype n){
    double max_num = 0;
    for(vtype i = 0; i < n; i ++){
        //cout << max_num << " " << grad[i]/ds[i] << endl;
        max_num = max(max_num,abs(grad[i]/ds[i]));
    }
    return max_num;
}

    template<typename vtype, typename itype>
void update_grad(double* grad, double* y, double* c, itype* ai, vtype* aj, double* a,
                 vtype n, double alpha)
{
    double* temp = new double[n]();
    double* temp1 = new double[n]();
    for(vtype i = 0; i < n; i ++){
        for(itype j = ai[i]; j < ai[i+1]; j ++){
            temp[i] += a[j]*y[aj[j]];
            temp1[aj[j]] += a[j]*y[i];
        }
        
    }
    for(vtype i = 0; i < n; i ++){
        grad[i] = (1+alpha)/2*y[i] - (temp[i]+temp1[i])/2 - c[i];
    }
}

    template<typename vtype, typename itype>
vtype proxl1PRaccel(vtype n, itype* ai, vtype* aj, double* a, double alpha, double rho,
                    vtype* v, vtype v_num, double* d, double epsilon, double* grad, double* p,
                    vtype maxiter)
{
    double* ds = new double[n];
    double* dsinv = new double[n];
    vtype not_converged = 0;
    for(vtype i = 0; i < n; i ++){
        ds[i] = sqrt(d[i]);
        dsinv[i] = 1/ds[i];
    }
    
    /*cout << "dsinv" << endl;
    for(vtype i = 0; i < n; i ++){
        cout << dsinv[i] << endl;
    }*/
    
    for(vtype i = 0; i < n; i ++){
        for(itype j = ai[i]; j < ai[i+1]; j ++){
            a[j] = a[j] * dsinv[i] * dsinv[aj[j]] * (1-alpha)/2;
        }
    }
    /*cout << "a" << endl;
    for(vtype i = 0; i < 8; i ++){
        cout << a[i] << endl;
    }*/
    double* c = new double[n]();
    for(vtype i = 0; i < v_num; i ++){
        grad[v[i]] = -1 * alpha / ds[v[i]];
        c[v[i]] = -1 * grad[v[i]];
    }
    double* q = new double[n]();
    double* q_old = new double[n]();
    double* y = new double[n]();
    double z;
    size_t iter = 0;
    double thd = (1 + epsilon) * rho * alpha;
    //cout << thd << " " << find_max<vtype>(grad,ds,n) << endl;
    double thd1,betak;
    while((iter < maxiter) && (find_max<vtype>(grad,ds,n) > thd)){
        iter ++;
        q_old = q;
        for(vtype i = 0; i < n; i ++){
            z = y[i] - grad[i];
            thd1 = rho*alpha*ds[i];
            if(z > thd1){
                q[i] = z - thd1;
            }
            else if(z < -1 * thd1){
                q[i] = z + thd1;
            }
            else{
                q[i] = 0;
            }
        }
        if(iter == 1){
            betak = 0;
        }
        else{
            betak = (1-sqrt(alpha))/(1+sqrt(alpha));
        }
        for(vtype i = 0; i < n; i ++){
            y[i] = q[i] + betak*(q[i]-q_old[i]);
        }
        update_grad(grad,y,c,ai,aj,a,n,alpha);
        
        /*if(iter == 1){
            cout << "y" << endl;
            for(vtype i = 0; i < n; i ++){
                cout << y[i] << endl;
            }
            cout << "q" << endl;
            for(vtype i = 0; i < n; i ++){
                cout << q[i] << endl;
            }
            cout << "c" << endl;
            for(vtype i = 0; i < n; i ++){
                cout << c[i] << endl;
            }
            cout << "grad" << endl;
            for(vtype i = 0; i < n; i ++){
                cout << grad[i] << endl;
            }
        }*/
    }
    
    if(iter >= maxiter){
        not_converged = 1;
    }
    
    for(vtype i = 0; i < n; i ++){
        p[i] = q[i]*ds[i];
    }
    
    return not_converged;
}

uint32_t proxl1PRaccel32(uint32_t n, uint32_t* ai, uint32_t* aj, double* a, double alpha,
                         double rho, uint32_t* v, uint32_t v_num, double* d, double epsilon,
                         double* grad, double* p, uint32_t maxiter)
{
    return proxl1PRaccel<uint32_t,uint32_t>(n,ai,aj,a,alpha,rho,v,v_num,d,epsilon,grad,p,maxiter);
}

int64_t proxl1PRaccel64(int64_t n, int64_t* ai, int64_t* aj, double* a, double alpha,
                        double rho, int64_t* v, int64_t v_num, double* d, double epsilon,
                        double* grad, double* p, int64_t maxiter)
{
    return proxl1PRaccel<int64_t,int64_t>(n,ai,aj,a,alpha,rho,v,v_num,d,epsilon,grad,p,maxiter);
}

uint32_t proxl1PRaccel32_64(uint32_t n, int64_t* ai, uint32_t* aj, double* a, double alpha,
                            double rho, uint32_t* v, uint32_t v_num, double* d, double epsilon,
                            double* grad, double* p, uint32_t maxiter)
{
    return proxl1PRaccel<uint32_t,int64_t>(n,ai,aj,a,alpha,rho,v,v_num,d,epsilon,grad,p,maxiter);
}
