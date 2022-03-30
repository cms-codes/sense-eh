#include <errno.h>
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
#include "bmp180.h"


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
	
	bmp_calib_data_t calib_data =
	{
		.AC1 = 0,
		.AC2 = 0,
		.AC3 = 0,
		.AC4 = 0,
		.AC5 = 0,
		.AC6 = 0,

		.B1 = 0,
		.B2 = 0,
		.MB = 0,
		.MC = 0,
		.MD = 0,
	};
	//float temp = 0.0f;
	int32_t press = 0;

	Sys_init();
	systick_init();
	_i2c_init();

	systick_delay_ticks(500);
	printf("Hello, we're on the air...\n");

	_bmp_get_calib_data(&calib_data);

	while (1)
	{

		systick_delay_ticks(500);
		
		// must calibrate before each pressure reading
		_bmp_get_calib_data(&calib_data);
		press = bmp_get_press(&calib_data, 0);
		
		gpio_toggle( PORT_LED, PIN_LED );
		systick_delay_ticks(25);

		printf( "Press: %ld\n", press );
		gpio_toggle( PORT_LED, PIN_LED );
		
	}
	
}
