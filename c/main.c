#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

void perm(int D, int arr[]); //permutates an array
void genRandomHV(int D, int randomHV[]); //generates random hypervector
void circShift(int n, int d, int arr[][d]); //shifts the array circularly
//void assignItemMemory(int count, char buff[], char iM[]); //creates equivalent item memory
void createItemMemoryHV(int D, int iMHV[][D], int size); //creates hypervectors for every single character in itemMemory
void lookupItemMemory(int D, int iMHV[][D], char itemMemory[], char key, int block[][D], int size); // sets the first row of block to the cooresponding randHV of key
void computeSumHV(int D, int N, int sumHV[D], int count, char buffer[], int iMHV[][D], char itemMemory[]); //performs the encoding on a text files
void buildLangHV(int N, int D, int langAM[][D], int iMHV[][D], char itemMemory[]); //loads and computes the training files
void binarizeHV(int v[], int szofv); //sets the testing HV to 1s and -1s for comparrison 
double norm(int a[], int n); //returns the norm of an array
double dotProduct(int a[], int b[], int n); //returns the dot product between two arrays
double cosAngle(int a[], int b[], int n) ; //measures distance between vectors
double test(int N, int D, int langAM[][D], int iMHV[][D], char itemMemory[]);

int main() {
	int N = 4;
	int D = 10000;
	
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	int length = (sizeof langLabels)/(sizeof langLabels[0]);
	char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";
	int langAM[length][D];
	double accuracy;
	
	//assignItemMemory(count, buffer, itemMemory);

	int iMHV[27][D];
	createItemMemoryHV(D, iMHV, 27);

	buildLangHV(N, D, langAM, iMHV, itemMemory);
	
	accuracy = test(N, D, langAM, iMHV, itemMemory);
	printf("Accuracy = %.3f%\n", accuracy);
	   
	return 0;
}
double test(int N, int D, int langAM[][D], int iMHV[][D], char itemMemory[]) {
	DIR *dir;
	struct dirent *sd;
	FILE *fileID;
	
	//double accuracy;
	char fileLabel[][3] = {"af", "bg", "cs", "da", "nl", "de", "en", "et", "fi", "fr", "el", "hu", "it", "lv", "lt", "pl", "pt", "ro", "sk", "sl", "es", "sv"};
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	int length = (sizeof fileLabel)/(sizeof fileLabel[0]);
	
	char predicLang[3];
	char actualLang[3];
	
	int tmp[D];
	int testSumHV[D];
	int correct = 0;
	int total = 0;
	double accuracy;
	double angle;
	char buffer[300000];

    
    
    dir = opendir("./code/testing_texts");
	
    if (dir == NULL) {
        printf("Failed: Directory could not be openned.\n");
        exit(1);
    }
	
    while((sd=readdir(dir)) != NULL) {
        char fileAddress[50];
        double maxAngle = -1.0;
        
        //To ignore parent directory print
        if (!strcmp (sd->d_name, "."))
            continue;
        if (!strcmp (sd->d_name, ".."))    
            continue;
        
        sprintf(fileAddress, "%s%s", "./code/testing_texts/", sd->d_name);
        
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
        
		computeSumHV (D, N, testSumHV, count, buffer, iMHV, itemMemory);
		binarizeHV(testSumHV, D);
		
		for(int l=0; l<length; l++) { //loop to go through the languages	
			for(int i=0; i<D; i++) {
				tmp[i] = langAM[l][i];
			}	
			
			angle = cosAngle(testSumHV, tmp, D);
			//printf("%i ", angle);
			
			if (angle > maxAngle) {
				maxAngle = angle;
				sprintf(predicLang, "%s", fileLabel[l]);
			}
		}
		//printf("\n");
		
		//printf("%s\n", predicLang);*/
		
		if ((predicLang[0] == (sd->d_name)[0]) && (predicLang[1] == (sd->d_name)[1])) {
			//printf("Correct\n");
			correct++;
		
		}
		total++;
    }
    closedir(dir);
    accuracy = (double)correct/(double)total;
    accuracy = accuracy * 100.0;
	
	return accuracy;
}


double norm(int a[], int n) {
	double sum=0.0;
	double norm=0.0;
	for (int i=0; i<n; i++) {
		sum += ((double)a[i] * (double)a[i]);
	}
	norm = sqrt(sum);

	return sum;
}

double dotProduct(int a[], int b[], int n) {
	double product = 0.0;
	for (int i=0; i<n; i++) {
		product += ((double)a[i] * (double)b[i]);
	}	
	return product;
}
//Change
double cosAngle(int a[], int b[], int n) {
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


void computeSumHV(int D, int N, int sumHV[D], int count, char buffer[], int iMHV[][D], char itemMemory[]) { 
	int block[N][D];
	char key;
	//int size = strlen(itemMemory);

	
	//Initializing block and sum to be arrays of 0
	for(int i=0; i<N; i++) {
		for(int j=0; j<D; j++) {
			block[i][j] = 0;
		}
	}
	for(int i=0; i<D; i++) {
		sumHV[i] = 0;
	}
	
	//itemMemory[0] = buffer[0];
	for(int i=0; i<count; i++) {			
			
		key = buffer[i];
		circShift(N, D, block);
		
		lookupItemMemory(D, iMHV, itemMemory, key, block, 27); 	
			
		if (i >= N) {
			int nGrams[D];
				
			//assigns nGrams to the first row of block
			for(int j=0; j<D; j++) {
				nGrams[j] = block[0][j];
			}		
				
			for(int j=1; j<N; j++) {
				for(int l=0; l<D; l++) {
						nGrams[l] = nGrams[l] * block[j][l];
				}
			}	
			for(int j=0; j<D; j++) {
				sumHV[j] = sumHV[j] + nGrams[j];
			}
		}
	}
}
void removeChar(char *s, int c) {
	int j, n = strlen(s);
	for(int i=j=0; i<n; i++) {
		if(s[i] != c) {
			s[j++] = s[i];
		}
	}
	s[j] = '\0';
	
}
void lookupItemMemory(int D, int iMHV[][D], char itemMemory[], char key, int block[][D], int size) {
	for (int i=0; i<size; i++) {
		if (itemMemory[i] == key) {
			for(int j=0; j<D; j++) {
				block[0][j] = iMHV[i][j];
			}
			break;
		}
	}	
}
void assignItemMemory(int count, char buff[], char iM[]) {
	char temp[count];
	int k=0;
	
	//so as to not delete content of original buffer
	for(int i=0; i<count; i++) {
		temp[i] = buff[i];
	}
	
	int i=0;
	int j=0;
	if (count > 300000) {
		for(i=0; i<27; i++) {
			if((temp[0] == '\n') || (temp[0] == '\t') || (temp[0] == '\r')) {
				removeChar(temp, temp[0]);
			}
			else {
				iM[i] = temp[0];
				removeChar(temp, temp[0]);
			}	
		}
		iM[i] = '\0';
	}
	else {
		for(j=0; j<strlen(temp); j++) {
			if((temp[0] == '\n') || (temp[0] == '\t') || (temp[0] == '\r')) {
				removeChar(temp, temp[0]);
			}
			else {
				iM[j] = temp[0];
				removeChar(temp, temp[0]);	
			}
		}
		iM[j] = '\0';
	}	
	//printf("\n");
	//printf("%s\n", iM);
}
void createItemMemoryHV(int D, int iMHV[][D], int size) {
	for(int i=0; i<size; i++) {
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
void buildLangHV(int N, int D, int langAM[][D], int iMHV[][D], char itemMemory[]) {
	int sumHV[D];
	
	//Creating langLabels
	char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels
	char buffer[2000000];
	//iterating through every file computingHV
	for(int t=0; t<length; t++) {
		//Creating the file address 
		char fileAddress[29];

		sprintf(fileAddress, "%s%s%s", "./code/training_texts/", langLabels[t], ".txt");
		
		
		//Opening the file address
		FILE *fileID = fopen(fileAddress, "r"); 
		
		//Check to make sure the file can be opended
		if (fileID == NULL) {
			printf("Failed: File could not be opened.\n");
			break;
		}
		
		
		//Compiles every character in the text document into array, buffer
		int count=0;
		while(1) {
			buffer[count] = fgetc(fileID);
			if(feof(fileID)) {
				buffer[count] = '\0';
				break;
			}
			count++;	
		}
		fclose(fileID);
		//printf("%s\n", buffer);
		printf("Loaded training language file %s\n", fileAddress);

		computeSumHV(D, N, sumHV, count, buffer, iMHV, itemMemory);
		 
		for(int i=0; i<D; i++) {
			langAM[t][i] = sumHV[i];
			//printf("%i ", langAM[t][i]);
		}
		//printf("\n");
	}
}
