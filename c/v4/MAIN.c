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

void cleanText(wchar_t *itemMemory, wchar_t *alphMemory, wchar_t *unknown, int imSize);

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
    wchar_t *alphMemory = L" ABCDEFGHIJKLMNOPQRSTUVWXYZĂÃĀÀÁĄÅÄÂÆßÇČĆĎÐÉĖĒÈÊĚËĘĢĞÎÏÍÌĮĪĶŁĻĽÑŃŇŉŅŌÔÓÒÕŐØÖŒÞŠȘŚŞŤŘŔÚŰŪŮŲÙÛÜŸÝỲŽŻАБВГҐДЕЁЖЗЭЄЇІИЙКЛМНОПРСТУФХЦЧШЩЪЬЫЮЯΑΒΓΔΈΕΖΉΗΘΊΙΚΛΜΝΞΌΟΠΡΣΤΥΦΧΨΏΩ";
    int imSize= wcslen(itemMemory);
/*****************************************************/


/*****************************************************/
/*Conversion of argv[2] to be compatable for testing*/
    const char *str = argv[2];                          //grabs agrv[2] to be the unknwon text
    int maxSize = 10000;                                //the max input size
    wchar_t *unknown = (wchar_t *)malloc(maxSize * sizeof(wchar_t));    //the wchar version of str
    mbstowcs(unknown, str, maxSize);                                    //converts str (multibyte c-string) to unknown (wchar)
    cleanText(itemMemory, alphMemory, unknown, imSize);
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

void cleanText(wchar_t *itemMemory, wchar_t *alphMemory, wchar_t *unknown, int imSize) {
    int unknownSize= wcslen(unknown);
    int check=0;
    for(int i=0; i<unknownSize; i++) {
        if (unknown[i] == L'-') {
            unknown[i] = ' ';
        }
        else if(unknown[i] == L'—') {
            unknown[i] = ' ';
        }
        else if(unknown[i] == L'–') {
            unknown[i] = ' ';
        }
        else if(unknown[i] == L'/') {
            unknown[i] = ' ';
        }     
        for(int j=0; j<imSize; j++) {
            if(unknown[i] == alphMemory[j]) {
                unknown[i] = itemMemory[j];
                break;
            }
        }
    }

    for(int i=0; i<unknownSize; i++) {
        for(int j=0; j<imSize; j++) {
            if(unknown[i] == itemMemory[j]) {
                check=0;
                break;
            }
            else {
                check=1;
            }
        }

        if(check==1) {
            wmemmove(&unknown[i], &unknown[i+1], unknownSize-i);
        }
    }   
}