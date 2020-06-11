#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void perm(int D, int arr[]); //permutates an array
void genRandomHV(int D, int randomHV[]); //generates random hypervector
void circShift(int n, int d, int arr[][d]); //shifts the array circularly
void createItemMemoryHV(int D, int iMHV[][D]); //creates hypervectors for every single character in itemMemory
void lookupItemMemory(int D, int iMHV[][D], char itemMemory[], char key, int block[][D]); // sets the first row of block to the cooresponding randHV of key
void computeSumHV(int D, int N, int sumHV[D], int count, char buffer[]);
void binarizeHV(int v[], int szofv);
double norm(double a[], int n);
double dotProduct(double a[], double b[], int n);
double cosAngle(double a[], double b[], int n);

int main() {
	int N = 4;
	int D = 100;
	
	char actualLabel[][3] = {"af", "bg", "cs", "da", "nl", "de", "en", "et", "fi", "fr", "el", "hu", "it", "lv", "lt", "pl", "pt", "ro", "sk", "sl", "es", "sv"};
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};

	int length = (sizeof actualLabel)/(sizeof actualLabel[0]);
	int total = 0;
	int correct = 0;
	char buffer[500];
	int testSumHV[D];
	double maxAngle = -1;
	double angle;
	double accuracy;
	char predicLang[4];
	
	for(int i=1; i<2; i++) {
		for(int j=0; j<3; j++) {
			char fileAddress[50];
			
			sprintf(fileAddress, "%s%s%s%d%s", "./code/testing_texts/", actualLabel[i], "_", j, "_p.txt");
			
			FILE *fileID = fopen(fileAddress, "r");
			if(fileID == NULL) {
				printf("Error opening file!\n");
				return 1;
			}
		
			int count = 0;
			while(1) {
				buffer[count] = fgetc(fileID);
				if(feof(fileID)) {
					break;
				}
				count++;
			}
			buffer[count] = '\0';
			fclose(fileID);
			printf("Loaded file: %s\n", fileAddress);
			printf("%s\n", buffer);
			
			computeSumHV (D, N, testSumHV, count, buffer);
			binarizeHV(testSumHV, D);
			
			for(int j=0; j<D; j++) {
				printf("%d ", testSumHV[j]);
			}
			printf("\n");
			
			for(int l=0; l<length; l++) {
				//angle = cosAngle();
				if (angle > maxAngle) {
					maxAngle = angle;
					sprintf(predicLang, "%s", langLabels[l]); //assigns predicLang to langLabels[l]
				}
			}
			/*if(predicLang == actualLabel[?]) {
				correct = correct +1;
			}
			else {
				total = total +1;
			} */	
		}	
	}
	
	accuracy = correct / total;
	
return 0;
}

double norm(double a[], int n) {
	double sum=0;
	double norm=0;
	for (int i=0; i<n; i++) {
		sum += a[i] * a[i];
	}
	sum = (double)sum;
	norm = sqrt(sum);
	return norm;
}

double dotProduct(double a[], double b[], int n) {
	double product = 0;
	for (int i=0; i<n; i++) {
		product += a[i] * b[i];
	}	
	return (double)product;
}

double cosAngle(double a[], double b[], int n) {
	return dotProduct(a, b, n)/(norm(a, n)*norm(b, n));	
}

void binarizeHV( int v[], int szofv) {
	int threshold = 0;
	for( int i=0; i<szofv; i++) {
		if( v[i] > threshold ) 
			v[i] = 1;
		else 
			v[i] = -1;
	}
}


void computeSumHV(int D, int N, int sumHV[D], int count, char buffer[]) {
	int block[N][D];
	char key;
	char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz "; //every character in the alphabet & ' '
	int iMHV[27][D];
	
	//Initializing block and sum to be arrays of 0
	for(int i=0; i<N; i++) {
		for(int j=0; j<D; j++) {
			block[i][j] = 0;
		}
	}
	for(int i=0; i<D; i++) {
		sumHV[i] = 0;
	}
	
	//assignItemMemory(count, buffer, itemMemory);
	createItemMemoryHV(D, iMHV);
	
	
	for(int i=0; i<=8; i++) {			
			
		key = buffer[i];
		circShift(N, D, block);
		lookupItemMemory(D, iMHV, itemMemory, key, block); 	
			
		if (i >= N) {
			//int *temp = block[0];
			int nGrams[D];
				
			//assigns nGrams to the first row of block
			for(int j=0; j<D; j++) {
				nGrams[j] = block[0][j];
			}		
				
			for(int j=1; j<N; j++) {
				for(int k=0; k<1; k++) {
					for(int l=0; l<D; l++) {
						nGrams[l] = nGrams[l] * block[j][l];
					}
				}
			}	
			for(int j=0; j<D; j++) {
				sumHV[j] = sumHV[j] + nGrams[j];
			}
		}
	}
}
void createItemMemoryHV(int D, int iMHV[][D]) {
	for(int i=0; i<27; i++) {
		int randomHV[D];
		genRandomHV(D, randomHV);
		for(int j=0; j<D; j++) {
			iMHV[i][j] = randomHV[j];
		}
	}
}

void genRandomHV(int D, int randomHV[]) {
	//Later will alter to be scanf (user input availability)
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

void perm(int D, int arr[]) {	
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
				//printf("%i ", arr[i][0]);   //#this was during testing
			}
			else {
				arr[i][j+1] = arr1[i][j];
			}
		}
	}
}

void lookupItemMemory(int D, int iMHV[][D], char itemMemory[], char key, int block[][D]) {
	for (int i=0; i<27; i++) {
		if (itemMemory[i] == key) {
			for(int j=0; j<D; j++) {
				block[0][j] = iMHV[i][j];
			}
			break;
		}
	}	
}
