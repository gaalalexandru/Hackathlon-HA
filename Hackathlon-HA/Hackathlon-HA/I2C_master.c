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

#include "twi_master.h"


/****************************************************************************

Internal functions

****************************************************************************/


static void Wait_TWI_int(void)

{

    while (!(TWCR & (1<<TWINT)))            // Wait for TWI interrupt flag set

        ;

}


/****************************************************************************

Public functions

****************************************************************************/


void i2c_init(void)

{

    TWBR = SCL_CLOCK;                       // Set SCL frequency

    TWCR = (1<<TWEN);

}


unsigned char i2c_write(unsigned char data)

{

    Wait_TWI_int();


    TWDR = data;

    TWCR = ((1<<TWINT)+(1<<TWEN));          // Clear int flag to send byte


    Wait_TWI_int();


    if(TWSR != MTX_DATA_ACK)                // If NACK received return TWSR

        return TWSR;

    return SUCCESS;

}


unsigned char i2c_send_start(void)

{

    TWCR = ((1<<TWINT)+(1<<TWSTA)+(1<<TWEN));// Send START


    Wait_TWI_int();


    if((TWSR != START)&&(TWSR != REP_START))// If status other than START

        return TWSR;                        // transmitted (0x08) or Repeated

    return SUCCESS;                         // START transmitted (0x10)

                                            // -> error and return TWSR.

                                            // If success return SUCCESS.

}


void i2c_send_stop(void)

{

    TWCR = ((1<<TWEN)+(1<<TWINT)+(1<<TWSTO));// Send STOP condition

}


unsigned char i2c_send_adr(unsigned char adr)

{

    Wait_TWI_int();


    TWDR = adr;

    TWCR = ((1<<TWINT)+(1<<TWEN));          // Clear int flag to send byte


    Wait_TWI_int();


    if (TWSR != MTX_ADR_ACK)                // If NACK received return TWSR

        return TWSR;

    return SUCCESS;                         // Else return SUCCESS

}