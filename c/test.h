#ifndef TEST_H
#define TEST_H

void storeLangAM(int N, int D, int *langAM, int length, char langLabels[][4]);
void binarizeHV(int *v, int szofv);
double norm(int *a, int n);
double dotProduct(int *a, int *b, int n);
double cosAngle(int *a, int *b, int n);
void test(int N, int D, int *langAM, int *imhv, char itemMemory[], int imSize);

#endif