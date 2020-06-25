#ifndef TRAIN_H
#define TRAIN_H

/**Creates the cache directory**/
void createCache();

/**Creates a directory for new N & D values**/
void createND(int N, int D, char cachedND[]);

/**Loads in the hypervectors into character files**/
void loadiM(int D, int imSize, char cachedND[], char itemMemory[]);

/**Stores the character files' hypervectors into an array (for speed purposes)**/
void createIMHV(int D, int imSize, char itemMemory[], char cachedND[], int *imhv);

/**Builds the assistive memory for the different language files**/
void buildLangHV(int N, int D, int length, char langLabels[][4], char cachedND[], int imSize, char itemMemory[], int *imhv);

/**Runs all the above functions and checks if directories exist and have all necessary files**/
void train(int N, int D, char itemMemory[], int imSize, int *imhv, char langLabels[][4], int length);

#endif