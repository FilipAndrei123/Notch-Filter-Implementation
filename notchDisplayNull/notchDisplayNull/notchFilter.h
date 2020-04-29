/*
 * notchFilter.h
 *
 * Created: 29.01.2020 15:41:07
 *  Author: Asus
 */ 


#ifndef NOTCHFILTER_H_
#define NOTCHFILTER_H_

double* a;
double* b;

double y[3];
double u[3]={0,0,0};

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

unsigned int longPress=0;

#endif /* NOTCHFILTER_H_ */