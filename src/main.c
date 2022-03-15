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

int main(void)
{
	
	const uint8_t cmd[2] = { 0xf4, 0x2e };
	uint8_t read_cmd;
	uint8_t temp_raw[2] = {0};

	Sys_init();
	Systick_init();



	while (1)
	{

		Systick_delayTicks(500);

		_i2c_init();
		i2c_transfer7(I2C1, 0x77, &cmd[0], 2, NULL, 0);
		gpio_toggle( PORT_LED, PIN_LED );
		Systick_delayTicks(25);

		read_cmd = 0xf6;
		i2c_transfer7(I2C1, 0x77, &read_cmd, 1, &temp_raw[1], 2);	

		Uart_send( (void*)&temp_raw[0], 2 );
		gpio_toggle( PORT_LED, PIN_LED );
		
	}
	
}
