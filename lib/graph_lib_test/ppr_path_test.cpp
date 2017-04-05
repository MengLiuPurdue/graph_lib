#include <stdlib.h>
#include <iostream>
#include "../include/ppr_path.hpp"
#include "../include/ppr_path_c_interface.h"
#include <stdint.h>
#include <sstream>
#include <string>
#include "../include/readData.hpp"

using namespace std;

int main()
{
    cout << "test ppr_path on file usps_3nn.smat with 0 offset" << endl;

    //Read and convert data
    string filename;
    filename = "../../graph/usps_3nn.smat";
    int64_t m = 0, n = 0;
    int64_t* ai = NULL, *aj = NULL;
    double* a = NULL;
    read_and_convert<int64_t, int64_t>(filename.c_str(), &m, &n, &ai, &aj, &a);
    
    //Read seed
    filename = "../../graph/usps_3nn_seed.smat";
    stringstream ss;
    int64_t nseedids = 0;
    int64_t* seedids = NULL;
    read_seed<int64_t, int64_t>(filename.c_str(), &nseedids, &seedids);
	double alpha = 0.99;
	double eps = 0.0001;
	int64_t* xids = (int64_t*)malloc(sizeof(int64_t)*m);

    //Begin calling C function
    cout << "calling C function" << endl;
	int64_t actual_length = ppr_path64(m,ai,aj,0,alpha,eps,0.0,seedids,nseedids,xids,m);

	cout << "actual length" << endl;
    cout<<actual_length<<endl;
    cout << "best set" << endl;
    for(size_t i = 0; i < actual_length; ++ i){
        cout << xids[i] << " ";
    }
    cout << endl;
    free(ai);
    free(aj);
    free(a);

    //Check the output
    cout << "compare the output with correct results" << endl;
    filename = "correct_output/ppr_path/usps_3nn_results.smat";
    char* read_file = readSMAT(filename.c_str());
    ss << read_file;
    free(read_file);
    int64_t correct_length;
    ss >> correct_length;
    int64_t* correct_xids = (int64_t *)malloc(sizeof(int64_t) * correct_length);
    for(size_t i = 0; i < correct_length; i ++){
        ss >> correct_xids[i];
    }
    ss.str("");
    if(actual_length != correct_length){
        cout << "output length is not correct!" << endl;
        return EXIT_FAILURE;
    }
    else{
        for(size_t i = 0; i < correct_length; i ++){
            if(xids[i] != correct_xids[i]){
                cout << "output is not correct!" << endl;
                return EXIT_FAILURE;
            }
        }
    }
    cout << "output is correct!" << endl;
    free(correct_xids);
    free(xids);
	return EXIT_SUCCESS;
}
