#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void removeChar(char *s, int c){ 
  
    int j, n = strlen(s); 
    for (int i=j=0; i<n; i++) 
       if (s[i] != c) 
          s[j++] = s[i]; 
      
    s[j] = '\0'; 
} 

int main() {
  FILE *fileID;
  FILE *langf;        //file ID for the lang .txt file

	char fileAddress[110];	
		char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "nor", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	int length = (sizeof langLabels)/(sizeof langLabels[0]);
	for(int i=0; i<length; i++) {
    char *buffer;
	  buffer = (char*)malloc(2000000 * sizeof(char));
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

	  int count=0; int y=0;
	  while(1) {
		  buffer[count] = fgetc(langf);
		  if(feof(langf)) {
		  	buffer[count] = '\0';
			  break;
		  }
	    count++;	
    }
	
	  fclose(langf);
	  
    snprintf(fileAddress, 110, "%s%s%s", "../trainingv2/", langLabels[i], ".bin");
    fileID = fopen(fileAddress, "wb");
    for(int j=0; j<count; j++) {
		fwrite(&buffer[j], sizeof(char), 1, fileID); 
    }
    fclose(fileID);
    free(buffer);
  }
    return 0;
}