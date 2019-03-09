/*
 * Hackathlon_HA.c
 *
 * Created: 3/9/2019 9:30:36 AM
 *  Author: Ninja
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "configuration.h"
#include "timer_handler.h"
#include "spi_handler.h"
#include "uart_handler.h"
#include "bme280/bme280.h"

#define MAIN_LOG_ACTIV (0)

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



#define INIT_STATUS_LED		(STATUS_LED_DDR |= (1 << STATUS_LED_PIN))
#define TOGGLE_STATUS_LED	(STATUS_LED_PORT ^= (1 << STATUS_LED_PIN))
#define STATUS_LED_ON		(STATUS_LED_PORT |= (1 << STATUS_LED_PIN))
#define STATUS_LED_OFF		(STATUS_LED_PORT &= ~(1 << STATUS_LED_PIN))

typedef struct send_data {
	uint8_t temp_msb;
	uint8_t temp_lsb;
	uint8_t hum_msb;
	uint8_t hum_lsb;
	uint8_t pres_msb;
	uint8_t pres_lsb;
	} stSendData ;
	

struct bme280_dev bme280_interf;
extern uint32_t timer_system_ms;
void print_bme280_data(struct bme280_data *comp_data);

int main(void)
{
	int8_t rslt = BME280_OK;
	uint8_t u8state = 0;
	struct bme280_uncomp_data uncomp_data;
	struct bme280_data comp_data;
	stSendData SendData;
	uint32_t u32PresConv = 0;	
	INIT_STATUS_LED;
	DDRC |= (1<<4);
	
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
	
    while(1) 
	{
		/*LED toggle to confirm SW function*/
		//u8state ^= 0x01; if (u8state) { PORTC |= 0x01; } else {PORTC &= ~0x01; }
		//TOGGLE_STATUS_LED;
		
		#if MAIN_LOG_ACTIV
		if (1)
		#else
		/************************************************************************/
		/*                      Waiting for command from GC                     */
		/************************************************************************/
		if(uart_get_char() == 'X')
		#endif //MAIN_LOG_ACTIV
		{	
			PORTC ^= (1<<4);
			/************************************************************************/
			/*                          Reading sensor data                         */
			/************************************************************************/
			rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280_interf);  //trigger forced measurement
			LOG_DEC("BME280 sensor force mode trigger with state: ",rslt);
			bme280_interf.delay_ms(50);  //delay needed for measurement to complete
			rslt = bme280_get_raw_sensor_data(BME280_ALL, &uncomp_data, &bme280_interf);
			LOG_DEC("BME280 sensor RAW read with state: ",rslt);
			rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &bme280_interf);
			
			#if MAIN_LOG_ACTIV
			print_bme280_data(&comp_data);
			uart_send_string("BME280 sensor read with state: ");uart_send_dec(rslt);uart_newline();
			uart_send_udec(timer_system_ms/1000); uart_newline();
			#endif
			
			/************************************************************************/
			/*                      Data conversion for sending                     */
			/************************************************************************/
			SendData.temp_msb = (uint8_t)((comp_data.temperature)/100);
			SendData.temp_lsb = (uint8_t)((comp_data.temperature)%100);
			comp_data.humidity = comp_data.humidity / 10;
			SendData.hum_msb = (uint8_t)((comp_data.humidity)/100);
			SendData.hum_lsb = (uint8_t)((comp_data.humidity)%100);
			u32PresConv = (comp_data.pressure * 10000) / 101325;
			SendData.pres_msb = (uint8_t)((u32PresConv)/100);
			SendData.pres_lsb = (uint8_t)((u32PresConv)%100);

			/************************************************************************/
			/*                          Sending out data                            */
			/************************************************************************/			
			#if MAIN_LOG_ACTIV
			uart_send_string("Temperature: "); uart_send_udec(SendData.temp_msb); uart_send_char('.'); uart_send_udec(SendData.temp_lsb); uart_send_string(" *C");uart_newline();
			uart_send_string("Humidity: ");uart_send_udec(SendData.hum_msb);uart_send_char('.');uart_send_dec(SendData.hum_lsb);uart_send_string(" %");	uart_newline();
			uart_send_string("Pressure: ");uart_send_string("0.");uart_send_udec(SendData.pres_msb);uart_send_dec(SendData.pres_lsb);uart_send_string(" Atm");uart_newline();
			timer_delay_ms(2000);
			#else
			uart_send_char('T');uart_send_char(SendData.temp_msb);uart_send_char(SendData.temp_lsb);uart_send_char('A');
			uart_send_char('H');uart_send_char(SendData.hum_msb);uart_send_char(SendData.hum_lsb);uart_send_char('B');
			uart_send_char('P');uart_send_char(SendData.pres_msb);uart_send_char(SendData.pres_lsb);uart_send_char('C');
			timer_delay_ms(100);
			#endif //MAIN_LOG_ACTIV
		}
    }
}

void print_bme280_data(struct bme280_data *comp_data)
{
	uart_send_string("Temperature: ");	uart_send_udec(comp_data->temperature);	uart_newline();
	uart_send_string("Pressure: ");	uart_send_udec(comp_data->pressure);uart_newline();
	uart_send_string("Humidity: ");	uart_send_udec(comp_data->humidity);uart_newline();
}