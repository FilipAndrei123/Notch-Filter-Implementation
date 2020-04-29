/*
 * notchFilter.c
 *
 * Created: 29.01.2020 15:40:48
 *  Author: Asus
 */ 
#define F_CPU 8000000UL

#include <avr/power.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "inits/inits.h"
#include "notchFilter.h"
#include "funcs/funcs.h"
#include "util/delay.h"

int main(void)
{
	clock_prescale_set(clock_div_1);
	
	a = (double*)malloc(3*sizeof(double));
	b = (double*)malloc(3*sizeof(double));
	
	double wo=HzToRads(50);
	double wc=HzToRads(10);
	double Te=0.001;
	
	getTf(a,b,wo,wc,Te,u,y);
	
	init();
	
	DDRB |= (1<<7);
	DDRB |= (1<<6);
	
	while (1)
	{
	}
}

ISR(TIMER2_COMPB_vect)
{
	//PORTB ^= (1<<6);
	OCR0B=(char)(lsim(readADC(0)/4,u,y,a,b));
	//	OCR1A=(char)(lsim(readADC(0)/4,u,y,a,b));
}

ISR(TIMER1_COMPA_vect)
{
	//OCR0B=(char)(lsim(readADC(0)/4,u,y,a,b));
}