#ifndef ABASED_H_
#define ABASED_H_
void base(char *inputFile,char *IRfile, char *outputfile);
void solve(float wavInput[], float irInput[], float result[], int size);
void fft(float data[], int nn, int isign);
#endif /* ABASED_H_ *