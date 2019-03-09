/*
 * Hackathlon_HA.c
 *
 * Created: 3/9/2019 9:30:36 AM
 *  Author: Ninja
 */ 

//#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>


#include "configuration.h"
#include "timer_handler.h"
#include "spi_handler.h"
#include "uart_handler.h"
#include "bme280/bme280.h"

#define MAIN_LOG_ACTIV (1)

#if MAIN_LOG_ACTIV
#define LOG_DEC(x,y) uart_send_string(x); uart_send_dec(y); uart_newline();
#define LOG_HEX(x,y) uart_send_string(x); uart_send_uhex(y); uart_newline();
#define LOG_STR(x,y) uart_send_string(x); uart_send_string(y); uart_newline();
#define LOG_CHR(x,y) uart_send_string(x); uart_send_char(y); uart_newline();
#else
#define LOG_DEC(x,y)
#define LOG_HEX(x,y)
#define LOG_STR(x,y)
#define LOG_CHR(x,y)
#endif  //MAIN_LOG_ACTIV

//#define INIT_STATUS_LED		(STATUS_LED_DDR |= (1 << STATUS_LED_PIN))
//#define TOGGLE_STATUS_LED	(STATUS_LED_PORT ^= (1 << STATUS_LED_PIN))
//#define STATUS_LED_ON		(STATUS_LED_PORT |= (1 << STATUS_LED_PIN))
//#define STATUS_LED_OFF		(STATUS_LED_PORT &= ~(1 << STATUS_LED_PIN))

struct bme280_dev bme280_interf;

void print_bme280_data(struct bme280_data *comp_data);

int main(void)
{
	int8_t rslt = BME280_OK;
	uint8_t u8state = 0;
	struct bme280_uncomp_data uncomp_data;
	struct bme280_data comp_data;
	
	DDRC = 0x01;
	
	cli();
	uart_init(MYUBRR);
	timer0_init(); //global timer init
	timer2_init(); //global timer init
	spi_init();
	sei();
	
	LOG_STR("Compiled on: ",__DATE__);
	LOG_STR("Compiled at: ",__TIME__);
	LOG_STR("Drivers initialized: ","UART & SPI");

	
	
	
	#if USE_BME280
	/* Sensor_0 interface over SPI with native chip select line */
	bme280_interf.dev_id = 0;
	bme280_interf.intf = BME280_SPI_INTF;
	bme280_interf.read = spi_transfer_sensors;
	bme280_interf.write = spi_transfer_sensors;
	bme280_interf.delay_ms = timer_delay_ms;
	rslt = bme280_init(&bme280_interf);
	
	LOG_DEC("BME280 sensor initialized with state: ",rslt);
	rslt = bme280_setup_weather_monitoring_meas(&bme280_interf);
	LOG_DEC("BME280 sensor setup with state: ",rslt);
	LOG_STR("Hardware initialized: ","BME280 Sensor");
	#endif //USE_BME280
	
    while(1) {
		u8state ^= 0x01;
		if (u8state) {
			PORTC |= 0x01;
		} else {
			PORTC &= ~0x01;
		}

		rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280_interf);  //trigger forced measurement
		LOG_DEC("BME280 sensor force mode trigger with state: ",rslt);
		bme280_interf.delay_ms(200);  //delay needed for measurement to complete
		//rslt = bme280_get_raw_sensor_data(BME280_ALL, &uncomp_data, &bme280_interf);
		//LOG_DEC("BME280 sensor RAW read with state: ",rslt);
		rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &bme280_interf);
		print_bme280_data(&comp_data);
		uart_send_string("BME280 sensor read with state: ");uart_send_dec(rslt);uart_newline();
		
		timer_delay_ms(2000);
    }
}

void print_bme280_data(struct bme280_data *comp_data)
{
	uart_send_string("Temperature: ");	uart_send_udec(comp_data->temperature);	uart_newline();
	uart_send_string("Pressure: ");	uart_send_udec(comp_data->pressure);uart_newline();
	uart_send_string("Humidity: ");	uart_send_udec(comp_data->humidity);uart_newline();
}