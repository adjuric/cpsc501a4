/*	Aleksandar Djuric
	10057804
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include <math.h>
#include <time.h>
#include "ABased.h"

int fsize;
clock_t t1, t2;
#define PI	3.141592653589793
#define TWO_PI	(2.0 * PI)
#define SWAP(a,b)	tempr=(a);(a)=(b);(b)=tempr

void abased(char *inputFile, char *IRfile, char *outputfile){

	printf("Frequency Based\n");
	char *DSound = inputFile;
	char *IResponse = IRfile;
	char *convolution = outputfile;
	int inputFileSignal;
	int IRFileSignal;

	// Data from both Dry and Impulse
	float* inputFileSignal = readData(DSound, inputFileSignal, &fsize);
	float* IRFileSignal = readData(IResponse, IRFileSignal, &fsize);
	inputFileSignal = fsize;
	IRFileSignal = fsize;

	// Intiating Array and Setting Size opf the output
	int outputFileSignal = inputFileSignal + IRFileSignal - 1;
	float *outputFileSignal = new float[outputFileSignal];

	int padSize = inputFileSignal + IRFileSignal - 1;
	int totalsize = 1;
	int someNum = 0;
	while(totalsize < padSize){
		totalsize <<= someNum;
		someNum++;
	}
	// Creating the Padding for the Dry File
	float *complex = new float [2*totalsize];
	float *input = new float[2*totalsize];
	float *ir = new float [2*totalsize];

	int i, k;
	for(i = 0, k = 0; i < inputFileSignal; i++, k+=2){
	    input[k] = inputFileSignal[i];
	    input[k + 1] = 0;
	}

	// pad for the impulse response file
	i = k;
	memset(input + k, 0, (2*totalsize) -1);

	for(i = 0, k = 0; i < IRFileSignal; i++, k+=2){
	    ir[k] = IRFileSignal[i];
	    ir[k + 1] = 0;
	}
	i = k;
	memset(ir + k, 0, (2*totalsize) -1);
	memset(complex, 0, (2*totalsize));
	
	//Using the FFT Algorithm from class with Padded Results
	fft(input-1, totalsize, 1);
	fft(ir-1, totalsize, 1);

	//Solving the Complex Number
	solve(input, ir, complex, totalsize);
	fft(complex-1, totalsize,-1);

	for(i=0; i < totalsize; i++){
		int sqrd = i*2;
		complex[sqrd] /= (float)totalsize;
		complex[(sqrd)+1] /= (float)totalsize;
	}

	for(i=0, k=0; i < outputFileSignal; i++, k+=2){
		outputFileSignal[i]=complex[k];
	}

	//Reusuing the Write and Scale functions in the Base Program
	scaleWave(outputFileSignal,outputFileSignal);
	writeFile(convolution, outputFileSignal, outputFileSignal);
}

// Created with the class material
void fft(float data[], int nn, int isign){
	 unsigned long n, mmax, m, j, istep, i;
	    float wtemp, wr, wpr, wpi, wi, theta;
	    float tempr, tempi;

	    n = nn << 1;
	    j = 1;

	    for (i = 1; i < n; i += 2)
	    {
	    	if (j > i)
	    	{
	    		SWAP(data[j], data[i]);
	    		SWAP(data[j+1], data[i+1]);
	    	}
	    	m = nn;
	    	while (m >= 2 && j > m)
	    	{
	    		j -= m;
	    		m >>= 1;
	    	}
	    	j += m;
	    }
	    mmax = 2;
	    while (n > mmax)
	    {
	    	istep = mmax << 1;
	    	theta = isign * ((TWO_PI) / mmax);
	    	wtemp = sin(0.5 * theta);
	    	wpr = -2.0 * wtemp * wtemp;
	    	wpi = sin(theta);
	    	wr = 1.0;
	    	wi = 0.0;
	    	for (m = 1; m < mmax; m += 2){
	    		for (i = m; i <= n; i += istep){
	    			j = i + mmax;
	    			tempr = wr * data[j] - wi * data[j+1];
	    			tempi = wr * data[j+1] + wi * data[j];
	    			data[j] = data[i] - tempr;
	    			data[j+1] = data[i+1] - tempi;
	    			data[i] += tempr;
	    			data[i+1] += tempi;
	    		}
	    		wr = (wtemp = wr) * wpr - wi * wpi + wr;
	    		wi = wi * wpr + wtemp * wpi + wi;
	    	}
	    	mmax = istep;
	    }
}
void solve(float wavInput[], float irInput[], float result[], int size){
	int i  = 0;
	for(i = 0; i < size; i++){
		result[i * 2] = wavInput[i * 2] * irInput[i * 2] - wavInput[(i * 2)+1] * irInput[(i * 2)+1];
		result[(i * 2) +1] = wavInput[(i * 2)+1] * irInput[i * 2] - wavInput[i * 2] * irInput[(i * 2)+1];
	}
}
