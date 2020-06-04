#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gen_randomhv.h"
#include "cos_angle.h"


int main() {
	
    int x = 10000;
	double u[x];
	double v[x];
	
	int *ptr = genRandomHV(x);
	
	for (int i=0; i<x; i++) {
		printf("%i ", ptr[i]);
	}
	
	initialization(u, x);
	initialization(v, x);
	
	double distance = cosAngle(u, v, x);
	
	printf("%g\n", distance);
	
	free(ptr);
	
	return 0;
}
