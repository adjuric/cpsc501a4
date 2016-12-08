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

int size, SC1Size, chunkSize, SRate, BRate, wavSize;
char *chunkID, *formatArr, *SC1ID, *SC2ID;
int16_t aFormat, channelNum, BAlign, bPS;
short* wavData;

void base(char *inputFile,char *IRfile, char *outputfile)
{
	printf("Base\n");

	char *DSound = inputFile;
	char *IResponse = IRfile;
	char *convolution = outputfile;
	int IRSize;

	// Data from Dry Clip and IR Clip
	float* inputFileSignal = readFile(DSound, inputFileSignal, &size);
	float* IRFileSignal = readFile(IResponse, IRFileSignal, &size);
	DSize = size;
	IRSize = size;
	int outSize = DSize + IRSize - 1;

	// Array for Output
	float *outputFileSignal = new float[outSize];

	// Convolution method from Class
	baseConvolution(inputFileSignal, DSize, IRFileSignal, IRSize, outputFileSignal, outSize);

	// Scaling the wave back
	scaleWave(outputFileSignal, outSize);

	// Output the Wave File
	writeFile(convolution, outSize, outputFileSignal);

	printf("Output: %s\n", convolution);

}

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

// Code Derived from Tutorial Example
void writeFile(char *fileName, int numSamples, float *signal)
{
	//Open Stream and Set Parameters
	ofstream outFile( fileName, ios::out | ios::binary);

	printf("Writing: %s\n", fileName);
	int chunkSize = channelNum * numSamples * (bPS / 8);
	// Get Riff header and set the variables
	chunkID = "RIFF";
	outFile.write( chunkID, 4);
	outFile.write( (char*) &chunkSize, 4);
	// Get wave header and set the variables
	formatArr = "WAVE";
	outFile.write( formatArr, 4);
	outFile.write( SC1ID, 4);
	SC1Size = 16;
	outFile.write( (char*) &SC1Size, 4);
	outFile.write( (char*) &aFormat, 2);
	outFile.write( (char*) &channelNum, 2);
	outFile.write( (char*) &SRate, 4);
	outFile.write( (char*) &BRate, 4);
	outFile.write( (char*) &BAlign, 2);
	outFile.write( (char*) &bPS, 2);

	// Sub Chunk ID2
	outFile.write( SC2ID, 4);

	wavSize = numSamples * 2;
	outFile.write( (char*)&wavSize, 4);

	// Set Wave Data, Then Scale back up
	int16_t val;
	for(int i = 0; i < numSamples; i++){
		val = (int16_t)(signal[i] * (32767));
		outFile.write((char*)&val, 2);
	}
	
	outFile.close();
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
