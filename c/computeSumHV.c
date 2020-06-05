#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	int R = 5;
	int C = 5;
	int block[R][C];
	int sumHV[1][C];
	
	
	printf("block:\n");
	for(int i=0; i<R; i++) {
		for(int j=0; j<C; j++) {
			block[i][j] = 0;
			printf("%d ", block[i][j]);
		}
		printf("\n");
	}
	
	printf("sumHV:\n");
	for(int i=0; i<1; i++) {
		for(int j=0; j<C; j++) {
			sumHV[i][j] = 0;
			printf("%d ", sumHV[i][j]);
		}
		printf("\n");
	}
	
	char fileAddress[] = "/home/pi/Desktop/sampleText";
	FILE *fileID = fopen(fileAddress, "r");
	
	if(fileID == NULL) {
		printf("Failed!\n");
		return 1;
	}
	
	int count = 0;
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
	
	printf("Loaded training file! %s\n", fileAddress);
	for(int i=0; i<count; i++) {
		printf("%c",buffer[i]);
	}
	
	printf("\n");
	printf("%s\n",buffer);
		
	return 0;
		
}
