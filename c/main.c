#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

void perm(int D, int *arr);
void genRandomHV(int D, int *randomHV);
void circShift(int n, int d, int *arr);
void lookupItemMemory(int D, int *imhv, char itemMemory[], char key, int *block, int imSize);
void computeSumHV(int N, int D, int *sumHV, int count, char *buffer, int *imhv, char itemMemory[], int imSize);
int storeLangAMwithCheck(int N, int D, int *langAM, int length, char langLabels[][4], int imSize, char itemMemory[], int *imhv); //checks to make sure all files are there before testing
void storeLangAM(int N, int D, int *langAM, int length, char langLabels[][4]);
double norm(int *a, int n);
double dotProduct(int *a, int *b, int n);
double cosAngle(int *a, int *b, int n);
double test(int N, int D, int *langAM, int *imhv, char itemMemory[], int imSize);
void createCache();
void createND(int N, int D, char cachedND[]);
void loadiM(int D, int imSize, char cachedND[], char itemMemory[]);
void createIMHV(int D, int imSize, char itemMemory[], char cachedND[], int *imhv);
void buildLangHV(int N, int D, int length, char langLabels[][4], char cachedND[], int imSize, char itemMemory[], int *imhv);
void train(int N, int D, char itemMemory[], int imSize, int *imhv, char langLabels[][4], int length);
void comb(char *tmp);
int personalTest(int N, int D, int *langAM, int *imhv, char itemMemory[], int imSize, char fileAddress[]); //tests a single file
double program(int N, int D);	//runs the entire program with all the testing files, returns percentage
int sentence(int N, int D);	//tests the document linked with the python code
int training(int N, int D);	//runs the train function
double percentage(int N, int D, int *langAM, int *imhv, char itemMemory[], int imSize, char fileAddress[]);
double sentencePercentage(int N, int D);

int main() {
   
    int N = 3;
    int D = 5000;
    
    scanf("%d %d", &N, &D);
    
    //printf("My guess is: %d %.2f%% match\n", sentence(N, D), sentencePercentage(N, D));
    printf("%d\n", training(N, D));
    
   // printf("N: %d\nD: %d\n", N, D);
    
    return 0;
} 

void perm(int D, int *arr) {	
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
void genRandomHV(int D, int *randomHV) {
	if (D % 2 != 0) {
		printf("Dimension is odd\n");
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
void circShift(int n, int d, int *arr) {	
    int *arr1 = (int *)malloc(n * d * sizeof(int)); 

	//format: *(arr + i*col + j));

	for(int i=0; i<1; i++) {
		for(int j=0; j<d; j++) {
			*(arr1 + i*d + j) = *(arr + (n-1)*d + j);
		}
	}
	
	for(int i=1; i<n; i++) {
		for(int j=0; j<d; j++) {
			*(arr1 + i*d + j) = *(arr + (i-1)*d + j);
		}
	}
	

	for(int i=0; i<n; i++) {
		for(int j=0; j<d-1; j++){
			*(arr + i*d + (j+1)) = *(arr1 + i*d + j);
		}
		
	}
	
	for(int i=0; i<n; i++) {
		for(int j=d-1; j<d; j++) {
			*(arr + i*d) = *(arr1 + i*d + (d-1));
		}
	}

	free(arr1);
}
void lookupItemMemory(int D, int *imhv, char itemMemory[], char key, int *block, int imSize) {
	for (int i=0; i<imSize; i++) {
		if (itemMemory[i] == key) {
				for(int j=0; j<D; j++) {
                    int num = *(imhv + i*D + j);
					*(block + j) = num;
			}
			break;
		}
	}

}
void computeSumHV(int N, int D, int *sumHV, int count, char *buffer, int *imhv, char itemMemory[], int imSize) { 
	int *block = (int *)malloc(N * D * sizeof(int)); 
	
	char key;
	
	for(int i=0; i<N; i++) {
		for(int j=0; j<D; j++) {
			*(block + i*D + j) = 0;
		}
	}
	
	for(int i=0; i<D; i++) {
		*(sumHV +i) = 0;
	}
	
	for(int i=0; i<count; i++) {
		key = buffer[i];
		circShift(N, D, block);
        
		lookupItemMemory(D, imhv, itemMemory, key, block, imSize); 
		
		if (i >= N) {
			int *nGrams;
			nGrams = (int*)malloc(D * sizeof(int));
			
			//assigns nGrams to the first row of block
			for(int j=0; j<D; j++) {
				nGrams[j] = *(block + j);
			}	
			
			for(int j=1; j<N; j++) {
				for(int l=0; l<D; l++) {
					nGrams[l] = nGrams[l] * (*(block + j*D + l));
				}
			}
			
			for(int j=0; j<D; j++) {
				sumHV[j] = sumHV[j] + nGrams[j];
			}
			free(nGrams);
		}

	}
	free(block);
}
int storeLangAMwithCheck(int N, int D, int *langAM, int length, char langLabels[][4], int imSize, char itemMemory[], int *imhv) {
	
    DIR* cacheDir;
    DIR* ndDir;
    FILE *fileID;
    char fileAddress[34];
    char cachedND[24];

    snprintf(cachedND, 24, "%s%d%s%d%s", "cachedTraining/", D, "_", N, "/");        //Directory information for cache
    cacheDir = opendir("cachedTraining");
    if (cacheDir) {                     //Checks that the cacheDir already exists
        ndDir = opendir(cachedND);  
        if (ndDir){
		for(int i=0; i<imSize; i++) {      		//Checks that all of the im files exist
			if (itemMemory[i] == ' ') {
				snprintf(fileAddress, 34, "%s%s", cachedND, "im__.txt");
			}
			else {
				snprintf(fileAddress, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".txt");
			}

			fileID = fopen(fileAddress, "r"); 
			if (fileID == NULL) {
				printf("Error: no file called %s", fileAddress);
				fclose(fileID);
				return -1;
			}
			
			fclose(fileID);
		}
		createIMHV(D, imSize, itemMemory, cachedND, imhv);
			
		for(int i=0; i<length; i++) {
			snprintf(fileAddress, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".txt");
			fileID = fopen(fileAddress, "r");
			if (fileID == NULL) {
				printf("Error: no file called %s", fileAddress);
				fclose(fileID);
				return -1;
			}
			fclose(fileID);
		}
		closedir(ndDir);
        }

        else if (ENOENT == errno) {                     //Checks that ndDir already exists (i.e. that if the cacheDir does not include memory for certain N&D, creates it)
			//Checks if the directory is empty/missing files
	    printf("Please train that setting first!\n");
	    return -1;
        }

        closedir(cacheDir);
    }
    else if (ENOENT == errno) {
        printf("Please train a setting first!\n");
	return -2;
    }
    ////continue storelangAM
	char fileAddress2[34];
	FILE *laf;

	char cachedND2[24];
	snprintf(cachedND2, 24, "%s%d%s%d%s", "cachedTraining/", D, "_", N, "/");        //Directory information for cache

	for(int i=0; i<length; i++) {
		snprintf(fileAddress2, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".txt");
		laf = fopen(fileAddress2, "r");
		if (laf == NULL) {
			fclose(laf);
			break;
		}
		for(int j=0; j<D; j++){
			fscanf(laf, "%i", &(*(langAM + i*D + j)));
		}
		fclose(laf);
	}
	return 1;
}
void storeLangAM(int N, int D, int *langAM, int length, char langLabels[][4]) {
	char fileAddress[34];
	FILE *laf;

	char cachedND[24];
	snprintf(cachedND, 24, "%s%d%s%d%s", "cachedTraining/", D, "_", N, "/");        //Directory information for cache

	for(int i=0; i<length; i++) {
		snprintf(fileAddress, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".txt");
		laf = fopen(fileAddress, "r");
		if (laf == NULL) {
			fclose(laf);
			break;
		}
		for(int j=0; j<D; j++){
			fscanf(laf, "%i", &(*(langAM + i*D + j)));
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
double test(int N, int D, int *langAM, int *imhv, char itemMemory[], int imSize) {
	DIR *dir;
	struct dirent *sd;
	FILE *fileID;

	int *testSumHV;
	testSumHV = (int*)malloc(D * sizeof(int));
	
	int *tmp;
	tmp = (int*)malloc(D * sizeof(int));
	
	char *buffer;
	buffer = (char*)malloc(300000 * sizeof(char));
	
	char fileLabel[][3] = {"af", "bg", "cs", "da", "nl", "de", "en", "et", "fi", "fr", "el", "hu", "it", "lv", "lt", "pl", "pt", "ro", "sk", "sl", "es", "sv"};
	int length = (sizeof fileLabel)/(sizeof fileLabel[0]);
	
	double angle = 0.0;
	char predicLang[3];
	double correct=0.0;
	double total = 0.0;
	double accuracy = 0.0;
	
	dir = opendir("/home/pi/Downloads/testing_texts");
	
    if (dir == NULL) {
        printf("Failed: Directory could not be openned.\n");
        exit(1);
    }
	
	while((sd=readdir(dir)) != NULL) {
		char fileAddress[300];
		double maxAngle = -1.0;
		
        if (!strcmp (sd->d_name, "."))
            continue;
        if (!strcmp (sd->d_name, ".."))    
            continue;
		
		
		snprintf(fileAddress, 300, "%s%s", "testing_texts/", sd->d_name);
		fileID = fopen(fileAddress, "r"); 
		
        if (fileID == NULL) {
            printf("Failed: %s could not be opened.\n", fileAddress);
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
		
		computeSumHV (N, D, testSumHV, count, buffer, imhv, itemMemory, imSize);
		binarizeHV(testSumHV, D);
		
		for(int l=0; l<length; l++) { //loop to go through the languages	
			for(int i=0; i<D; i++) {
				tmp[i] = *(langAM + l*D + i);
			}	

			angle = cosAngle(testSumHV, tmp, D);
			
			
			if (angle > maxAngle) {
				maxAngle = angle;
				snprintf(predicLang, 3, "%c%c", fileLabel[l][0], fileLabel[l][1]);
			}
		}
		
		//printf("%s ", predicLang);
		
		if ((predicLang[0] == (sd->d_name)[0]) && (predicLang[1] == (sd->d_name)[1])) {
			correct++;
		}
		total++;
		
	}
	closedir(dir);
	free(buffer);
	free(tmp);
	free(testSumHV);
	
    accuracy = correct/total;
    accuracy = accuracy * 100.0;
	//printf("Accuracy: %.3f%%\n", accuracy);
	return accuracy;
}
void createCache() {
    mkdir("cachedTraining/", 0777);
}
void createND(int N, int D, char cachedND[]) {
    mkdir(cachedND, 0777);
}
void loadiM(int D, int imSize, char cachedND[], char itemMemory[]) {
	FILE *fileID;
	for(int i=0; i<imSize; i++) {
		char fiAd[34];
		int *randomHV;
		randomHV = (int*)malloc(D * sizeof(int));
			
		if (itemMemory[i] == ' ') {
			snprintf(fiAd, 34, "%s%s", cachedND, "im__.txt");
		}
		else {
			snprintf(fiAd, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".txt");
		}
		fileID = fopen(fiAd, "w");
		genRandomHV(D, randomHV);
		for(int j=0; j<D; j++) {
			fprintf(fileID, "%i ", randomHV[j]); 
		}
		fclose(fileID);
		free(randomHV);
	}	
}
void createIMHV(int D, int imSize, char itemMemory[], char cachedND[], int *imhv) {
    FILE *imf;
    char imAddress[34];

    for(int i=0; i<imSize; i++) {                                   //Loads the HVs from the character text files into an array (for speed purposes)
        if (itemMemory[i] == ' ') {
            snprintf(imAddress, 34, "%s%s", cachedND, "im__.txt");
        }
        else {
            snprintf(imAddress, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".txt");
        }

        imf = fopen(imAddress, "r");
        for(int j=0; j<D; j++) {
            int num=0;
            fscanf(imf, "%i", &num);
            *(imhv + i*D + j) = num;
        }
        fclose(imf);
    }
}
void buildLangHV(int N, int D, int length, char langLabels[][4], char cachedND[], int imSize, char itemMemory[], int *imhv) {
	FILE *fileID;
	FILE *langf;        //file ID for the lang .txt file

	char fileAddress[110];	

	for(int i=0; i<length; i++) {
		char *buffer;
		buffer = (char*)malloc(2000000 * sizeof(char));

		int *sumHV;
		sumHV = (int*)malloc(D * sizeof(int));

		if(buffer == NULL) {
			printf("Not enough memory!\n");
			exit(1);
		}

		snprintf(fileAddress, 110, "%s%s%s", "training_texts/", langLabels[i], ".txt");

		langf = fopen(fileAddress, "r");

		if (langf == NULL) {
			printf("Failed: %s could not be opened.\n", fileAddress);
			exit(1);
		}

		int count=0;
		while(1) {
			buffer[count] = fgetc(langf);
			if(feof(langf)) {
				buffer[count] = '\0';
				break;
			}
			count++;	
		}
		
		fclose(langf);
		printf("Loading: %s\n", fileAddress);

		computeSumHV(N, D, sumHV, count, buffer, imhv, itemMemory, imSize);

		char fiAd[34];                          //fileID for sumHV of lang
		snprintf(fiAd, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".txt");	
		fileID = fopen(fiAd, "w");

		for(int j=0; j<D; j++) {
		    fprintf(fileID, "%i ", sumHV[j]);
		}

		fclose(fileID);
		free(sumHV);
		free(buffer);
	}
}
void train(int N, int D, char itemMemory[], int imSize, int *imhv, char langLabels[][4], int length) {
	
    DIR* cacheDir;
    DIR* ndDir;
    FILE *fileID;
    char fileAddress[34];
    char cachedND[24];

    snprintf(cachedND, 24, "%s%d%s%d%s", "cachedTraining/", D, "_", N, "/");        //Directory information for cache
    cacheDir = opendir("cachedTraining");
    if (cacheDir) {                     //Checks that the cacheDir already exists
        ndDir = opendir(cachedND);  
        if (ndDir){
			for(int i=0; i<imSize; i++) {      		//Checks that all of the im files exist
				if (itemMemory[i] == ' ') {
					snprintf(fileAddress, 34, "%s%s", cachedND, "im__.txt");
				}
				else {
					snprintf(fileAddress, 34, "%s%s%c%s", cachedND, "im_", itemMemory[i], ".txt");
				}

				fileID = fopen(fileAddress, "r"); 
				if (fileID == NULL) {
					loadiM(D, imSize, cachedND, itemMemory);
					fclose(fileID);
					break;
				}

				fclose(fileID);
			}

			createIMHV(D, imSize, itemMemory, cachedND, imhv);

			for(int i=0; i<length; i++) {
				snprintf(fileAddress, 34, "%s%s%c%c%c%s", cachedND, "la_", langLabels[i][0], langLabels[i][1], langLabels[i][2], ".txt");
				fileID = fopen(fileAddress, "r");
				if (fileID == NULL) {
					buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, imhv);
					fclose(fileID);
					break;
				}
				fclose(fileID);
			}
            closedir(ndDir);
        }

        else if (ENOENT == errno) {                     //Checks that ndDir already exists (i.e. that if the cacheDir does not include memory for certain N&D, creates it)
            createND(N, D, cachedND);                  //Checks if the directory is empty/missing files
			loadiM(D, imSize, cachedND, itemMemory);
			createIMHV(D, imSize, itemMemory, cachedND, imhv);
			buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, imhv);
        }

        closedir(cacheDir);
    }
    else if (ENOENT == errno) {
        createCache();
        createND(N, D, cachedND);
        loadiM(D, imSize, cachedND, itemMemory);
	createIMHV(D, imSize, itemMemory, cachedND, imhv);
        buildLangHV(N, D, length, langLabels, cachedND, imSize, itemMemory, imhv);
    }
	
}
void comb(char *tmp) {
	char upper[] = "\nABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	char lower[] = " abcdefghijklmnopqrstuvwxyz ";
	for(int i=0; i<27; i++) {
		if(upper[i] == *tmp) 
			*tmp = lower[i];
	}
}
int personalTest(int N, int D, int *langAM, int *imhv, char itemMemory[], int imSize, char fileAddress[]) {

	char alpha[] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
	char temp;
	char cleanAddress[] = "clean.txt";
	
	int *testSumHV;
	testSumHV = (int*)malloc(D * sizeof(int));
	
	int *tmp;
	tmp = (int*)malloc(D * sizeof(int));
	
	char *text;
	text = (char*)malloc(300000 * sizeof(char));
	
	char fileLabel[][12] = {"afrikaans ", "bulgarian ", "czech ", "danish ", "dutch ", "german ", "english ", "estonian ", "finnish ", "french ", "greek ", "hungarian ", "italian ", "latvian ", "lithuanian ", "polish ", "portuguese ", "romanian ", "slovak ", "slovenian ", "spanish ", "swedish "};
	int length = (sizeof fileLabel)/(sizeof fileLabel[0]);
	
	double angle = 0.0;
	int predicLang;
	double maxAngle = -1.0;
	
	FILE *fileID = fopen(fileAddress, "r");
	
	if (fileID == NULL) {
		printf("Failed: Can't open %s\n", fileAddress);
		exit(1);
	}

	printf("Loaded: %s\n", fileAddress);
        int counter = 0;
	while(1) {
		temp = fgetc(fileID);
		if(feof(fileID)) {
			text[counter] = '\0';
			break;
		}
		else {
			for(int i=0; i<54; i++) {
				if(alpha[i] == temp) {
					comb(&temp);
					text[counter] = temp;
					counter++;
				}
			}
		}
	}
	printf("Finished creating testing file: %s\n", cleanAddress);

	fclose(fileID);
	
	FILE *wfile = fopen(cleanAddress, "w");
	if (wfile == NULL) {
		printf("Failed: %s could not be openned.\n", cleanAddress);
		exit(1);
	}	
	fprintf(wfile, "%s", text);
	
	computeSumHV (N, D, testSumHV, counter, text, imhv, itemMemory, imSize);
	binarizeHV(testSumHV, D);
	
	for(int l=0; l<length; l++) { //loop to go through the languages	
		for(int i=0; i<D; i++) {
			tmp[i] = *(langAM + l*D + i);
		}	

		angle = cosAngle(testSumHV, tmp, D);
		//printf("%c%c%c: %f\n", fileLabel[l][0], fileLabel[l][1], fileLabel[l][2], angle); //angles
		
		if (angle > maxAngle) {
			maxAngle = angle;
			predicLang = l;
		}
	}
	//double percentage = ((1+maxAngle)/2)*100;
		
	//printf("Guess: %d %.2f%% match\n", predicLang, percentage);

	free(text);
	free(tmp);
	free(testSumHV);
	
	return predicLang;

}
double program(int N, int D) {
    srand(time(0));
    struct timeval start, end; 
    struct timeval start2, end2; 
    double trainTime;
    double testTime;
    double accuracy = 0.0;

    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";     
    int imSize = sizeof(itemMemory);
    char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
    int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels

    int *imhv = (int*)malloc(imSize * D * sizeof(int));
    int *langAM = (int*)malloc(length * D * sizeof(int));

    gettimeofday(&start, NULL);                                            //comment out to not time
	train(N, D, itemMemory, imSize, imhv, langLabels, length);
    gettimeofday(&end, NULL);                                       //comment out to not time

    gettimeofday(&start2, NULL);                                                //comment out to not time
	storeLangAM(N, D, langAM, length, langLabels);
	accuracy = test(N, D, langAM, imhv, itemMemory, imSize);
    gettimeofday(&end2, NULL);                                           //comment out to not time
    
    trainTime = (end.tv_sec - start.tv_sec) * 1e6; 
    trainTime = (trainTime + (end.tv_usec - start.tv_usec)) * 1e-6; 
    testTime = (end2.tv_sec - start2.tv_sec) * 1e6; 
    testTime = (testTime + (end2.tv_usec - start2.tv_usec)) * 1e-6;                    //comment out to not time

    printf("Training: %.3fs\nTesting: %.3fs\nTotal: %.3fs\n", trainTime, testTime, (trainTime + testTime));   //comment out to not time

    free(imhv);
	free(langAM);
	return accuracy;
}
int sentence(int N, int D) {
	
	srand(time(0));
//struct timeval start, end; 
    struct timeval start2, end2; 
   // double trainTime;
    double testTime;
    int answer;
    int errorhandler;
    
	char fileAddress[] = "pyFile.txt"; //LINK TO PYTHON FILE

    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";     
    int imSize = sizeof(itemMemory);
    char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
    int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels

    int *imhv = (int*)malloc(imSize * D * sizeof(int));
    int *langAM = (int*)malloc(length * D * sizeof(int));

   // gettimeofday(&start, NULL);                                            //comment out to not time
	//train(N, D, itemMemory, imSize, imhv, langLabels, length);
    //gettimeofday(&end, NULL);                                       //comment out to not time
    gettimeofday(&start2, NULL);                                                //comment out to not time
	errorhandler = storeLangAMwithCheck(N, D, langAM, length, langLabels, imSize, itemMemory, imhv);
	if(errorhandler == -1) {
		answer = -1;
	}
	else if(errorhandler == -2) {
		answer = -2;
	}
	else {
		answer = personalTest(N, D, langAM, imhv, itemMemory, imSize, fileAddress);
		//answer = test(N, D, langAM, imhv, itemMemory, imSize);
	}
    gettimeofday(&end2, NULL);                                           //comment out to not time
    
   // trainTime = (end.tv_sec - start.tv_sec) * 1e6; 
    //trainTime = (trainTime + (end.tv_usec - start.tv_usec)) * 1e-6; 
    testTime = (end2.tv_sec - start2.tv_sec) * 1e6; 
    testTime = (testTime + (end2.tv_usec - start2.tv_usec)) * 1e-6;                    //comment out to not time

    printf("Training: 0.0s\nTesting: %.3fs\nTotal: %.3fs\n", testTime, testTime);   //comment out to not time

    free(imhv);
    free(langAM);
    return answer;
}
int training(int N, int D) {
	
	srand(time(0));
    struct timeval start, end; 
    double trainTime;
    
    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";     
    int imSize = sizeof(itemMemory);
    char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
    int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels

    int *imhv = (int*)malloc(imSize * D * sizeof(int));
    int *langAM = (int*)malloc(length * D * sizeof(int));

    gettimeofday(&start, NULL);                                            //comment out to not time
	train(N, D, itemMemory, imSize, imhv, langLabels, length);
    gettimeofday(&end, NULL);                                       //comment out to not time
    
     trainTime = (end.tv_sec - start.tv_sec) * 1e6; 
    trainTime = (trainTime + (end.tv_usec - start.tv_usec)) * 1e-6; 
      printf("Training: %.3fs\n", trainTime);   //comment out to not time

    free(imhv);
	free(langAM);
    
    return 1;
}
double percentage(int N, int D, int *langAM, int *imhv, char itemMemory[], int imSize, char fileAddress[]) {

	char alpha[] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
	char temp;
	char cleanAddress[] = "clean.txt";
	
	char fileLabel[][12] = {"afrikaans ", "bulgarian ", "czech ", "danish ", "dutch ", "german ", "english ", "estonian ", "finnish ", "french ", "greek ", "hungarian ", "italian ", "latvian ", "lithuanian ", "polish ", "portuguese ", "romanian ", "slovak ", "slovenian ", "spanish ", "swedish "};
	int length = (sizeof fileLabel)/(sizeof fileLabel[0]);
	
	int *testSumHV;
	testSumHV = (int*)malloc(D * sizeof(int));
	
	int *tmp;
	tmp = (int*)malloc(D * sizeof(int));
	
	char *text;
	text = (char*)malloc(300000 * sizeof(char));
	
	double angle = 0.0;
	double maxAngle = -1.0;
	
	FILE *fileID = fopen(fileAddress, "r");
	
	if (fileID == NULL) {
		printf("Failed: Can't open %s\n", fileAddress);
		exit(1);
	}

	printf("Loaded: %s\n", fileAddress);
        int counter = 0;
	while(1) {
		temp = fgetc(fileID);
		if(feof(fileID)) {
			text[counter] = '\0';
			break;
		}
		else {
			for(int i=0; i<54; i++) {
				if(alpha[i] == temp) {
					comb(&temp);
					text[counter] = temp;
					counter++;
				}
			}
		}
	}
	printf("Finished creating testing file: %s\n", cleanAddress);
	fclose(fileID);
	FILE *wfile = fopen(cleanAddress, "w");
	if (wfile == NULL) {
		printf("Failed: %s could not be openned.\n", cleanAddress);
		exit(1);
	}	
	fprintf(wfile, "%s", text);
	
	computeSumHV (N, D, testSumHV, counter, text, imhv, itemMemory, imSize);
	binarizeHV(testSumHV, D);
	
	for(int l=0; l<length; l++) { //loop to go through the languages	
		for(int i=0; i<D; i++) {
			tmp[i] = *(langAM + l*D + i);
		}	

		angle = cosAngle(testSumHV, tmp, D);
		//printf("%c%c%c: %f\n", fileLabel[l][0], fileLabel[l][1], fileLabel[l][2], angle); //angles
		
		if (angle > maxAngle) {
			maxAngle = angle;
		}
	}
	double percentage = ((1+maxAngle)/2)*100;
		
	//printf("Guess: %d %.2f%% match\n", predicLang, percentage);

	free(text);
	free(tmp);
	free(testSumHV);
	
	return percentage;

}
double sentencePercentage(int N, int D) {
	
	srand(time(0));

    double answer;
    int errorhandler;
    
	char fileAddress[] = "pyFile.txt"; //LINK TO PYTHON FILE

    char itemMemory[27] = "abcdefghijklmnopqrstuvwxyz ";     
    int imSize = sizeof(itemMemory);
    char langLabels[][4] = {"afr", "bul", "ces", "dan", "nld", "deu", "eng", "est", "fin", "fra", "ell", "hun", "ita", "lav", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
    int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels

    int *imhv = (int*)malloc(imSize * D * sizeof(int));
    int *langAM = (int*)malloc(length * D * sizeof(int));
                                              //comment out to not time
	errorhandler = storeLangAMwithCheck(N, D, langAM, length, langLabels, imSize, itemMemory, imhv);
	if(errorhandler == -1) {
		answer = -1;
	}
	else if(errorhandler == -2) {
		answer = -2;
	}
	else {
		answer = percentage(N, D, langAM, imhv, itemMemory, imSize, fileAddress);
		//answer = test(N, D, langAM, imhv, itemMemory, imSize);
	}

    free(imhv);
    free(langAM);
    return answer;
}
