#ifndef BMP180_H

#include <math.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/g0/i2c.h>
#include <libopencm3/stm32/g0/dma.h>
#include <libopencm3/stm32/g0/dmamux.h>
#include <libopencm3/stm32/g0/nvic.h>

#include "systick.h"
#include "uart.h"

#define BMP_ADDR 0x77
#define BMP_TEMP_REQ_ADDR 0xF4
#define BMP_TEMP_REQ_CMD 0x2E
#define BMP_TEMP_READ_ADDR 0xF6
#define BMP_TEMP_READ_LEN 2
#define BMP_CALIB_R_ADDR 0xAA
#define BMP_CALIB_READ_LEN 22 

typedef struct bmp_calib_data bmp_calib_data_t;
struct bmp_calib_data
{
	int16_t AC1;
	int16_t AC2;
	int16_t AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;

	int16_t B1;
	int16_t B2;
	int16_t MB;
	int16_t MC;
	int16_t MD;

	int32_t X1;
	int32_t X2;
	int32_t B5;
};                                                                                                               


/********* _bmp_get_calib_data *******
*  Reads 22 bytes of factory recorded calibration data into the provided struct
*   Inputs: bmp_calib_data_t struct
*  Outputs: none
*/
void _bmp_get_calib_data(bmp_calib_data_t *calib_data);

/********* _bmp_get_temp_raw *******
*  Sets up clocks, pins, and peripherals.
*   Inputs: none
*  Outputs: Raw 16 bit ADC reading from temperature sensor.
*/
int16_t _bmp_get_temp_raw(void);

/********* bmp_get_temp *******
*  Sets up clocks, pins, and peripherals.
*   Inputs: Uncalibrated temperature reading, device calibration struct. See p. 15 of
*           the BMP180 datasheet (April 2013).
*  Outputs: Calibrated temperature.
*/
float bmp_get_temp(const bmp_calib_data_t *calib_data);
	

#define BMP180_H 1
#endif
