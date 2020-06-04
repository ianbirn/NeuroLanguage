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
		
		
		//Curently ignoring the issue when i=1
		if (i==1) {
			i++;
		}
		
		
		char *ptr = langLabels[i];
		
		//Creating the file address 
		char fileAddress[] = "./training_texts/"; char txt[] = ".txt"; strcat(fileAddress, ptr); strcat(fileAddress, txt);
		printf("%s\n", fileAddress);
		
		//Opening the file address
		FILE *fileID = fopen(fileAddress, "r"); 
		
		//Check to make sure the file can be opended
		if (fileID == NULL) {
			printf("Failed: File could not be opened.\n");
			return 1;
		}
		
		
		//Compiles every character in the text document into array, buffer
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
		buffer[0] = '\0';
		printf("Loaded training language file %s\n", fileAddress);
		
		
		

		
		
	}
	
	
	return 0;
}
