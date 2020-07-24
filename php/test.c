#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <wchar.h>
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
		snprintf(fileAddress, 34, "%s%s%s%s", cachedND, "la_", langLabels[i], ".txt");
		laf = fopen(fileAddress, "rb");
		if (laf == NULL) {
			fclose(laf);
			break;
		}
		for(int j=0; j<D; j++){
            int num=0;
            fwscanf(laf, L"%d", (&num));
            *(langAM + i*D + j) = num;
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
void test(int N, int D, int *langAM, int *iMHV, wchar_t *itemMemory, int imSize, char actualLabels[][11], char langLabels[][4], int length, wchar_t *unknown) {
    int *testSumHV = (int*)malloc(D * sizeof(int));
    char *predicLang = (char*)malloc(3 * sizeof(char));

    storeLangAM(N, D, langAM, length, langLabels);
    int n = wcslen(unknown);
    computeSumHV(N, D, testSumHV, unknown, n, itemMemory, iMHV, imSize);
    binarizeHV(testSumHV, D);

    double maxAngle = -1.0;
    for(int l=0; l<length; l++) {
        double angle=0.0;
        int *tmp = (int*)malloc(D * sizeof(int));
            
        for(int i=0; i<D; i++) {
            tmp[i] = *(langAM + l*D + i);
        }

        angle = cosAngle(tmp, testSumHV, D);

        if (angle > maxAngle) {
            maxAngle = angle;
            sprintf(predicLang, "%s", actualLabels[l]);
        }
        free(tmp);
    }
    free(testSumHV);

    wprintf(L"%s\n", predicLang);  

    free(predicLang);
}
