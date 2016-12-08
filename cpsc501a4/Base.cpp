/*	Aleksandar Djuric
	10057804
 */
 
 #include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "Base.h"

using namespace std;

// Code from Lecture
void baseConvolution(float x[], int N, float h[], int M, float y[], int P) {
	int n, m;

	for (n = 0; n < P; n++)
		y[n] = 0.0;

	float percent = 0.0;
	for (n = 0; n < N; n++) {
		for (m = 0; m < M; m++)
		{
			y[n+m] += x[n] * h[m];
		}
	}
}