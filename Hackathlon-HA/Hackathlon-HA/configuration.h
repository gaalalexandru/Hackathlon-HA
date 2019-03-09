/*
 * configuration.h
 *
 * Created: 8/3/2018 11:52:44 PM
 *  Author: alexandru.gaal
 */ 


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <avr/io.h>
#include <avr/portpins.h>

//#define MAIN_LOG_ACTIV

//////////////////////////////////////////////////////////////////////////
//					CONTROLLER SELECTION AND CONFIGURATION				//
//////////////////////////////////////////////////////////////////////////

#define ATMEGA48 (1)
#define ATMEGA8	(0)

//#error "Please specify one target controller"
//#endif  //controller selection
#define FOSC 16000000// Clock Speed
#define F_CPU FOSC

//////////////////////////////////////////////////////////////////////////
//							STATUS LED CONFIGURATIONS					//
//////////////////////////////////////////////////////////////////////////
//#define STATUS_LED_DDR		DDRD
//#define STATUS_LED_PORT		PORTD
//#define STATUS_LED_PIN		PIN4


//////////////////////////////////////////////////////////////////////////
//							UART CONFIGURATIONS							//
//////////////////////////////////////////////////////////////////////////
#define BAUD 38400 //38400 // Old value only for terminal control: 9600
#define SET_U2X (1)
#define DONT_USE_ISR_UART (0)
#if SET_U2X
#define MYUBRR (uint32_t)((FOSC/(8*BAUD))-1)
#else
#define MYUBRR (uint32_t)((FOSC/(16*BAUD))-1)
#endif

#define USE_UART_DEC_OUTPUT
#define USE_UART_HEX_OUTPUT
//////////////////////////////////////////////////////////////////////////
//							 SPI CONFIGURATIONS							//
//////////////////////////////////////////////////////////////////////////
#define SPI_USE_INT		(0)
#define SPI_DDR			DDRB
#define SPI_PORT		PORTB
#define SPI_MOSI_PIN	(3)
#define SPI_SS_PIN		(2)
#define SPI_MISO_PIN	(4)
#define SPI_SCK_PIN		(5)

#define CS_BME280_DDR	DDRB
#define CS_BME280_PORT	PORTB
#define CS_BME280_PIN	(1)

//////////////////////////////////////////////////////////////////////////
//						 MEASUREMENTS CONFIGURATION						//
//////////////////////////////////////////////////////////////////////////
#define USE_BME280		(1)
#define WEATHER_MONITORING_INTERVAL_MS (60000)
#define WEATHER_MONITORING_ACCELERATION (10)
#define DATA_FRAME_SIZE	(64)
#define DATA_FRAME_SEPARATOR (0x55)

#endif /* CONFIGURATION_H_ */