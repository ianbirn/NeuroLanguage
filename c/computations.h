#ifndef COMPUTATIONS_H
#define COMPUTATIONS_H

void perm(int D, int *arr);
void genRandomHV(int D, int *randomHV);
void circShift(int n, int d, int *arr);
void lookupItemMemory(int D, int *imhv, char itemMemory[], char key, int *block, int imSize);
void computeSumHV(int N, int D, int *sumHV, int count, char *buffer, int *imhv, char itemMemory[], int imSize);

#endif