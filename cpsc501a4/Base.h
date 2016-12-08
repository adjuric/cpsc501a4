#ifndef BASE_H_
#define BASE_H_
float* readFile(char *fileName, float *signal, int *Thesize);
void writeFile(char *fileName, int numSamples, float *signal);
void baseConvolution(float x[], int N, float h[], int M, float y[], int P);
void scaleWave(float signal[], int samples);
#endif /* BASE_H_ */