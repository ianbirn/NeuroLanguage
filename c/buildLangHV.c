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
	
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels
	
	
	
	for(int i=0; i<length; i++) {
		//Creating the file address

		char *ptr = langLabels[i];
		
		char fileAddress[] = "./training_texts/"; char txt[] = ".txt"; strcat(fileAddress, ptr); strcat(fileAddress, txt);
		printf("%s\n", fileAddress);
		
		FILE *fileID = fopen(fileAddress, "r"); 
		
		
		if (fileID == NULL) {
			printf("Failed: File could not be opened.\n");
			return 1;
		}
		
		int count=0;
		char buffer[count];
		
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
