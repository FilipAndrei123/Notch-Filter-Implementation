/*
 * notchFilter.h
 *
 * Created: 25.12.2019 19:39:05
 *  Author: Asus
 */ 


#ifndef NOTCHFILTER_H_
#define NOTCHFILTER_H_

double* a;
double* b;

double y[3];
double u[3]={0,0,0};
	
double yStep[3];
double uStep[3]={0,0,0};

double wo;
double wc;

unsigned int adcValue=0;
unsigned int lastAdcValue=0;

unsigned int freqVal=5;
unsigned int freqValX;
unsigned int freqValY;

unsigned int freqAmp=1;
unsigned int freqAmpX;
unsigned int freqAmpY;

unsigned int bandwidthVal=10;
unsigned int bandwidthValX;
unsigned int bandwidthValY;

unsigned int bandwidthAmp=1;
unsigned int bandwidthAmpX;
unsigned int bandwidthAmpY;

unsigned int selectedOption=1;
unsigned int onBodePlot=0;

unsigned int longPress=0;

void plotBode();
void plotStep();
void mainUI();
int heightOffset;

#endif /* NOTCHFILTER_H_ */