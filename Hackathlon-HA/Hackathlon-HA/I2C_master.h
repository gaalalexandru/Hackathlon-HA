/*

 * TWI (aka I2C) implementation of Master Transmit mode. Based on AVR155.

 *

 * This library only implements as much as is needed to send data to a PCF8574

 * 8-bit I/O expander.

 *

 * Target:          ATmega8

 *

 * Original Author: Asmund Saetre

 * License:         Unknown

 */

#ifndef TWI_MASTER_H

#define TWI_MASTER_H

#include <avr/io.h>


/****************************************************************************

TWI configuration

****************************************************************************/


#define PCF8574_ADDR    0x4E            // Slave address of PCF8574

#define SCL_CLOCK         72            // Set SCL to 100 KHz at 3.6 MHz xtal

                                        // See application note AVR315, table 1

/****************************************************************************

TWI status register definitions

****************************************************************************/

/* General Master status codes */

#define START           0x08            // START has been transmitted

#define REP_START       0x10            // Repeated START has been transmitted


/* Master Transmitter status codes */

#define MTX_ADR_ACK     0x18            // SLA+W has been transmitted and ACK received

#define MTX_DATA_ACK    0x28            // Data byte has been transmitted and ACK received


/* Other TWI related settings */

#define SUCCESS         0xFF            // return code for success


/****************************************************************************

TWI functions

****************************************************************************/

void i2c_init(void);


unsigned char i2c_send_start(void);


void i2c_send_stop(void);


unsigned char i2c_send_adr(unsigned char adr);


unsigned char i2c_write(unsigned char data);

#endif
