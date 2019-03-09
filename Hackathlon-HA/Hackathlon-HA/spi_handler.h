/*
 * spi_handler.h
 *
 * Created: 8/4/2018 12:15:48 AM
 *  Author: alexandru.gaal
 */ 


#ifndef SPI_HANDLER_H_
#define SPI_HANDLER_H_

#define INIT_CS_PIN(x,y)	(x) |=  (1<<(y))
#define SET_CS_PIN(x,y)	(x) |=(1<<(y))
#define CLEAR_CS_PIN(x,y)	(x) &= ~(1<<(y))

#define DISABLE_CS_BME280	SET_CS_PIN(CS_BME280_PORT,CS_BME280_PIN)
#define ENABLE_CS_BME280	CLEAR_CS_PIN(CS_BME280_PORT,CS_BME280_PIN)

void spi_init(void);
uint8_t spi_transfer_generic(uint8_t u8data);
void spi_transfer_sensors(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

#endif /* SPI_HANDLER_H_ */