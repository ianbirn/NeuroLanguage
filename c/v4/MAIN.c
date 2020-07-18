#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <locale.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include "train.h"
#include "test.h"
#include "computations.h"

void perm(int D, int *arr);
void genRandomHV(int D, int *randomHV);
void circShift(int n, int d, int *arr);
void lookupItemMemory(int D, int *block, wchar_t *buffer, wchar_t *itemMemory, wchar_t key, int *iMHV, int size);
void computeSumHV(int N, int D, int *sumHV, wchar_t *buffer, int count, wchar_t *itemMemory, int *iMHV, int size);
void binarizeHV(int *v, int szofv);
double norm(int *a, int n);
double dotProduct(int *a, int *b, int n);
double cosAngle(int *a, int *b, int n);

int main(int argc, char** argv) {
    //Setting locale(for utf-8 purposes)
    if (!setlocale(LC_ALL, "en_US.utf8")) {
      printf( "Can't set the specified locale! "
              "Check LANG, LC_CTYPE, LC_ALL.\n");
      return -1;
    }

    //For randomizing
    srand(time(0));

/**For timing purposes, if you wish to not time, then comment out the following and other instances**/
    struct timespec start1, end1;
    struct timespec start2, end2;
    double trainTime, testTime;
/****************************************************************************************************/


/*************************************************/
    int D = atoi(argv[1]);
    int N = 3;
/*************************************************/


    //For training & testing
    char langLabels[][4] = {"afr", "bul", "ces", "dan", "deu", "nld", "ell", "eng", "est", "fin", "fra", "hun", "isl", "ita", "lat", "lit", "nor", "pol", "por", "ron", "rus", "slk", "slv", "spa", "swe", "tuk", "ukr"};
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels


/******************************************************/
/*                  Item Memory                      */
    wchar_t *itemMemory = L" abcdefghijklmnopqrstuvwxyzăãāàáąåäâæßçčćďðéėēèêěëęģğîïíìįīķłļľñńňŉņōôóòõőøöœþšșśşťřŕúűūůųùûüÿýỳžżабвгґдеёжзэєїіийклмнопрстуфхцчшщъьыюяαβγδέεζήηθίικλμνξόοπρστυφχψώω";
    int imSize= wcslen(itemMemory);
/*****************************************************/


/*****************************************************/
/*Conversion of argv[2] to be compatable for testing*/
    const char *str = argv[2];
    int maxSize = 10000;
    wchar_t *unknown = (wchar_t *)malloc(maxSize * sizeof(wchar_t));
    mbstowcs(unknown, str, maxSize);
/*****************************************************/

    //Variable Declaration  
    int *langAM = (int*)malloc(length * D * sizeof(int));
    int *iMHV = (int*)malloc(imSize * D * sizeof(int));

    clock_gettime(CLOCK_MONOTONIC, &start1);  
    train(N, D, itemMemory, imSize, iMHV, langLabels, length);
    clock_gettime(CLOCK_MONOTONIC, &end1);

    clock_gettime(CLOCK_MONOTONIC, &start2);
    test(N, D, langAM, iMHV, itemMemory, imSize, langLabels, length, unknown);
    clock_gettime(CLOCK_MONOTONIC, &end2);

    trainTime = (end1.tv_sec - start1.tv_sec) * 1e9;                          //comment out to not time
    trainTime = (trainTime + (end1.tv_nsec - start1.tv_nsec)) * 1e-9; 
    testTime = (end2.tv_sec - start2.tv_sec) * 1e9;                                     //comment out to not time
    testTime = (testTime + (end2.tv_nsec - start2.tv_nsec)) * 1e-9; 

    wprintf(L"Training: %.3fs\nTesting: %.3fs\nTotal: %.3fs\n", trainTime, testTime, (trainTime+testTime));                                  //comment out to not time

    free(unknown);
    free(iMHV);
    free(langAM);
    return 0;
}