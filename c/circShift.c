#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//This function is just for testing purposes
void initialization( int n, int d, int arr[][d]){
  int x=1;
  for(int i=0; i<n; i++) {
    for(int j=0; j<d; j++) {
      arr[i][j] = x;
      printf("%3i ", arr[i][j]);
      x++;
    }
    printf("\n");
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
      //printf("%3i ", arr1[i][j]);
    }
    //rintf("\n");
  }
  //printf("\n");
  
  //Pushes every column to the right by 1
  int arr2[n][d];
  for(int i=0; i<n; i++) {
    for(int j=0; j<=d; j++) {
      if (j == (d-1)) {
        arr2[i][0] = arr1[i][d-1];
        //printf("%i ", arr2[i][0]);   #this was during testing
      }
      else {
        arr2[i][j+1] = arr1[i][j];
      }
    }
  }

  //Prints out the final results (only necessary for testing, can be deleted)
  for(int i=0; i<n; i++) {
    for(int j=0; j<d; j++) {
      printf("%3i ", arr2[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main() {
	//Declarations/initializations for computeSumHV
  int n=3; int d=2;

	int block[n][d];
  printf("Original array: \n");
  initialization(n, d, block);

  printf("\n");
  printf("Shifted array: \n");
  circShift(n, d, block);
  

	return 0;
		
}
