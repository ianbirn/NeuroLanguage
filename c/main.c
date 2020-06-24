#include <stdio.h>
#include <stdlib.h>
#include "train.h"
#include "computations.h"
#include "test.h"
#include <sys/time.h>

int main() {
/**For timing purposes, if you wish to not time, then comment out the following and other instances**/
    struct timeval start, end; 
    struct timeval start2, end2; 
    double trainTime;
    double testTime;
/****************************************************************************************************/

/**These values can be altered to best speed/accuracy ratings**/    
    int N = 4;
    int D = 10000;

/**The following are declarations and are determined based on what languages you wish to use. Currently, 
      the default is the latin alphabet and the 22 European languages pulled in ../training_texts.  **/
    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";     
    int imSize = sizeof(itemMemory);
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels
/******************************************************************************************************/

    int *imhv = (int*)malloc(imSize * D * sizeof(int));
	int *langAM = (int*)malloc(length * D * sizeof(int));

    gettimeofday(&start, NULL);                                            //comment out to not time
	train(N, D, itemMemory, imSize, imhv, langLabels, length);
    gettimeofday(&end, NULL);                                       //comment out to not time

    gettimeofday(&start2, NULL);                                                //comment out to not time
	storeLangAM(N, D, langAM, length, langLabels);
	test(N, D, langAM, imhv, itemMemory, imSize);
    gettimeofday(&end2, NULL);                                           //comment out to not time
    
    trainTime = (end.tv_sec - start.tv_sec) * 1e6; 
    trainTime = (trainTime + (end.tv_usec - start.tv_usec)) * 1e-6; 
    testTime = (end2.tv_sec - start2.tv_sec) * 1e6; 
    testTime = (testTime + (end2.tv_usec - start2.tv_usec)) * 1e-6;                    //comment out to not time

    printf("Training: %.3fs\nTesting: %.3fs\nTotal: %.3fs\n", trainTime, testTime, (trainTime + testTime));   //comment out to not time

    free(imhv);
	free(langAM);
	return 0;
} 

