#include <stdio.h>
#include <stdlib.h>
#include "train.h"
#include "computations.h"
#include "test.h"


int main() {
    int N = 4;
    int D = 10000;

    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";
    int imSize = sizeof(itemMemory);

	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels
    

    int *imhv = (int*)malloc(imSize * D * sizeof(int));
	int *langAM = (int*)malloc(length * D * sizeof(int));

	train(N, D, itemMemory, imSize, imhv, langLabels, length);
	storeLangAM(N, D, langAM, length, langLabels);
	test(N, D, langAM, imhv, itemMemory, imSize);


    free(imhv);
	free(langAM);

	return 0;
	
} 

