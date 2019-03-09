/*
 * Hackathlon_HA.c
 *
 * Created: 3/9/2019 9:30:36 AM
 *  Author: Ninja
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include "util/delay.h"



int main(void)
{
	uint8_t u8state = 0;
	DDRB = (1<<5);
	
    while(1) {
        //TODO:: Please write your application code 
		u8state ^= 0x01;
		if (u8state) {
			PORTB |= (1 << 5);
		} else {
			PORTB &= ~(1 << 5);
		}
		_delay_ms(500);
    }
}