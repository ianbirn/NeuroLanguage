#ifndef COMPUTATIONS_H
#define COMPUTATIONS_H

/**Creates an array of values 1-D in a random order**/
void perm(int D, int *arr);

/**Stores a 1 or -1 to permutate array (1 for <= D/2 & -1 for >D/2)**/
void genRandomHV(int D, int *randomHV);

/**Shift an array down 1 and to the right 1**/
void circShift(int n, int d, int *arr);

/**Assigns the first row of block to the HV that the character key has stored**/
void lookupItemMemory(int D, int *imhv, char itemMemory[], char key, int *block, int imSize);

/**Does the M.A.P. computatiosn on an array and stores it into a sumHV**/
void computeSumHV(int N, int D, int *sumHV, int count, char *buffer, int *imhv, char itemMemory[], int imSize);

#endif