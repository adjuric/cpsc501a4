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

float* readFile(char *fileName, float *signal, int *Thesize)
{
	ifstream inputFile( fileName, ios::in | ios::binary);

	printf("Reading: %s\n", fileName);
	// Start at the Begining
	inputFile.seekg(ios::beg);
	// Header data from the wave file
	chunkID = new char[4];
	inputFile.read( chunkID, 4);
	inputFile.read( (char*) &chunkSize, 4);
	formatArr = new char[4];
	inputFile.read( formatArr, 4);
	SC1ID = new char[4];
	inputFile.read( SC1ID, 4);
	inputFile.read( (char*) &SC1Size, 4);
	inputFile.read( (char*) &aFormat, 2);
	inputFile.read( (char*) &channelNum, 2);
	inputFile.read( (char*) &SRate, 4);
	inputFile.read( (char*) &BRate, 4);
	inputFile.read( (char*) &BAlign, 2);
	inputFile.read( (char*) &bPS, 2);

	// Sub chunk id 1, random collection
	if(SC1Size == 18){
		char *random;
		random = new char[2];
		inputFile.read(random, 2);
	}
	// Sub chunk id 2, for the wave data chunk
	SC2ID = new char[4];
	inputFile.read( SC2ID, 4);

	//Data size
	inputFile.read( (char*)&wavSize, 4);

	*Thesize = wavSize / 2;
	int size = wavSize / 2;
	wavData = new short[size];
	
	// Reading Data
	for(int j = 0 ; j < size; j++){
		inputFile.read((char*) &wavData[j], 2);
	}
	printf("\n Creating Signal\n");

	// Signal which will be used in Convolution
	// Derived from Lecture
	short val;
	signal = new float[size];
	for(int i = 0; i < size; i++)
	{
		val = wavData[i];
		signal[i] = (val * 1.0) / (32767);
		if(signal[i] < -1.0)
			signal[i] = -1.0;
	}
	inputFile.close();

	return signal;
}

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

// Scale the File, code from lectures/tutorials
void scaleWave(float signal[], int samples)
{
	printf("Scaling\n");
	float min = 0, max = 0;
	int i = 0;

	for(i = 0; i < samples; i++){
		if(signal[i] > max){
			max = signal[i];
		}
		if(signal[i] < min){
			min = signal[i];
		}
	}

	min *= -1;
	if(min > max){
		max = min;
	}

	for(i = 0; i < samples; i++){
		signal[i] = signal[i] / max;
	}
}
