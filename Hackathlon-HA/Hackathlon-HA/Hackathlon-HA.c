
#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

#include <I2C_master.h>

#define LCD_WRITE 0X4E;


int main()
{
	i2c_init;
	i2c_send_start();
	i2c_send_adr(0x4E);
	i2c_write("La alba ca zapada!");
	i2c_send_stop();
	
	
	return 0;
}