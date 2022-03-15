#include "i2c.h"

void _i2c_init(void)
{
	i2c_reset(I2C1);
	i2c_peripheral_disable(I2C1);

	i2c_enable_analog_filter(I2C1);

	i2c_set_speed(I2C1, i2c_speed_sm_100k, 16);
	i2c_set_7bit_addr_mode(I2C1);
	i2c_peripheral_enable(I2C1);



}