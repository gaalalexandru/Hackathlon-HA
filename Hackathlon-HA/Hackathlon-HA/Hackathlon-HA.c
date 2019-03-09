/*
 * Hackathlon_HA.c
 *
 * Created: 3/9/2019 9:30:36 AM
 *  Author: Ninja
 */ 

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include "I2C_master.h"

#define DISPLAY_WRITE 0x4E 
#define DISPLAY_READ 0xF

void init_LCD(void){

	i2c_start(DISPLAY_WRITE);
	i2c_write(0x60); // set pointer to CRA
	i2c_write(0x88); // write 0x70 to CRA
	i2c_stop();

}

int main(void){
	
	i2c_init();
	init_LCD();
	
	while(1){

		_delay_ms(1000);
	}
	
	return 0;
}