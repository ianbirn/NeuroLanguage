/***********************************************************************
 *	GENERATING THE RANDOM HYPERVECTORS
 * 
 * 
 * 
 *	MATLAB-SOURCE: https://github.com/abbas-rahimi/HDC-Language-Recognition
 * 
 * 
 * 
 **********************************************************************/
 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Permutations
// Source:	https://gist.github.com/qiuwch/41860c0e4c02ec2dd357
int *perm(int D) {
	int *arr;
	arr = malloc(D*sizeof(int));
	
	//Initialize Array
	for(int i=0; i<D; i++) {
		arr[i] = i;
	}
	
	
	
	int k, l;
	for(int j=0; j<D; j++) {
		k = rand() % (D-j) + j;
		l = arr[k];
		arr[k] = arr[j];
		arr[j] = l; 
	}
	
	return arr;
}

int *genRandomHV(int D) {
	
	//Later will alter to be scanf (user input availability)
	int *randomHV;
	randomHV = malloc(D*sizeof(int));
	
	if (D % 2 != 0) {
		printf("Dimension is odd");
	}
	else {
		//Permute the Array
		randomHV = perm(D);
		
		for(int i=0; i<D; i++) {
			if (randomHV[i] <= D/2) {
				randomHV[i] = 1;
			}
			else {
				randomHV[i] = -1;
			}
		}
	}
	 
	return randomHV;
}
