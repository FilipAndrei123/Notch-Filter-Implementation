/*
 * BCD.c
 *
 * Created: 25.12.2019 18:37:52
 *  Author: Asus
 */ 

#include <avr/io.h>
#include <util/delay.h>

void displayNumber(int value)
{
	display(1, value%10);
	display(2,(value/10)%10);
	display(3,(value/100)%10);
	display(4,(value/1000)%10);
}

void display(char p, char c)
{
	PORTD &= 0b11110000;
	PORTB &= 0b00000000;
	
	switch(c)
	{
		case 0: PORTB |= 0b11000000; break;
		case 1: PORTB |= 0b11111001; break;
		case 2: PORTB |= 0b10100100; break;
		case 3: PORTB |= 0b10110000; break;
		case 4: PORTB |= 0b10011001; break;
		case 5: PORTB |= 0b10010010; break;
		case 6: PORTB |= 0b10000010; break;
		case 7: PORTB |= 0b11111000; break;
		case 8: PORTB |= 0b10000000; break;
		case 9: PORTB |= 0b10011000; break;
	}
	
	switch(p)
	{
		case 1: PORTD |= 0b00001000; break;
		case 2: PORTD |= 0b00000100; break;
		case 3: PORTD |= 0b00000010; break;
		case 4: PORTD |= 0b00000001; break;
	}
	
	_delay_ms(4);
}