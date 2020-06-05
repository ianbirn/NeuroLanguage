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
#include <string.h>


int main() {
	
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	char buffer[2000000];
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels
	
	
	for(int i=0; i<length; i++) {
		
		
		char *ptr = langLabels[i];
		
		//Creating the file address 
		char fileAddress[29];
		
		sprintf(fileAddress, "%s%s%s", "./code/training_texts/", langLabels[i], ".txt");
		
		
		//Opening the file address
		FILE *fileID = fopen(fileAddress, "r"); 
		
		//Check to make sure the file can be opended
		if (fileID == NULL) {
			printf("Failed: File could not be opened.\n");
			return 1;
		}
		
		
		//Compiles every character in the text document into array, buffer
		int count=0;
		
		while(1) {
			buffer[count] = fgetc(fileID);
			if(feof(fileID)) {
				break;
			}
			count++;	
		}
		fclose(fileID);
		printf("Loaded training language file %s\n", fileAddress);
		
	}
	
	
	return 0;
}
