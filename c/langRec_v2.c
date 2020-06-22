#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>

void createCache();
void createND(int N, int D, char cachedND[]);
void loadiM(int D, int imSize, char cachedND[], char itemMemory[]);
void perm(int D, int *arr);
void genRandomHV(int D, int *randomHV);
void buildLangHV(int N, int D, int length, char langLabels[][4], char cachedND[], int imSize, char itemMemory[], int *imhv);
void createIMHV(int D, int imSize, char itemMemory[], char cachedND[], int *imhv);
void computeSumHV(int N, int D, int *sumHV, int count, char *buffer, int *imhv, char itemMemory[], int imSize);
void circShift(int n, int d, int *arr);
void lookupItemMemory(int D, int *imhv, char itemMemory[], char key, int *block, int imSize);

int main() {
    int N = 4;
    int D = 10;

    char cachedND[24];

    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";
    int imSize = sizeof(itemMemory);

	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels

    char fileAddress[34];

    int *imhv = (int*)malloc(imSize * D * sizeof(int));

    DIR* cacheDir;
    DIR* ndDir;
    FILE *fileID;

    snprintf(cachedND, 24, "%s%d%s%d%s", "cachedTraining/", D, "_", N, "/");        //Directory information for cache

    cacheDir = opendir("cachedTraining");
    if (cacheDir) {                     //Checks that the cacheDir already exists
        ndDir = opendir(cachedND);  

        if (ndDir){
            closedir(ndDir);
        }

        if (ENOENT == errno) {                     //Checks that ndDir already exists (i.e. that if the cacheDir does not include memory for certain N&D, creates it)
            createND(N, D, cachedND);                  //Checks if the directory is empty/missing files
        }

        for(int i=0; i<imSize; i++) {               //Checks that all of the im files exist
            if (itemMemory[i] == ' ') {
                snprintf(fileAddress, 34, "%s%s", cachedND, "im__.txt");
            }
            else {
                snprintf(fileAddress, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".txt");
            }
            
            fileID = fopen(fileAddress, "r"); 

			if (fileID == NULL) {
				loadiM(D, imSize, cachedND, itemMemory);
				fclose(fileID);
				break;
            }
            fclose(fileID);
        }

        createIMHV(D, imSize, itemMemory, cachedND, imhv);

        for(int i=0; i<length; i++) {
            snprintf(fileAddress, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".txt");
            
            fileID = fopen(fileAddress, "r"); 
			if (fileID == NULL) {
				buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, imhv);
				fclose(fileID);
				break;
            }
            fclose(fileID);
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

    free(imhv);
	return 0;
	
} 
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
            snprintf(fiAd, 34, "%s%s", cachedND, "im__.txt");
        }
        else {
            snprintf(fiAd, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".txt");
        }
			
		fileID = fopen(fiAd, "w");
		genRandomHV(D, randomHV);
		for(int j=0; j<D; j++) {
			fprintf(fileID, "%i ", randomHV[j]); 
		}
		
		fclose(fileID);
		free(randomHV);
	}	
}
void perm(int D, int *arr) {	
	//Initialize Array
	for(int i=0; i<D; i++) {
		arr[i] = i;
	}
	
	//Randomize the numbers of the array
	int k, l;
	for(int j=0; j<D; j++) {
		k = rand() % (D-j) + j;
		l = arr[k];
		arr[k] = arr[j];
		arr[j] = l; 
	}
}
void genRandomHV(int D, int *randomHV) {
	if (D % 2 != 0) {
		printf("Dimension is odd");
	}
	else {
		//Permute the Array
		perm(D, randomHV);
		
		for(int i=0; i<D; i++) {
			if (randomHV[i] <= D/2) {
				randomHV[i] = 1;
			}
			else {
				randomHV[i] = -1;
			}
		}
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

        snprintf(fileAddress, 110, "%s%s%s", "../training_texts/", langLabels[i], ".txt");

        langf = fopen(fileAddress, "r");

	    if (langf == NULL) {
		    printf("Failed: File could not be opened.\n");
		    exit(1);
	    }

	    int count=0;
	    while(1) {
		    buffer[count] = fgetc(langf);
		    if(feof(langf)) {
			    buffer[count] = '\0';
			    break;
		    }
		    count++;	
	    }
	
	    fclose(langf);
	    printf("Loading %s\n", fileAddress);

        computeSumHV(N, D, sumHV, count, buffer, imhv, itemMemory, imSize);

		char fiAd[34];                          //fileID for sumHV of lang
        snprintf(fiAd, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".txt");	
		fileID = fopen(fiAd, "w");

        for(int j=0; j<D; j++) {
            fprintf(fileID, "%i ", sumHV[j]);
        }

		fclose(fileID);
        free(sumHV);
        free(buffer);
	}
}
void createIMHV(int D, int imSize, char itemMemory[], char cachedND[], int *imhv) {
    FILE *imf;
    char imAddress[34];

    for(int i=0; i<imSize; i++) {                                   //Loads the HVs from the character text files into an array (for speed purposes)
        if (itemMemory[i] == ' ') {
            snprintf(imAddress, 34, "%s%s", cachedND, "im__.txt");
        }
        else {
            snprintf(imAddress, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".txt");
        }

        imf = fopen(imAddress, "r");
        for(int j=0; j<D; j++) {
            int num=0;
            fscanf(imf, "%i", &num);
            *(imhv + i*D + j) = num;
        }
        fclose(imf);
    }
}
void computeSumHV(int N, int D, int *sumHV, int count, char *buffer, int *imhv, char itemMemory[], int imSize) { 
	int *block = (int *)malloc(N * D * sizeof(int)); 
	
	char key;
	
	for(int i=0; i<N; i++) {
		for(int j=0; j<D; j++) {
			*(block + i*D + j) = 0;
		}
	}
	
	for(int i=0; i<D; i++) {
		*(sumHV +i) = 0;
	}
	
	for(int i=0; i<count; i++) {
		key = buffer[i];
		circShift(N, D, block);
        
		lookupItemMemory(D, imhv, itemMemory, key, block, imSize); 
		
		if (i >= N) {
			int *nGrams;
			nGrams = (int*)malloc(D * sizeof(int));
			
			//assigns nGrams to the first row of block
			for(int j=0; j<D; j++) {
				nGrams[j] = *(block + j);
			}	
			
			for(int j=1; j<N; j++) {
				for(int l=0; l<D; l++) {
					nGrams[l] = nGrams[l] * (*(block + j*D + l));
				}
			}
			
			for(int j=0; j<D; j++) {
				sumHV[j] = sumHV[j] + nGrams[j];
			}
			free(nGrams);
		}

	}
	free(block);
}
void circShift(int n, int d, int *arr) {	
    int *arr1 = (int *)malloc(n * d * sizeof(int)); 

	//format: *(arr + i*col + j));

	for(int i=0; i<1; i++) {
		for(int j=0; j<d; j++) {
			*(arr1 + i*d + j) = *(arr + (n-1)*d + j);
		}
	}
	
	for(int i=1; i<n; i++) {
		for(int j=0; j<d; j++) {
			*(arr1 + i*d + j) = *(arr + (i-1)*d + j);
		}
	}
	

	for(int i=0; i<n; i++) {
		for(int j=0; j<d-1; j++){
			*(arr + i*d + (j+1)) = *(arr1 + i*d + j);
		}
		
	}
	
	for(int i=0; i<n; i++) {
		for(int j=d-1; j<d; j++) {
			*(arr + i*d) = *(arr1 + i*d + (d-1));
		}
	}

	free(arr1);
}
void lookupItemMemory(int D, int *imhv, char itemMemory[], char key, int *block, int imSize) {
	for (int i=0; i<imSize; i++) {
		if (itemMemory[i] == key) {
				for(int j=0; j<D; j++) {
                    int num = *(imhv + i*D + j);
					*(block + j) = num;
			}
			break;
		}
	}

}