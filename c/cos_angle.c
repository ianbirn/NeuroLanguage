/***********************************************************************
 *	COMPUTING THE DISTANCE BETWEEN COSINE ANGLES
 * 
 * 
 * 
 *	MATLAB-SOURCE: https://github.com/abbas-rahimi/HDC-Language-Recognition
 * 
 * 
 * 
 **********************************************************************/
  
 
#include <math.h>
#include <stdio.h>

//----Finding the norm of a vector----//
void initialization(double a[], int n) {
	for (int i=0; i<n; i++) {
		a[i] = i;
	}
}

double norm(double a[], int n) {
	double sum=0;
	double norm=0;
	for (int i=0; i<n; i++) {
		sum += a[i] * a[i];
	}
	sum = (double)sum;
	norm = sqrt(sum);
	return norm;
}

//----Finding the dot product of vectors----//
double dotProduct(double a[], double b[], int n) {
	double product = 0;
	for (int i=0; i<n; i++) {
		product += a[i] * b[i];
	}	
	return (double)product;
}

//----Calcuating the Cosine Angle Distance----//
double cosAngle(double a[], double b[], int n) {
	return dotProduct(a, b, n)/(norm(a, n)*norm(b, n));	
}
