#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include "computations.h"

void createCache() {
    mkdir("cachedTraining/", 0777);
}
void createND(int N, int D, char cachedND[]) {
    mkdir(cachedND, 0777);
}
void loadiM(int D, int imSize, char cachedND[], char itemMemory[]) {
	FILE *fileID;
	for(int i=0; i<imSize; i++) {
		char fiAd[34];
		int *randomHV;
		randomHV = (int*)malloc(D * sizeof(int));
			
        if (itemMemory[i] == ' ') {
            snprintf(fiAd, 34, "%s%s", cachedND, "im__.bin");
        }
        else {
            snprintf(fiAd, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".bin");
        }
			
		fileID = fopen(fiAd, "wb");
		genRandomHV(D, randomHV);
		for(int j=0; j<D; j++) {
			fwrite(&randomHV[j], sizeof(int), 1, fileID); 
		}
		
		fclose(fileID);
		free(randomHV);
	}	
}
void createIMHV(int D, int imSize, char itemMemory[], char cachedND[], int *imhv) {
    FILE *imf;
    char imAddress[34];

    for(int i=0; i<imSize; i++) {                                   //Loads the HVs from the character text files into an array (for speed purposes)
        if (itemMemory[i] == ' ') {
            snprintf(imAddress, 34, "%s%s", cachedND, "im__.bin");
        }
        else {
            snprintf(imAddress, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".bin");
        }

        imf = fopen(imAddress, "rb");
        for(int j=0; j<D; j++) {
            int num=0;
			fread(&num, sizeof(int), 1, imf); 
            *(imhv + i*D + j) = num;
        }
        fclose(imf);
    }
}
void buildLangHV(int N, int D, int length, char langLabels[][4], char cachedND[], int imSize, char itemMemory[], int *imhv) {
	FILE *fileID;
    FILE *langf;        //file ID for the lang .txt file

	char fileAddress[110];	

	for(int i=0; i<length; i++) {
        char *buffer;
	    buffer = (char*)malloc(2000000 * sizeof(char));

        int *sumHV;
        sumHV = (int*)malloc(D * sizeof(int));

	    if(buffer == NULL) {
		    printf("Not enough memory!\n");
		    exit(1);
	    }

        snprintf(fileAddress, 110, "%s%s%s", "../../trainingv2/", langLabels[i], ".bin");

        langf = fopen(fileAddress, "rb");

	    if (langf == NULL) {
		    printf("Failed: File could not be opened.\n");
		    exit(1);
	    }

	    int count=0;
	    while(1) {
			fread(&buffer[count], sizeof(char), 1, langf); 
		    if(feof(langf)) {
			    buffer[count] = '\0';
			    break;
		    }
		    count++;	
	    }
		removeChar(buffer, '\n');
	
	    fclose(langf);
	    printf("Loading %s\n", fileAddress);
        computeSumHV(N, D, sumHV, count, buffer, imhv, itemMemory, imSize);

		char fiAd[34];                          //fileID for sumHV of lang
        snprintf(fiAd, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".bin");	
		fileID = fopen(fiAd, "wb");

        for(int j=0; j<D; j++) {
			fwrite(&sumHV[j], sizeof(int), 1, fileID); 
        }

		fclose(fileID);
        free(sumHV);
        free(buffer);
	}
}
void train(int N, int D, char itemMemory[], int imSize, int *imhv, char langLabels[][4], int length) {
	
    DIR* cacheDir;
    DIR* ndDir;
    FILE *fileID;
	

	char fileAddress[34];
	char cachedND[24];

    snprintf(cachedND, 24, "%s%d%s%d%s", "cachedTraining/", D, "_", N, "/");        //Directory information for cache
    cacheDir = opendir("cachedTraining");
    if (cacheDir) {                     //Checks that the cacheDir already exists
        ndDir = opendir(cachedND);  
        if (ndDir){
			for(int i=0; i<imSize; i++) {      		//Checks that all of the im files exist
				if (itemMemory[i] == ' ') {
					snprintf(fileAddress, 34, "%s%s", cachedND, "im__.bin");
				}
				else {
					snprintf(fileAddress, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".bin");
				}

				fileID = fopen(fileAddress, "rb"); 
				if (fileID == NULL) {
					loadiM(D, imSize, cachedND, itemMemory);
					fclose(fileID);
					break;
				}

				fclose(fileID);
			}

			createIMHV(D, imSize, itemMemory, cachedND, imhv);

			for(int i=0; i<length; i++) {
				snprintf(fileAddress, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".bin");
				fileID = fopen(fileAddress, "rb");
				if (fileID == NULL) {
					buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, imhv);
					fclose(fileID);
					break;
				}
				fclose(fileID);
			}
            closedir(ndDir);
        }

        else if (ENOENT == errno) {                     //Checks that ndDir already exists (i.e. that if the cacheDir does not include memory for certain N&D, creates it)
            createND(N, D, cachedND);                  //Checks if the directory is empty/missing files
			loadiM(D, imSize, cachedND, itemMemory);
			createIMHV(D, imSize, itemMemory, cachedND, imhv);
			buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, imhv);
        }

        closedir(cacheDir);
    }
    else if (ENOENT == errno) {
        createCache();
        createND(N, D, cachedND);
        loadiM(D, imSize, cachedND, itemMemory);
		createIMHV(D, imSize, itemMemory, cachedND, imhv);
        buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, imhv);
    }
	
}