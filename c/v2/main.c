#include <stdio.h>
#include <stdlib.h>
#include "train.h"
#include "computations.h"
#include "test.h"
#include <sys/time.h>
#include <time.h>

int main() {
    srand(time(0));
/**For timing purposes, if you wish to not time, then comment out the following and other instances**/
    struct timespec start1, end1;
    struct timespec start2, end2;
    double trainTime, testTime;
/****************************************************************************************************/

/**These values can be altered to best speed/accuracy ratings**/    
    int N = 3;
    int D = 1000;

/**The following are declarations and are determined based on what languages you wish to use. Currently, 
      the default is the latin alphabet and the 22 European languages pulled in ../training_texts.  **/
    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";     
    int imSize = sizeof(itemMemory);
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
    char fullName[][11] = {
        "Afrikaanas", "Bulgarian", "Czech", "Danish", "Dutch", "German", "English", "Estonian", "Finnish", "French", "French",
        "Greek", "Hungarian", "Italian", "Latvian", "Lithuanian", "Polish", "Portugese", "Romanian", "Slovenian", "Swedish"};
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels
/******************************************************************************************************/

    int *imhv = (int*)malloc(imSize * D * sizeof(int));
	int *langAM = (int*)malloc(length * D * sizeof(int));

    clock_gettime(CLOCK_MONOTONIC, &start1);                                         //comment out to not time
	train(N, D, itemMemory, imSize, imhv, langLabels, length);
    clock_gettime(CLOCK_MONOTONIC, &end1);                                       //comment out to not time

    clock_gettime(CLOCK_MONOTONIC, &start2);                                     //comment out to not time
	storeLangAM(N, D, langAM, length, langLabels);
	test(N, D, langAM, imhv, itemMemory, imSize, langLabels, length, fullName);
    clock_gettime(CLOCK_MONOTONIC, &end2);

    trainTime = (end1.tv_sec - start1.tv_sec) * 1e9;                          //comment out to not time
    trainTime = (trainTime + (end1.tv_nsec - start1.tv_nsec)) * 1e-9; 
    testTime = (end2.tv_sec - start2.tv_sec) * 1e9;                                     //comment out to not time
    testTime = (testTime + (end2.tv_nsec - start2.tv_nsec)) * 1e-9; 

    printf("Training: %.3fs\nTesting: %.3fs\nTotal: %.3fs\n", trainTime, testTime, (trainTime+testTime));                                  //comment out to not time

    free(imhv);
	free(langAM);
	return 0;
} 

