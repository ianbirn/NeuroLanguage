#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

void perm(int D, int *arr);
void genRandomHV(int D, int *randomHV);
void circShift(int n, int d, int *arr);
int lookupItemMemory(int D, int *block, char *buffer, int index, char *itemMemory, int *iMHV, int byteSize);
void computeSumHV(int N, int D, int *sumHV, char *buffer, int count, char *itemMemory, int *iMHV, int byteSize);
void binarizeHV(int *v, int szofv);
double norm(int *a, int n);
double dotProduct(int *a, int *b, int n);
double cosAngle(int *a, int *b, int n);
void cleanText(char *buffer, int size, char *itemMemory, int imSize, int byteSize);
void removeChar(char *s, int c);
void removeSubstr (char *string, char *sub);

int main() {
    srand(time(0));
    DIR *dir;
    struct dirent *sd;
    FILE *f;
    int D = 10000;
    int N = 3;
    const int byteSize = 165;
    int imSize = ((byteSize-27)/2)+27;
    char *itemMemory = (char*)malloc(byteSize * sizeof(char));
	//char langLabels[][4] = {"afr", "ces", "dan", "deu", "nld", "eng", "est", "fin", "fra", "hun", "isl", "ita", "lat", "lit", "nor", "pol", "por", "rom", "slk", "slv", "spa", "swe"};
    char langLabels[][4] = {"afr", "ces", "dan", "deu", "nld", "eng", "est", "fin", "fra", "hun", "ita", "lat", "lit", "pol", "por", "ron", "slk", "slv", "spa", "swe"};
    char fileLabel[][3] = {"af", "cs", "da", "de", "nl", "en", "et", "fi", "fr", "hu", "it", "lv", "lt", "pl", "pt", "ro", "sk", "sl", "es", "sv"};
	int length =  (sizeof langLabels)/(sizeof langLabels[0]); //size of langLabels
    int *langAM = (int*)malloc(length * D * sizeof(int));
    char fileAd[273];
    char *alpha = (char*)malloc(byteSize * sizeof(char));

    f = fopen("alpha.txt", "r");
    int u=0;
    while(1) {
        alpha[u] = fgetc(f);
        if (feof(f)) {
            alpha[u] = '\0';
		    break;
        }
        u++;
    }
    fclose(f);

    int *iMHV = (int*)malloc(imSize * D * sizeof(int));

    f = fopen("itemMemory.txt", "r");
    int x=0;
    while(1) {
        itemMemory[x] = fgetc(f);
        if (itemMemory[x] == '\n') {
            itemMemory[x] = '\0';
		    break;
        }
        else if (feof(f)) {
            itemMemory[x] = '\0';
            break;
        }
        x++;
    }
    fclose(f);

    for(int i=0; i<imSize; i++) {
        int *randomHV = (int*)malloc(D * sizeof(int));
        genRandomHV(D, randomHV);
        for(int j=0; j<D; j++) {
            *(iMHV + i*D + j) = randomHV[j];
        }
        free(randomHV);
    }

    for(int l=0; l<length; l++) {
        char *buffer = (char*)malloc(2000000 * sizeof(char));
        int *sumHV = (int*)malloc(D * sizeof(int));
        char fileAddress[24];

        snprintf(fileAddress, 24, "%s%s%s", "../newTraining/", langLabels[l], ".txt");
        f = fopen(fileAddress, "r");
        if (f == NULL) { 
            printf("Error\n");
        }

        int count=0;
        while(1) {
            buffer[count] = fgetc(f);
		    if(feof(f)) {
	  	        buffer[count] = '\0';
		        break;
		    }
            count++;
        }
        fclose(f);
        printf("Loading %s\n", fileAddress);
        computeSumHV(N, D, sumHV, buffer, count, itemMemory, iMHV, byteSize);

        for(int i=0; i<D; i++) {
            *(langAM + l*D + i) = sumHV[i];
            //printf("%i ", *(langAM + l*D + i));
        }
        //printf("\n");
        free(sumHV);
        free(buffer);
    }

    int total=0; int correct=0; int correct2=0;
    double accuracy=0.0; double accuracy2=0.0;
    dir = opendir("../test_expand/");

    if (dir == NULL) {
        printf("Failed: Directory could not be openned.\n");
        exit(1);
    }
	while((sd=readdir(dir)) != NULL) {
        int *testSumHV = (int*)malloc(D * sizeof(int));
        char *buff = (char*)malloc(3000000 * sizeof(char));
        char *predicLang = (char*)malloc(3 * sizeof(char));
        char *predicLang2 = (char*)malloc(3 * sizeof(char));
        double maxAngle = -1.0; double angle2=0.0; double diff=0.0;
        int newSize=0;
        if (!strcmp (sd->d_name, "."))
            continue;
        if (!strcmp (sd->d_name, ".."))    
            continue;
        
        snprintf(fileAd, 273, "%s%s", "../test_expand/", sd->d_name);
        f = fopen(fileAd, "r");
        int c=0;
        while(1) {
            buff[c] = fgetc(f);
            if (buff[c] == '\n') {
                buff[c] == ' ';
            }
            else if(feof(f)) {
                buff[c] = '\0';
                break;
            }
            c++;
        }
        fclose(f);
        //cleanText(buff, c, itemMemory, imSize, byteSize);
        printf("Loaded %s\n", fileAd);

        computeSumHV(N, D, testSumHV, buff, c, itemMemory, iMHV, byteSize);
        binarizeHV(testSumHV, D);

        for(int l=0; l<length; l++) {
            double angle=0.0;
            int *tmp = (int*)malloc(D * sizeof(int));
            
            for(int i=0; i<D; i++) {
                tmp[i] = *(langAM + l*D + i);
            }

            angle = cosAngle(tmp, testSumHV, D);
            //printf("%s\t%.3f\n", langLabels[l], angle);

            if (angle > maxAngle) {
                angle2 = maxAngle;
                maxAngle = angle;
                //sprintf(predicLang2, "%s", predicLang);
                sprintf(predicLang, "%s", fileLabel[l]);
            }
            /*else if (angle > angle2) {
                angle2 = angle;
                sprintf(predicLang2, "%s", fileLabel[l]);
            }*/
            free(tmp);
        }

        //printf("%s\n", predicLang);

        if (((sd->d_name)[0] == predicLang[0]) && ((sd->d_name)[1] == predicLang[1])) {
            correct++;
        }
        /*else if (((sd->d_name)[0] == predicLang2[0]) && ((sd->d_name)[1] == predicLang2[1])) { 
            printf("%s\n", predicLang);
            correct2++;
        }*/

        total++;
        free(predicLang);
        free(predicLang2);
        free(buff);  
        free(testSumHV);
    }
    closedir(dir);

    accuracy = (((double)correct)/((double)total)) * 100;
    printf("First Guess: %.3f\t", accuracy);
    //accuracy2 = (((double)correct2)/((double)total)) * 100;
    //printf("Second Guess: %.3f\tRemainder: %.3f", accuracy2, (100-(accuracy+accuracy2)));


    free(iMHV);
    free(alpha);
    free(itemMemory);
    free(langAM);
    return 0;
}
void cleanText(char *buffer, int size, char *itemMemory, int imSize, int byteSize) {
    char *alpha = (char*)malloc(byteSize * sizeof(char));
    FILE *f;
    int newSize=0;

    f = fopen("alpha.txt", "r");
    int x=0;
    while(1) {
        alpha[x] = fgetc(f);
        if (feof(f)) {
            alpha[x] = '\0';
		    break;
        }
        x++;
    }
    fclose(f);

	removeChar(buffer, '\t'); removeChar(buffer, '.'); removeChar(buffer, ';'); removeChar(buffer, '\?'); removeChar(buffer, '!');
	removeChar(buffer, '%'); removeChar(buffer, ':'); removeChar(buffer, '-'); removeChar(buffer, '\"'); removeChar(buffer, '\'');
	removeChar(buffer, ','); removeChar(buffer, '('); removeChar(buffer, ')'); removeChar(buffer, '\\'); removeChar(buffer, '/');
	removeChar(buffer, '<'); removeChar(buffer, '>'); removeChar(buffer, '~'); removeChar(buffer, '`'); removeChar(buffer, '=');
	removeSubstr(buffer, "»"); removeSubstr(buffer, "«"); removeSubstr(buffer, "„"); removeSubstr(buffer, "“"); removeSubstr(buffer, "‚");   
	removeSubstr(buffer, "’");  removeSubstr(buffer, "–"); removeSubstr(buffer, "…"); removeSubstr(buffer, "▶︎"); removeSubstr(buffer, "·");
	removeSubstr(buffer, "&"); removeSubstr(buffer, "°"); removeSubstr(buffer, "‘"); removeSubstr(buffer, "—"); removeSubstr(buffer, "*");
	removeSubstr(buffer, "$"); removeSubstr(buffer, "+"); removeSubstr(buffer, "´"); removeSubstr(buffer, "•"); removeSubstr(buffer, "”");
	removeSubstr(buffer, "§");

    for(int i=0; i<10; i++) {
        char num = i + '0';
        removeChar(buffer, num);
    }

    newSize = strlen(buffer);

    for(int i=0; i<newSize; i++) {
        for(int j=0; j<27; j++) {
            if(alpha[j] == buffer[i]) {
                buffer[i] = itemMemory[j];
            }
        }
        for(int j=27; j<byteSize; j=j+2) {
            if((alpha[j] == buffer[i]) && (alpha[j+1] == buffer[i+1])) {
                buffer[i] = itemMemory[j];
                buffer[i+1] = itemMemory[j+1];
                //printf("%c%c\n", buffer[i], buffer[i+1]);
                i++;
            }
        }
    }

    free(alpha);
}
void removeChar(char *s, int c) { 
  
    int j, n = strlen(s); 
    for (int i=j=0; i<n; i++) 
       if (s[i] != c) 
          s[j++] = s[i]; 
      
    s[j] = '\0'; 
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

void binarizeHV(int *v, int szofv) {
	int threshold = 0;
	for( int i=0; i<szofv; i++) {
		if( v[i] > threshold ) 
			v[i] = 1;
		else 
			v[i] = -1;
	}
}
void computeSumHV(int N, int D, int *sumHV, char *buffer, int count, char *itemMemory, int *iMHV, int byteSize) {
    int *block = (int*)malloc(N * D * sizeof(int));
     for(int i=0; i<D; i++) {
        sumHV[i] = 0;
    }

    for(int i=0; i<N; i++) {
        for(int j=0; j<D; j++) {
            *(block + i*D + j) = 0;
        }
    }

    for(int i=0; i<count; i++) {
        circShift(N, D, block);
        i = lookupItemMemory(D, block, buffer, i, itemMemory, iMHV, byteSize);

        if (i >= N) {
            int *nGrams;
            nGrams = (int*)malloc(D * sizeof(int));

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
int lookupItemMemory(int D, int *block, char *buffer, int index, char *itemMemory, int *iMHV, int byteSize) {
    int y;
    int imIndex=27;
    char key[2];
    for(int i=0; i<27; i++) {
        if(buffer[index] == itemMemory[i]) {
            for(int j=0; j<D; j++) {
                *(block + j) = *(iMHV + i*D + j);
            }
            y = 0;                             //key is found in 27 latin 1 byte
            break;
        }
        else {
            y=1;
        }
    }

    if(y==1) {
        key[0] = buffer[index];
        key[1] = buffer[index+1];
        
        for(int i=27; i<byteSize; i=i+2) {
            //printf("%c%c", itemMemory[i], itemMemory[i+1]);/*
            if((key[0] == itemMemory[i]) && (key[1] == itemMemory[i+1])) {
                //buffer[i]
                //printf("%c%c ", key[0], key[1]);
                buffer[index] = key[0];
                buffer[index+1] = key[1];
                //printf("%c%c\t", buffer[index], buffer[index+1]);
                
                for(int j=0; j<D; j++) {
                    *(block + j) = *(iMHV + imIndex*D + j);
                }
                index++;
                break;
            }
            imIndex++;
        }
    }
    return index;
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
void removeSubstr (char *string, char *sub) {
    char *match;
    int len = strlen(sub);
    while ((match = strstr(string, sub))) {
        *match = '\0';
        strcat(string, match+len);
    }
}
