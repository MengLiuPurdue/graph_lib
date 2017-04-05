#include <stdlib.h>
#include <iostream>
#include "../include/sweepcut.h"
#include "../include/sweepcut_c_interface.h"
#include "../include/readData.hpp"

using namespace std;

int main()
{
    cout << "test sweepcut on file minnesota.smat with 0 offset" << endl;

    //Read and convert data
    int64_t* ei = NULL;
    int64_t* ej = NULL;
    double* w = NULL;
    int64_t m = 0;
    int64_t n = 0;
    string filename = "../../graph/minnesota.smat";
    cout << "read data file" << endl;
    readList<int64_t, int64_t>(filename.c_str(), &m, &n, &ei, &ej, &w);
    cout << "read data file, done!" << endl;
    int64_t* ai = (int64_t*)malloc(sizeof(int64_t) * (m + 1));
    int64_t* aj = (int64_t*)malloc(sizeof(int64_t) * n);
    double* a = (double*)malloc(sizeof(double) * n);
    cout << "convert edge list to CSR" << endl;
    list_to_CSR<int64_t, int64_t>(m, n, ei, ej, w, ai, aj, a);
    cout << "convert edge list to CSR, done!" << endl;
    free(ei);
    free(ej);
    free(w);

    //Read seed
    filename = "../../graph/minnesota_ids.smat";
    char* read_file = read_file = readSMAT(filename.c_str());
    stringstream ss;
    ss << read_file;
    free(read_file);
    int64_t nids;
    ss >> nids;
    int64_t* ids = (int64_t*)malloc(sizeof(int64_t) * nids);
    for(size_t i = 0; i < nids; i ++){
       ss >> ids[i];
    }
    ss.str("");
    int64_t* bestclus = (int64_t*)malloc(sizeof(int64_t) * nids);
    
    //Begin calling C function
    cout << "calling C function" << endl;
    int64_t offset = 0;
    int64_t actual_length = sweepcut_without_sorting64(ids, bestclus, nids,
                                                       m, ai, aj, offset);
    cout << "actual length" << endl << actual_length << endl;
    cout << "best set" << endl;
    for(int i = 0; i < actual_length; i ++)
    {
        cout << bestclus[i] << " ";
    }
    cout << endl;
    free(ids);
    free(ai);
    free(aj);
    free(a);

    //Check the output
    cout << "compare the output with correct results" << endl;
    filename = "correct_output/sweepcut/minnesota_results.smat";
    read_file = readSMAT(filename.c_str());
    ss << read_file;
    free(read_file);
    int64_t correct_length;
    ss >> correct_length;
    int64_t* correct_clus = (int64_t *)malloc(sizeof(int64_t) * correct_length);
    for(size_t i = 0; i < correct_length; i ++){
        ss >> correct_clus[i];
    }
    ss.str("");
    if(actual_length != correct_length){
        cout << "output length is not correct!" << endl;
        return EXIT_FAILURE;
    }
    else{
        for(size_t i = 0; i < correct_length; i ++){
            if(bestclus[i] != correct_clus[i]){
                cout << "output is not correct!" << endl;
                return EXIT_FAILURE;
            }
        }
    }
    cout << "output is correct!" << endl;
    free(correct_clus);
    free(bestclus);
	return EXIT_SUCCESS;
}
