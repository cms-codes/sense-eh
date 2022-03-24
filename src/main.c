#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/g0/syscfg.h>
#include <libopencm3/stm32/g0/usart.h>
#include <libopencm3/stm32/i2c.h>

#include "sys.h"
#include "systick.h"
#include "uart.h"
#include "i2c.h"


int _write( int file, char *ptr, int len )
{
	int i;

	if (file == 1) 
	{
		for ( i = 0; i < len; i++ )
		{
			if ( ptr[i] == '\n' )
			{
				Uart_send("\r", 1);
			}
			Uart_send( &ptr[i], 1);
		}
		return i;

	}
	errno = EIO;
	return -1;
}

int main(void)
{
	
	uint8_t callib_data[22] = {0};
	uint8_t callib_cmd = 0xAA;

	const uint8_t cmd[2] = { 0xf4, 0x2e };
	uint8_t read_cmd;
	uint8_t temp_raw[2] = {0};
	uint16_t temp = 0;

	int16_t AC1 = 0;
	int16_t AC2 = 0;
	int16_t AC3 = 0;
	uint16_t AC4 = 0;
	uint16_t AC5 = 0;
	uint16_t AC6 = 0;

	int16_t B1 = 0;
	int16_t B2 = 0;
	int16_t MB = 0;
	int16_t MC = 0;
	int16_t MD = 0;

	int32_t X1 = 0;
	int32_t X2 = 0;
	int32_t B5 = 0;

	int32_t temp_cal = 0;
	float temp_disp = 0.0f;

	Sys_init();
	Systick_init();

	Systick_delayTicks(500);
	printf("Hello, we're on the air...\n");

	_i2c_init();
	// get calibration data

	i2c_transfer7( I2C1, 0x77, &callib_cmd, 1, callib_data, 22 );
	Systick_delayTicks(25);

	AC1 = ((callib_data[0] << 8) | callib_data[1]);
	AC2 = ((callib_data[2] << 8) | callib_data[3]);
	AC3 = ((callib_data[4] << 8) | callib_data[5]);
	AC4 = ((callib_data[6] << 8) | callib_data[7]);
	AC5 = ((callib_data[8] << 8) | callib_data[9]);
	AC6 = ((callib_data[10] << 8) | callib_data[11]);
	B1 = ((callib_data[12] << 8) | callib_data[13]);
	B2 = ((callib_data[14] << 8) | callib_data[15]);
	MB = ((callib_data[16] << 8) | callib_data[17]);
	MC = ((callib_data[18] << 8) | callib_data[19]);
	MD = ((callib_data[20] << 8) | callib_data[21]);

	while (1)
	{

		Systick_delayTicks(500);

		
		i2c_transfer7( I2C1, 0x77, cmd, 2, NULL, 0 );
		gpio_toggle( PORT_LED, PIN_LED );
		Systick_delayTicks(25);

		read_cmd = 0xf6;
		i2c_transfer7( I2C1, 0x77, &read_cmd, 1, temp_raw, 2 );

		temp = ( temp_raw[0] << 8 ) + temp_raw[1];

		X1 = ( (temp - AC6) * ( AC5 / ( pow(2,15) ) ) );
		X2 = ( ( MC * ( pow(2,11) ) ) / (X1 + MD) );
		B5 = X1 + X2;
		temp_cal = (B5 + 8) / (pow(2,4));
		temp_disp = temp_cal / 10.0;

		printf( "Temp: %f\n", temp_disp );
		gpio_toggle( PORT_LED, PIN_LED );
		
	}
	
}
