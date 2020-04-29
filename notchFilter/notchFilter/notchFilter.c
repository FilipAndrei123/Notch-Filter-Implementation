/*
 * notchFilter.c
 *
 * Created: 11.12.2019 22:32:28
 * Author : Asus
 */ 
#define F_CPU 8000000UL

#include <avr/power.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "inits/inits.h"
#include "notchFilter.h"
#include "funcs/funcs.h"
#include "util/delay.h"
#include "display/BCD.h"
#include "float.h"
#include "glcd.c"
#include "colors.h"

int main(void)
{
	clock_prescale_set(clock_div_1);
 	
 	glcd_init();
 	glcd_led_on();
 	
 	a = (double*)malloc(3*sizeof(double));
 	b = (double*)malloc(3*sizeof(double));
 	
 	wo=HzToRads(50);
 	wc=HzToRads(10);
 	double Te=0.001008064516129;
 	
 	getTf(a,b,wo,wc,Te,u,y);
 
 	init();
	
	mainUI();
	
	DDRC |=(1<<1);
    while (1) 
    {
		if (!onBodePlot)
		{
 	    adcValue = readADC(3)/100;
		 
		if (adcValue!=lastAdcValue)
			switch(selectedOption)
			{
				case 1:
				{
					freqVal=adcValue;
					
					glcd_number(lastAdcValue,freqValX,freqValY,2,white);
					glcd_number(freqVal,freqValX,freqValY,2,black);
				} break;
				
				case 2:
				{
					freqAmp=adcValue;
					
					glcd_number(lastAdcValue,freqAmpX,freqAmpY,2,white);
					glcd_number(freqAmp,freqAmpX,freqAmpY,2,black);
				} break;
	
				case 3:
				{
					bandwidthVal=adcValue;
					
					glcd_number(lastAdcValue,bandwidthValX,bandwidthValY,2,white);
					glcd_number(bandwidthVal,bandwidthValX,bandwidthValY,2,black);
				} break;
				
				case 4:
				{
					bandwidthAmp=adcValue;
					
					glcd_number(lastAdcValue,bandwidthAmpX,bandwidthAmpY,2,white);
					glcd_number(bandwidthAmp,bandwidthAmpX,bandwidthAmpY,2,black);
				} break;
			}
		lastAdcValue = adcValue;
		
		if (!(PINC & (1<<5)))
		{
			longPress++;
			
			if (longPress>4&&longPress<8)
				PORTC |= (1<<4);
			else 
				PORTC &= ~(1<<4);
		}
		else if (longPress != 0)
		{
			if (longPress<4)
			{
				selectedOption++;
				if (selectedOption == 5)
					selectedOption = 1;
				longPress = 0;
			} else if (longPress>4&&longPress<8)
			{
				PORTC &= ~(1<<4);
				
				
				glcd_double(*(b+2),5*2*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,0,white);
				glcd_double(*(b+1),5*2*FONT_SPACE+7*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,1,white);
				glcd_double(*(b+0),5*2*FONT_SPACE+14*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,1,white);
				glcd_double(*(a+2),5*2*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,0,white);
				glcd_double(*(a+1),5*2*FONT_SPACE+7*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,1,white);
				glcd_double(*(a+0),5*2*FONT_SPACE+14*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,1,white);
				
				wo=HzToRads(freqVal*pow(10,freqAmp));
				wc=HzToRads(bandwidthVal*bandwidthAmp);
				
				getTf(a,b,wo,wc,Te,u,y);
				
				glcd_double(*(b+2),5*2*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,0,black);
				glcd_double(*(b+1),5*2*FONT_SPACE+7*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,1,black);
				glcd_double(*(b+0),5*2*FONT_SPACE+14*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,1,black);
				glcd_double(*(a+2),5*2*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,0,black);
				glcd_double(*(a+1),5*2*FONT_SPACE+7*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,1,black);
				glcd_double(*(a+0),5*2*FONT_SPACE+14*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,1,black);
				
				longPress =0;
				//TODO update tf
			} else if (longPress>8)
			{
				onBodePlot = 1;
				longPress = 0;
			}
		}
			
			_delay_ms(500);
		} else
		{//ONBODEPLT
			glcd_bg(white);
			plotBode();
			
			while((PINC & (1<<5)))
			{
			}
			
			glcd_bg(white);
			plotStep();
						
			while((PINC & (1<<5)))
			{
			}
			
			glcd_bg(white);
			onBodePlot=0;
			
			mainUI();
		}
	}
}

ISR(TIMER2_COMPB_vect)
{
	OCR1A=(char)(lsim(readADC(0)/4,u,y,a,b));
	PORTC^=(1<<1);
}

void mainUI()
{
		heightOffset=0;
		  
		glcd_fillRectangle(0,0,max_x,2*2*FONT_SPACE+2*2*FONT_SPACE,yellow);
		glcd_string("Proiect",0,0,3,black);
		glcd_string("Teoria Sistemelor II",0,3*FONT_SPACE+10,2,black);
		  
		heightOffset=2*2*FONT_SPACE+2*2*FONT_SPACE;
		  
		glcd_string("Universitatea Tehnica Cluj-Napoca",0,240-1*FONT_SPACE,1,black);
		  
		  
		heightOffset +=10;
		glcd_string("Frecventa actuala:",0,heightOffset,2,black);
		  
		heightOffset += 2*FONT_SPACE+5;
		freqValY=heightOffset;
		freqValX=0;
		freqAmpY=heightOffset;
		freqAmpX=8*2*FONT_SPACE;
		glcd_string("5  * 10^1",0,heightOffset,2,black);
		glcd_string("Hz",10*2*FONT_SPACE,heightOffset,2,black);
		  
		heightOffset+= 30;
		glcd_string("Lungimea de banda:",0,heightOffset,2,black);
		  
		  
		heightOffset += 2*FONT_SPACE+5;
		bandwidthValY=heightOffset;
		bandwidthValX=0;
		bandwidthAmpY=heightOffset;
		bandwidthAmpX=4*2*FONT_SPACE;
		glcd_string("10 *1",0,heightOffset,2,black);
		glcd_string("Hz",6*2*FONT_SPACE,heightOffset,2,black);
		  
		heightOffset+= 30;
		glcd_string("Functia de transfer:",0,heightOffset,2,black);
		  
		heightOffset+=40;
		  
		glcd_string("H(z)=",0,heightOffset,2,black);
		glcd_double(*(b+2),5*2*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,0,black);
		glcd_double(*(b+1),5*2*FONT_SPACE+7*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,1,black);
		glcd_double(*(b+0),5*2*FONT_SPACE+14*FONT_SPACE,heightOffset-FONT_SPACE/2-3,1,1,black);
		glcd_double(*(a+2),5*2*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,0,black);
		glcd_double(*(a+1),5*2*FONT_SPACE+7*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,1,black);
		glcd_double(*(a+0),5*2*FONT_SPACE+14*FONT_SPACE,heightOffset+2*FONT_SPACE/2+3,1,1,black);
		  
		glcd_line(5*2*FONT_SPACE,heightOffset+2*FONT_SPACE/2,5*2*FONT_SPACE+20*FONT_SPACE,heightOffset+2*FONT_SPACE/2,black);
}

void plotStep()
{
	double t=0;
	double y1=120-lsim(1,uStep,yStep,a,b)*500;
	double y2=0;
	
	glcd_line(40,0,40,240,red);
	
	for (t=0;t<279;t++)
	{
		y2=120-lsim(1,uStep,yStep,a,b)*500;
		
		glcd_line(40+t,y1,40+t+1,y2,black);
		
		y1=y2;
	}
	
	yStep[0]=0;
	yStep[1]=0;
	yStep[2]=0;
}

void plotBode()
{
	 	double modul,modulaux=0;
	 	double faza,fazaaux=0;
	 	
	 	wo=RadsToHz(wo);
	 	wc=RadsToHz(wc);
	 	
	 	double t=0;
	 	
	 	glcd_line(20,120,320,120,black);
	 	
	 	glcd_line(20,125,20,115,black);		glcd_number(0,20,130,1,black);
	 	glcd_line(40,125,40,115,black);		glcd_number(20,40,130,1,black);
	 	glcd_line(60,125,60,115,black);		glcd_number(40,60,130,1,black);
	 	glcd_line(80,125,80,115,black);		glcd_number(60,80,130,1,black);
	 	glcd_line(100,125,100,115,black);	glcd_number(80,100,130,1,black);
	 	glcd_line(120,125,120,115,black);	glcd_number(100,120,130,1,black);
	 	glcd_line(140,125,140,115,black);	glcd_number(120,140,130,1,black);
	 	glcd_line(160,125,160,115,black);	glcd_number(140,160,130,1,black);
	 	glcd_line(180,125,180,115,black);	glcd_number(160,180,130,1,black);
	 	glcd_line(200,125,200,115,black);	glcd_number(180,200,130,1,black);
	 	glcd_line(220,125,220,115,black);	glcd_number(200,220,130,1,black);
	 	glcd_line(240,125,240,115,black);	glcd_number(220,240,130,1,black);
	 	glcd_line(260,125,260,115,black);	glcd_number(240,260,130,1,black);
	 	glcd_line(280,125,280,115,black);	glcd_number(260,280,130,1,black);
	 	glcd_line(300,125,300,115,black);	glcd_number(280,300,130,1,black);
	 	glcd_line(320,125,320,115,black);	glcd_number(300,320,130,1,black);
	 	glcd_line(20,0,20,240,red);
	 	
	 	double minM=9999;
	 	double maxM=0;
	 	double minF=9999;
	 	double maxF=0;
	 	double Ffactor=0;
	 	double Mfactor=0;
	 	
	 	for(t=1;t<320;t++)
	 	{
		 	modul= round(fabs(20*log10(fabs(-(t*t)+wo*wo)/sqrt((-(t*t)+wo*wo)*(-(t*t)+wo*wo)+(wc*t)*(wc*t)))));
		 	faza = 90+round(atan(wc*t/(wo*wo - (t*t)))*180/M_PI);
		 	
		 	if (isinf(modul))
		 	modul=maxM;
		 	
		 	if (maxM<modul)
		 	maxM=modul;
		 	if (maxF<faza)
		 	maxF=faza;
		 	
		 	if (minF>faza)
		 	minF=faza;
		 	if (minM>modul)
		 	minM=modul;
	 	}
	 	
	 	//glcd_number(maxF,0,0,2,black);
	 	//glcd_number(minF,0,20,2,black);
	 	//glcd_number(maxM,0,40,2,black);
	 	//glcd_number(minF,0,60,2,black);
	 	
	 	Ffactor=120/maxF;
	 	Mfactor=100/maxM;
	 	
	 	t=1;
	 	modulaux = 10 + round(fabs(20*log10(fabs(-(t*t)+wo*wo)/sqrt((-(t*t)+wo*wo)*(-(t*t)+wo*wo)+(wc*t)*(wc*t)))))*Mfactor;
	 	fazaaux =120 + 90+round(atan(wc*t/(wo*wo - (t*t)))*180/M_PI)*Ffactor - minF;
	 	
	 	for(t=1;t<320;t++)
	 	{
		 	modul=10 + round(fabs(20*log10(fabs(-(t*t)+wo*wo)/sqrt((-(t*t)+wo*wo)*(-(t*t)+wo*wo)+(wc*t)*(wc*t)))))*Mfactor;
		 	faza =120 + 90+round(atan(wc*t/(wo*wo - (t*t)))*180/M_PI)*Ffactor - minF;
		 	
		 	if (isinf(modul))
		 	modul=maxM*Mfactor;
		 	glcd_line(20+t-1,modulaux,20+t,modul,black);
		 	glcd_line(20+t-1,fazaaux,20+t,faza,black);
		 	
		 	modulaux=modul;
		 	fazaaux=faza;
	 	}
	 	
	 	glcd_line(20,10,25,10,red); glcd_number(0,0,10,1,red);
	 	glcd_line(20,100,25,100,red); glcd_number(-14,0,100,1,red);
	 	glcd_line(20,145,25,145,red); glcd_number(90,0,140,1,red);
	 	glcd_line(20,200,25,200,red); glcd_number(0,0,200,1,red);
	 	glcd_line(20,240,25,240,red); glcd_number(-90,0,230,1,red);
}