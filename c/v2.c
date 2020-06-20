#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

void perm(int D, int *arr); //permutates an array
void genRandomHV(int D, int *randomHV); //generates random hypervector
void loadiM(char imhv[], int D, char itemMemory[], int size);
void buildLangHV(int N, int D, int *langAM, int index, char imhv[]);
void computeSumHV(int N, int D, int *sumHV, int count, char *buffer, char imhv[]);
void circShift(int n, int d, int *arr);

int main() {
    int N = 4;
    int D = 10;
    
    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";
    int size = sizeof(itemMemory);

	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels


	FILE *fileID;
	FILE *LAiD;
    char fileAd[273];
    
    char AM[14]; //int AMcheck;
    char imhv[20]; int IMcheck;
    char laAM[22]; //int LAcheck;
   
    //Beginning training, checks if there is already stored memory for the N & D
    snprintf(AM, 14, "%s%d%s%d%s", "./", N, "_", D, "_AM/");
	snprintf(imhv, 29, "%s%s", AM, "/iMHV/");
    snprintf(laAM, 22, "%s%s", AM, "/langAM/");
    
    mkdir(AM, 0777);      //assign these to checks later?
    IMcheck = mkdir(imhv, 0777);
    mkdir(laAM, 0777);
    
	
    //Loads the hyper vectors for item memory
    if(IMcheck == -1) {
		for(int i=0; i<size; i++) {
			if (itemMemory[i] == ' ') {
				snprintf(fileAd, 273, "%s%s%s", imhv, "/_", ".txt");
			}
			else {
				snprintf(fileAd, 273, "%s%s%c%s", imhv, "/", itemMemory[i], ".txt");
			}
		
		
			fileID = fopen(fileAd, "r"); 
			
			if (fileID == NULL) {
				loadiM(imhv, D, itemMemory, size);
				fclose(fileID);
				break;
			}
		}
	}
	else {
		loadiM(imhv, D, itemMemory, size);
	}
	

    /**Creates the AM for each language (will be function soon)**/
	for(int i=0; i<1; i++) {
		char fiAd[273];
		
		int *langAM;
		langAM = (int*)malloc(D * sizeof(int));
		
		snprintf(fiAd, 273, "%s%s%s%s", laAM, "/", langLabels[i], ".txt");
		
		LAiD = fopen(fiAd, "w");
		buildLangHV(N, D, langAM, i, imhv);
		
		fclose(LAiD);
		//printf("Created: %s\n", fiAd);
		
		free(langAM);
		
	}
    
    
    
    
	return 0;
	
} 
void lookupItemMemory(int D, char key, int *block, char imhv[]) {
	
	DIR *dir;
	struct dirent *sd;
	FILE *fileID;
	
	char temp;
	if(key == ' ') {
		temp = key;
		key = '_';
	}
	
	dir = opendir(imhv);
	while((sd=readdir(dir)) != NULL) {
		char fileAddress[275];
        if (!strcmp (sd->d_name, "."))
            continue;
        if (!strcmp (sd->d_name, ".."))    
            continue;
            
        snprintf(fileAddress, 275, "%s%s", imhv, sd->d_name);
        //printf("%c\n", (sd->d_name)[0]);
        
        if((sd->d_name)[0] == key) {
			fileID = fopen(fileAddress, "r");
			/*
			for(int i=0; i<D; i++) {
				*(block + i) = (int)fgetc(fileID);
				printf("%i ", *(block + i));
			}*/
			fclose(fileID);
		}
		
	}
	closedir(dir);
	
	
	/*
	for (int i=0; i<27; i++) {
		if (itemMemory[i] == key) {
				for(int j=0; j<D; j++) {
					*(block + j) = iMHV[i][j];
			}
			break;
		}
	}*/

}
void computeSumHV(int N, int D, int *sumHV, int count, char *buffer, char imhv[]) { 
	
	int *block = (int *)malloc(N * D * sizeof(int)); 
	
	memset(block, 0, sizeof(*block) * D * N);
	memset(sumHV, 0, D);
	
	for(int i=0; i<1; i++) {
		char key = buffer[i];
		circShift(N, D, block);
		lookupItemMemory(D, key, block, imhv);
	}
	
	//char key;
	
	
	/*
	for(int i=0; i<count; i++) {
		key = buffer[i];
		circShift(N, D, block);
		lookupItemMemory(D, iMHV, itemMemory, key, block); 
		
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
	free(block);*/
}
void buildLangHV(int N, int D, int *langAM, int index, char imhv[]) {
	FILE *f;
	
	int *sumHV;
	sumHV = (int*)malloc(D * sizeof(int));
	memset(sumHV, 0, D);
	
	char *buffer;
	buffer = (char*)malloc(2000000 * sizeof(char));
	
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	
	char fileAddress[110];
	snprintf(fileAddress, 110, "%s%s%s", "../training_texts/", langLabels[index], ".txt");
	
	f = fopen(fileAddress, "r");
	
	if (f == NULL) {
		printf("Failed: File could not be opened.\n");
		exit(1);
	}
	
	int count=0;
	while(1) {
		buffer[count] = fgetc(f);
		if(feof(f)) {
			buffer[count] = '\0';
			break;
		}
		count++;	
	}
	
	fclose(f);
	printf("Loading %s\n", fileAddress);
	
	computeSumHV(N, D, sumHV, count, buffer, imhv);
	
	for(int i=0; i<D; i++) {
		langAM[i] = sumHV[i];
	}

	free(sumHV);
	free(buffer);
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
void loadiM(char imhv[], int D, char itemMemory[], int size) {
	FILE *iMID;
	for(int i=0; i<size; i++) {
		char fiAd[273];
		int *randomHV;
		randomHV = (int*)malloc(D * sizeof(int));
			
		if (itemMemory[i] == ' ') {
			snprintf(fiAd, 273, "%s%s%s", imhv, "/_", ".txt");
		}
		else {
			snprintf(fiAd, 273, "%s%s%c%s", imhv, "/", itemMemory[i], ".txt");
		}
			
		iMID = fopen(fiAd, "w");
		genRandomHV(D, randomHV);
		for(int j=0; j<D; j++) {
			fprintf(iMID, "%i", randomHV[j]); 
		}
		
		fclose(iMID);
		free(randomHV);
	}	
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
