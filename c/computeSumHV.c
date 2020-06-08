#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int *perm(int D); //permutates an array
int *genRandomHV(int D); //generates random hypervector
void circShift(int n, int d, int arr[][d]); //shifts the array circularly
void removeChar(char *s, int c); //removes every occurence of a specific character from a string
void assignItemMemory(int size, char buff[], char itemMemory[]); //assigns the 27 chracters into an array 
int **createItemMemoryHV(int D); //creates hypervectors for every single character in itemMemory
void lookupItemMemory(int D, char itemMemory[], char key, int block[][D]);

int main() {
	//Declarations/initializations for computeSumHV
	int N = 4;
	int D = 10;
	int sumHV[1][D];
	int block[N][D];
	char key;
	char itemMemory[27]; //every character in the alphabet & ' '
	
	//Creating langLabels
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	
	char buffer[2000000]; //array for every c in .txt files
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels
	
	//iterating through every file computingHV
	for(int t=0; t<1; t++) {
		//Creating the file address 
		char fileAddress[29];
		
		sprintf(fileAddress, "%s%s%s", "./code/training_texts/", langLabels[t], ".txt");
		
		
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
		
		
		/*************************************************
		 * 
		 * 
		 * function for computesum
		 * 
		 * 
		 * ************************************************/
		for(int i=0; i<N; i++) {
			for(int j=0; j<D; j++) {
				block[i][j] = 0;
			}
		}
		for(int i=0; i<1; i++) {
			for(int j=0; j<D; j++) {
				sumHV[i][j] = 0;
			}
		}
		
		assignItemMemory(sizeof(buffer), buffer, itemMemory);
		printf("%i\n", count);
		for(int i=0; i<count; i++) {			
			
			key = buffer[i];
			circShift(N, D, block);
			
			lookupItemMemory(D, itemMemory, key, block); 		
			
			if (i >= N) {
				int *temp = block[0];
				int nGrams[1][D];
				
				//assigns nGrams to the first row of block
				for(int j=0; j<1; j++) {
					for(int k=0; k<D; k++) {
						nGrams[j][k] = temp[k];
					}
				}
				
				
				for(int j=1; j<N; j++) {
					for(int k=0; k<1; k++) {
						for(int l=0; l<D; l++) {
							nGrams[k][l] = nGrams[k][l] * block[j][l];
						}
					}
				}	
				for(int row=0; row<1; row++) {
					for(int col=0; col<D; col++) {
						sumHV[row][col] = sumHV[row][col] + nGrams[row][col];
					}
				}
			}
		}
		
		for(int i=0; i<1; i++) {
			for(int j=0; j<D; j++) {
				printf("%i ", sumHV[i][j]);
			}
			printf("\n");
		}
		
    }
	
	return 0;
		
}

int *perm(int D) {
	int *arr;
	arr = malloc(D*sizeof *arr);
	
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
	
	return arr;
}

int *genRandomHV(int D) {
	//Later will alter to be scanf (user input availability)
	int *randomHV;
	randomHV = malloc(D*sizeof *randomHV);
	
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
void circShift(int n, int d, int arr[][d]) {
  int arr1[n][d];
  
  //Pushes every row down one
  for(int i=0; i<n; i++) {
    for(int j=0; j<d; j++) {
      if (i==0) {
        arr1[i][j] = arr[n-1][j];
      }
      else {
        arr1[i][j] = arr[i-1][j];
      }
    }
  }
  
  //Pushes every column to the right by 1
  for(int i=0; i<n; i++) {
    for(int j=0; j<d; j++) {
      if (j == (d-1)) {
        arr[i][0] = arr1[i][d-1];
        //printf("%i ", arr2[i][0]);   #this was during testing
      }
      else {
        arr[i][j+1] = arr1[i][j];
      }
    }
  }
}
void removeChar(char *str, int c) {
	int j, n = strlen(str);
	for(int i=j=0; i<n; i++) {
		if(str[i] != c) {
			str[j++] = str[i];
		}
	}
	str[j] = '\0';
	
}
void assignItemMemory(int size, char buff[], char itemMemory[]) {
	char temp[size];
	
	//so as to not delete content of original buffer
	for(int i=0; i<size; i++) {
		temp[i] = buff[i];
	}
	for(int i=0; i<=27; i++) {
		if ((temp[0] == '\n') || (temp[0] == '\t') || (temp[0] == '\r')) {
			removeChar(temp, '\n');
		}
		else {	
			itemMemory[i] = temp[0];
			removeChar(temp, temp[0]);
			//printf("%c", itemMemory[i]);
		}
	}
	//printf("\n");
}
int **createItemMemoryHV(int D) {
	int **randomHV;
	randomHV = malloc(sizeof(int*) * 27);
	
	for(int i = 0; i < 27; i++) {
		randomHV[i] = malloc(sizeof(int*) * D);
    }
	
	for(int i=0; i<27; i++) {
		for(int j=0; j<D; j++) {
			randomHV[i] = genRandomHV(D);
		}
	}
	
	return randomHV;
}

void lookupItemMemory(int D, char itemMemory[], char key, int block[][D]) {
	int **iMHV;
	iMHV = createItemMemoryHV(D);
	
	for (int i=0; i<27; i++) {
		if (itemMemory[i] == key) {
			int *temp = iMHV[i];
			for(int j=0; j<1; j++) {
				for(int k=0; k<D; k++) {
					block[j][k] = temp[k];
				}
			}
			break;
		}
	}	
}
