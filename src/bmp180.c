#include "bmp180.h"

/********* _bmp_get_calib_data *******
*  Reads 22 bytes of factory recorded calibration data into the provided struct;
*  see p. 13 of the BMP180 datasheet (April 2013).
*   Inputs: bmp_calib_data_t struct
*  Outputs: none
*/
void _bmp_get_calib_data(bmp_calib_data_t *calib_data)
{
	const uint8_t calib_addr = { BMP_CALIB_R_ADDR };
	uint8_t calib_raw[22] = {0};

	int16_t temp_raw = 0;
	int32_t X1 = 0;
	int32_t X2 = 0;
	int32_t B5 = 0;

	i2c_transfer7( I2C1, BMP_ADDR, &calib_addr, 1, calib_raw, 22 );
	systick_delay_ticks(6);

	calib_data->AC1 = ((calib_raw[0] << 8) | calib_raw[1]);
	calib_data->AC2 = ((calib_raw[2] << 8) | calib_raw[3]);
	calib_data->AC3 = ((calib_raw[4] << 8) | calib_raw[5]);
	calib_data->AC4 = ((calib_raw[6] << 8) | calib_raw[7]);
	calib_data->AC5 = ((calib_raw[8] << 8) | calib_raw[9]);
	calib_data->AC6 = ((calib_raw[10] << 8) | calib_raw[11]);
	calib_data->B1 = ((calib_raw[12] << 8) | calib_raw[13]);
	calib_data->B2 = ((calib_raw[14] << 8) | calib_raw[15]);
	calib_data->MB = ((calib_raw[16] << 8) | calib_raw[17]);
	calib_data->MC = ((calib_raw[18] << 8) | calib_raw[19]);
	calib_data->MD = ((calib_raw[20] << 8) | calib_raw[21]);

	temp_raw = _bmp_get_temp_raw();
	X1 = ( (temp_raw - calib_data->AC6) * ( calib_data->AC5 / (pow(2,15)) ) );
	X2 = ( ( calib_data->MC * (pow(2,11)) ) / (X1 + calib_data->MD) );
	B5 = X1 + X2;
	// Is pressure reading temperature compensated? (see pressure calib. coefficient B6)
	// This calib. function will need to run before each pressure reading.
	calib_data->B5 = B5;

	return;

}

/********* _bmp_get_temp_raw *******
*  Reads the uncalibrated temperature.
*   Inputs: none
*  Outputs: Raw 16 bit ADC reading from sensor.
* 
*  0xf6: adc_out_msb
*  0xf7: adc_out_lsb
*  0x68: adc_out_xlsb
* 
*/
int16_t _bmp_get_temp_raw(void)
{
	uint8_t temp_parts[2] = {0};
	int16_t temp_raw = 0;
	const uint8_t temp_read_trig[2] = { BMP_READ_REQ_ADDR, BMP_TEMP_REQ_CMD };
	const uint8_t temp_read_addr = BMP_READ_ADDR;

	// initiate a reading of the temperature sensor
	i2c_transfer7( I2C1, BMP_ADDR, temp_read_trig, 2, NULL, 0 );
	// this delay is dependent on the oversampling mode uses, as is BMP_TEMP_READ_LEN
	systick_delay_ticks(6);
	// fetch the resulting reading
	i2c_transfer7( I2C1, BMP_ADDR, &temp_read_addr, 1, temp_parts, BMP_TEMP_READ_LEN );

	temp_raw = ( temp_parts[0] << 8 ) + temp_parts[1];
	return temp_raw;

}

/********* bmp_get_temp *******
*  Calibrated temperature reading (public function)
*   Inputs: Uncalibrated temperature reading, device calibration struct. See p. 15 of
*           the BMP180 datasheet (April 2013).
*  Outputs: Calibrated temperature.
*/
float bmp_get_temp(const bmp_calib_data_t *calib_data)
{
	int16_t temp_raw = 0;
	int32_t temp_cal = 0;

	int32_t X1 = 0;
	int32_t X2 = 0;
	int32_t B5 = 0;
	
	temp_raw = _bmp_get_temp_raw();

	X1 = ( (temp_raw - calib_data->AC6) * ( calib_data->AC5 / (pow(2,15)) ) );
	X2 = ( ( calib_data->MC * (pow(2,11)) ) / (X1 + calib_data->MD) );
	B5 = X1 + X2;
	temp_cal = (B5 + 8) / (pow(2,4));
	
	return temp_cal / 10.0;

}

/********* _bmp_get_press_raw *******
*  Uncalibrated pressure reading.
*   Inputs: hardware oversampling (0 = 1 sample @ 4.5ms; 1 = 2 samples @ 7.5ms;
*           2 = 4 @ 13.5ms; 3 = 8 @ 25.5ms)
*  Outputs: Raw 19 bit ADC reading from sensor memory.
*/
int32_t _bmp_get_press_raw(int oss)
{
	uint8_t press_parts[3] = {0};
	int32_t press_raw = 0;
	const uint8_t press_read_trig[2] = { BMP_READ_REQ_ADDR, BMP_PRESS_REQ_CMD + ( oss << 6 ) };
	const uint8_t press_read_addr = BMP_READ_ADDR;

	i2c_transfer7( I2C1, BMP_ADDR, press_read_trig, 2, NULL, 0 );
	switch (oss)
	{
			case(0):
				systick_delay_ticks(8);
				break;
			case(1):
				systick_delay_ticks(11);
				break;
			case(2):
				systick_delay_ticks(18);
				break;
			case(3):
				systick_delay_ticks(30);
				break;
	}
	i2c_transfer7( I2C1, BMP_ADDR, &press_read_addr, 1, press_parts, BMP_PRESS_READ_LEN );
	press_raw = ( ( press_parts[0] << 16 ) + (press_parts[1] << 8) + press_parts[2] )
				 >> ( 8 - oss );
	return press_raw;

}

/********* bmp_get_press *******
*  Calibrated presserature reading (public function)
*   Inputs: Uncalibrated presserature reading, device calibration struct. See p. 15 of
*           the BMP180 datasheet (April 2013), hardware oversampling oss
*           (see _bmp_get_press_raw())
*  Outputs: Calibrated absolute pressure in pascals.
*/
int32_t bmp_get_press(const bmp_calib_data_t *calib_data, const int oss)
{
	int32_t press_raw = 0;
	int32_t press = 0;

	int32_t B6 = 0;
	int32_t X1 = 0;
	int32_t X2 = 0;
	int32_t X3 = 0;
	int32_t B3 = 0;
	uint32_t B4 = 0;
	uint32_t B7 = 0;

	press_raw = _bmp_get_press_raw(oss);

	B6 = calib_data->B5 - 4000;
	X1 = ( calib_data->B2 * ( B6 * B6 / pow(2, 12) ) ) / pow(2, 11);
	X2 = calib_data->AC2 * B6 / pow(2, 11);
	X3 = X1 + X2;

	B3 = (((calib_data->AC1 * 4 + X3) << oss) + 2) / 4;
	X1 = calib_data->AC3 * B6 / pow(2, 13);
	X2 = ( calib_data->B1 * ( B6 * B6 / pow(2, 12) ) ) / pow(2, 16);
	X3 = ((X1 + X2) + 2) / pow(2, 2);

	B4 = calib_data->AC4 * (uint32_t)(X3 + 32768) / pow(2, 15);
	B7 = ((uint32_t)press_raw - B3) * (50000 >> oss);
	if (B7 < 0x80000000UL)
	{
		press = (B7 * 2) / B4;
	}
	else
	{
		press = (B7 / B4) * 2;
	}

	X1 = (press / pow(2, 8)) * (press / pow(2, 8));
	X1 = (X1 * 3038) / pow(2, 16);
	X2 = (-7357 * press) / pow(2, 16);
	press = press + (X1 + X2 + 3791) / pow(2, 4);

	// Absolute pressure (relative to sea level)
	return press; // return in Pa

}