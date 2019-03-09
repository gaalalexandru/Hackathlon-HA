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

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_write 0x4E

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
	lcd.init();                      // initialize the lcd
	lcd.backlight();
}

void loop()
{
	// when characters arrive over the serial port...
		// wait a bit for the entire message to arrive
		// clear the screen
		lcd.clear();
		// read all the available characters
			lcd.init(LCD_write);
			lcd.write("Nu!");
		}
	}
}