/*
 * inits.c
 *
 * Created: 25.12.2019 18:24:44
 *  Author: Asus
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void init_ADC() //citire semnal | citire potentiometru
{
	ADMUX = 0b01000000;
	ADCSRA= 0b10000110;
}

void init_timer2() //Te=8000000/64/125 = 1/1000
{
	TCCR2A = 0b00000010; //CTC - OC2A-disconnected
	TCCR2B = 0b00000100; //pre 64
	
	TIMSK2 = 0b00000100; //OCIE2A Enable
	
	TCNT2 = 0;
	OCR2A = 125;
}

void init_timer0() //generare semnal
{	
// 	TCCR0A=0b00100011; //FAST PWM - clr OC0A on CM
// 	TCCR0B=0b00000001; //pre 1 - 31250 ??z

	DDRB |= 0b00000010;
	TCCR1A=0b10000001;
	TCCR1B=0b00001001;
}

void init_display()
{
	DDRD |= 0b00100000;
	//DDRB |= 0b11111111;
}

void init()
{
	init_ADC();
	init_timer0();
	init_timer2();
	init_display();
	
	
	DDRC &= ~(1<<5);//adc5
	PORTC |= (1<<5);//adc5
	DDRC |= (1<<4);//adc4
	
	sei();
}

void initLDC()
{
}