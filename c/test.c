#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "computations.h"

void storeLangAM(int N, int D, int *langAM, int length, char langLabels[][4]) {
	char fileAddress[34];
	FILE *laf;

	char cachedND[24];
    snprintf(cachedND, 24, "%s%d%s%d%s", "cachedTraining/", D, "_", N, "/");        //Directory information for cache

	for(int i=0; i<length; i++) {
		snprintf(fileAddress, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".txt");
		laf = fopen(fileAddress, "r");
		if (laf == NULL) {
			fclose(laf);
			break;
		}
		for(int j=0; j<D; j++){
			fscanf(laf, "%i", &(*(langAM + i*D + j)));
		}
		fclose(laf);
	}
}
void binarizeHV(int *v, int szofv) {
	int threshold = 0;
	for( int i=0; i<szofv; i++) {
		if( v[i] > threshold ) 
			v[i] = 1;
		else 
			v[i] = -1;
	}
}
double norm(int *a, int n) {
	double sum=0.0;
	double norm=0.0;
	for (int i=0; i<n; i++) {
		sum += ((double)a[i] * (double)a[i]);
	}
	norm = sqrt(sum);

	return norm;
}
double dotProduct(int *a, int *b, int n) {
	double product = 0.0;
	for (int i=0; i<n; i++) {
		product += ((double)a[i] * (double)b[i]);
	}	
	return product;
}
double cosAngle(int *a, int *b, int n) {
	return dotProduct(a, b, n)/(norm(a, n)*norm(b, n));	
}
void test(int N, int D, int *langAM, int *imhv, char itemMemory[], int imSize) {
	DIR *dir;
	struct dirent *sd;
	FILE *fileID;

	int *testSumHV;
	testSumHV = (int*)malloc(D * sizeof(int));
	
	int *tmp;
	tmp = (int*)malloc(D * sizeof(int));
	
	char *buffer;
	buffer = (char*)malloc(300000 * sizeof(char));
	
	char fileLabel[][3] = {"af", "bg", "cs", "da", "nl", "de", "en", "et", "fi", "fr", "el", "hu", "it", "lv", "lt", "pl", "pt", "ro", "sk", "sl", "es", "sv"};
	int length = (sizeof fileLabel)/(sizeof fileLabel[0]);
	
	double angle = 0.0;
	char predicLang[3];
	double correct=0.0;
	double total = 0.0;
	double accuracy = 0.0;
	
	dir = opendir("../testing_texts");
	
    if (dir == NULL) {
        printf("Failed: Directory could not be openned.\n");
        exit(1);
    }
	
	while((sd=readdir(dir)) != NULL) {
		char fileAddress[273];
		double maxAngle = -1.0;
		
        if (!strcmp (sd->d_name, "."))
            continue;
        if (!strcmp (sd->d_name, ".."))    
            continue;
		
		
		snprintf(fileAddress, 273, "%s%s", "../testing_texts/", sd->d_name);
		fileID = fopen(fileAddress, "r"); 
		
        if (fileID == NULL) {
            printf("Failed: File could not be openned.\n");
            break;
        }

        int count=0;
		while(1) {
			buffer[count] = fgetc(fileID);
			if(feof(fileID)) {
				break;
			}
			count++;
		}
		buffer[count] = '\0';	//ending string, closing file

		fclose(fileID);
		printf("Loaded: %s\n", fileAddress);
		
		computeSumHV (N, D, testSumHV, count, buffer, imhv, itemMemory, imSize);
		binarizeHV(testSumHV, D);
		
		for(int l=0; l<length; l++) { //loop to go through the languages	
			for(int i=0; i<D; i++) {
				tmp[i] = *(langAM + l*D + i);
			}	

			angle = cosAngle(testSumHV, tmp, D);
			
			if (angle > maxAngle) {
				maxAngle = angle;
				snprintf(predicLang, 3, "%c%c", fileLabel[l][0], fileLabel[l][1]);
			}
		}
		
		//printf("%s ", predicLang);
		
		if ((predicLang[0] == (sd->d_name)[0]) && (predicLang[1] == (sd->d_name)[1])) {
			correct++;
		}
		total++;
		
	}
	closedir(dir);
	free(buffer);
	free(tmp);
	free(testSumHV);
	
    accuracy = correct/total;
    accuracy = accuracy * 100.0;
	printf("Accuracy: %.3f%%\n", accuracy);
}