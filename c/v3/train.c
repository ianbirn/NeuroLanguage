#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <wchar.h>
#include <errno.h>
#include "computations.h"

void createCache() {
    mkdir("cachedTraining/", 0777);
}
void createND(int N, int D, char cachedND[]) {
    mkdir(cachedND, 0777);
}
void loadiM(int D, int imSize, char cachedND[], wchar_t *itemMemory) {
	FILE *fileID;
	for(int i=0; i<imSize; i++) {
        wchar_t fiAd[32];
        char convertFile[35];
		int *randomHV;
		randomHV = (int*)malloc(D * sizeof(int));
			
		if (itemMemory[i] == L' ') {
			swprintf(fiAd, 32, L"%s%s%s", cachedND, "im__", ".txt");
		}
		else {
			swprintf(fiAd, 32, L"%s%s%lc%s", cachedND, "im_", itemMemory[i], ".txt");
		}
        wcstombs(convertFile, fiAd, 50);
			
		fileID = fopen(convertFile, "w");
		genRandomHV(D, randomHV);
		for(int j=0; j<D; j++) {
			fwprintf(fileID, L"%d ", randomHV[j]); 
		}
		
		fclose(fileID);
		free(randomHV);
	}	
}
void createIMHV(int D, int imSize, wchar_t *itemMemory, char cachedND[], int *imhv) {
    FILE *imf;

    for(int i=0; i<imSize; i++) {                                   //Loads the HVs from the character text files into an array (for speed purposes)
        wchar_t fiAd[32];
        char convertFile[35];
		if (itemMemory[i] == L' ') {
			swprintf(fiAd, 32, L"%s%s%s", cachedND, "im__", ".txt");
		}
		else {
			swprintf(fiAd, 32, L"%s%s%lc%s", cachedND, "im_", itemMemory[i], ".txt");
		}
        wcstombs(convertFile, fiAd, 50);

        imf = fopen(convertFile, "r");
        for(int j=0; j<D; j++) {
            int num=0;
            fwscanf(imf, L"%d", &num);
            *(imhv + i*D + j) = num;
        }
        fclose(imf);
    }
}
void buildLangHV(int N, int D, int length, char langLabels[][4], char cachedND[], int imSize, wchar_t *itemMemory, int *imhv) {
    FILE *f;
    for(int l=0; l<length; l++) {
        wchar_t *buffer = (wchar_t *)malloc(2500000000 * sizeof(wchar_t));
        int *sumHV = (int*)malloc(D * sizeof(int));
        char *fileAddress = (char*)malloc(25 * sizeof(char));
        sprintf(fileAddress, "%s%s%s", "../../training/", langLabels[l], ".txt");
        wprintf(L"Loading %s\n", fileAddress);
        f = fopen(fileAddress, "r");
        int n = 0;
        while (1) {
            buffer[n] = fgetwc(f);
            if (feof(f)) {
                buffer[n] = '\0';
                break;
            }
            n++;
        }
        fclose(f);
        free(fileAddress);
        

        computeSumHV(N, D, sumHV, buffer, n, itemMemory, imhv, imSize);

		char fiAd[36];                          //fileID for sumHV of lang
        snprintf(fiAd, 34, "%s%s%s%s", cachedND, "la_", langLabels[l], ".txt");
        f = fopen(fiAd, "w");
        for(int j=0; j<D; j++) {
			fwprintf(f, L"%d ", sumHV[j]); 
        }
		fclose(f);

        free(buffer);
        free(sumHV);
    }
}
void train(int N, int D, wchar_t *itemMemory, int imSize, int *iMHV, char langLabels[][4], int length) {
    DIR* cacheDir;
    DIR* ndDir;
    FILE *f;
	
    char cachedND[24];
    wchar_t fileAddress[32];
    char convertFile[35];
    
    snprintf(cachedND, 24, "%s%d%s%d%s", "cachedTraining/", D, "_", N, "/");        //Directory information for cache
    cacheDir = opendir("cachedTraining");
    if (cacheDir) {  
        ndDir = opendir(cachedND);  
        if (ndDir){
			for(int i=0; i<imSize; i++) {      		//Checks that all of the im files exist
				if (itemMemory[i] == L' ') {
					swprintf(fileAddress, 32, L"%s%s%s", cachedND, "im__", ".txt");
				}
				else {
					swprintf(fileAddress, 32, L"%s%s%lc%s", cachedND, "im_", itemMemory[i], ".txt");
				}
                wcstombs(convertFile, fileAddress, 50);
                
				f = fopen(convertFile, "r"); 
				if (f == NULL) {
					loadiM(D, imSize, cachedND, itemMemory);
					fclose(f);
					break;
				}
				fclose(f);

			}
            createIMHV(D, imSize, itemMemory, cachedND, iMHV);

			for(int i=0; i<length; i++) {
				snprintf(convertFile, 34, "%s%s%s%s", cachedND, "la_", langLabels[i], ".txt");
				f = fopen(convertFile, "r");
				if (f == NULL) {
					buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, iMHV);
                    break;
					fclose(f);
				}
				fclose(f);
			}

            closedir(ndDir);
        }
        else {
            createND(N, D, cachedND);  
            loadiM(D, imSize, cachedND, itemMemory);
            createIMHV(D, imSize, itemMemory, cachedND, iMHV);
            buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, iMHV);
        }
        closedir(cacheDir);
    }
    else if (ENOENT == errno) {
        createCache();
        createND(N, D, cachedND);
        loadiM(D, imSize, cachedND, itemMemory);
        createIMHV(D, imSize, itemMemory, cachedND, iMHV);
        buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, iMHV);
    }
}