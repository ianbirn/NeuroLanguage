#ifndef TEST_H
#define TEST_H

/**Stores the HVs from the language files into an array (for speed purposes)**/
void storeLangAM(int N, int D, int *langAM, int length, char langLabels[][4]);

/**Binarizes the sumHV (1 for values >0 && -1 for values <0)**/
void binarizeHV(int *v, int szofv);

/**Finds the magnitude of an array - norm(); func. in Matlab**/
double norm(int *a, int n);

/**Returns the dot product of two arrays**/
double dotProduct(int *a, int *b, int n);

/**Compute the cos Angle distance between two arrays**/
double cosAngle(int *a, int *b, int n);

/**Compute HVs on different testfiles and runs similarity tests with language files to predict the language of each test file**/
void test(int N, int D, int *langAM, int *iMHV, wchar_t *itemMemory, int imSize, char actualLabels[][11], char langLabels[][4], int length, wchar_t *unknown);

#endif
