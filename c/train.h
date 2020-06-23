#ifndef TRAIN_H
#define TRAIN_H

void createCache();
void createND(int N, int D, char cachedND[]);
void loadiM(int D, int imSize, char cachedND[], char itemMemory[]);
void createIMHV(int D, int imSize, char itemMemory[], char cachedND[], int *imhv);
void buildLangHV(int N, int D, int length, char langLabels[][4], char cachedND[], int imSize, char itemMemory[], int *imhv);
void train(int N, int D, char itemMemory[], int imSize, int *imhv, char langLabels[][4], int length);

#endif