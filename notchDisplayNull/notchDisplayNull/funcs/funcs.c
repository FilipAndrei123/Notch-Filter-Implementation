/*
 * funcs.c
 *
 * Created: 29.01.2020 15:42:05
 *  Author: Asus
 */ 

#include <avr/io.h>
#include <math.h>

void getTf(double* a,double* b,double wo,double wc,double Te,double u[3],double y[3])
{
	double t1 = tan(wo*Te/2);
	double t2 = t1*t1;

	double p11 = wo*wo+wo*wo*t2;
	double p12=2*wo*wo*t2-2*wo*wo;
	double p13=wo*wo+wo*wo*t2;

	double p21=wo*wo+wc*wo*t1+wo*wo*t2;
	double p22=2*wo*wo*t2-2*wo*wo;
	double p23=wo*wo-wc*wo*t1+wo*wo*t2;

	p11 /= p21;
	p12 /= p21;
	p13 /= p21;
	p22 /= p21;
	p23 /= p21;
	p21 /= p21;

	*(b+0)=p13;
	*(b+1)=p12;
	*(b+2)=p11;

	*(a+0)=p23;
	*(a+1)=p22;
	*(a+2)=p21;
	
	/*u[0]=1;
	y[0]=*(b+2)*u[0];
	u[1] = 1;
	y[1]=-(*(a+1))*y[0] + (*(b+2))*u[1] + (*(b+1)*u[0]);
	u[2] = 1;
	y[2]=-(*(a+1))*y[1] - (*(a+0))*y[0] + (*(b+2))*u[2] + (*(b+1))*u[1] + (*(b+0)*u[0]);*/
}

double lsim(char input,double u[3],double y[3],double* a,double* b)
{
	y[0]=y[1];
	y[1]=y[2];

	u[0]=u[1];
	u[1]=u[2];
	u[2]=input;

	y[2] = -(*(a+1))*y[1] - (*(a+0))*y[0]+(*(b+2))*u[2]+(*(b+1))*u[1]+(*(b+0))*u[0];

	return y[2];
}


double HzToRads(double val)
{
	return val*2*M_PI;
}


int adcl, adch, adc;
int readADC (char ch){
	ADMUX  &= 0b11100000;
	ADMUX  |= ch;
	ADCSRA |= (1<<6);
	
	while(ADCSRA & (1<<6));
	
	adcl = ADCL;
	adch = ADCH;
	
	int val = adch << 8 | adcl;
	
	return (val);
}