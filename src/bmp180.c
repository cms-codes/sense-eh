#include "bmp180.h"

/********* _bmp_get_calib_data *******
*  Reads 22 bytes of factory recorded calibration data into a provided struct
*   Inputs: bmp_calib_data_t struct
*  Outputs: none
*/
void _bmp_get_calib_data(bmp_calib_data_t *calib_data)
{
	const uint8_t calib_addr = { BMP_CALIB_R_ADDR };
	uint8_t calib_raw[22] = {0};

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
	return;

}

/********* _bmp_get_temp_raw *******
*  Sets up clocks, pins, and peripherals.
*   Inputs: none
*  Outputs: Raw 16 bit ADC reading from temperature sensor.
*/
int16_t _bmp_get_temp_raw(void)
{
	uint8_t temp_parts[2] = {0};
	int16_t temp_raw = 0;
	const uint8_t temp_read_trig[2] = { BMP_TEMP_REQ_ADDR, BMP_TEMP_REQ_CMD };
	const uint8_t temp_read_addr = BMP_TEMP_READ_ADDR;

	i2c_transfer7( I2C1, BMP_ADDR, temp_read_trig, 2, NULL, 0 );
	systick_delay_ticks(6);
	i2c_transfer7( I2C1, BMP_ADDR, &temp_read_addr, 1, temp_parts, 2 );

	temp_raw = ( temp_parts[0] << 8 ) + temp_parts[1];
	return temp_raw;

}

/********* bmp_get_temp *******
*  Sets up clocks, pins, and peripherals.
*   Inputs: Uncalibrated temperature reading, device calibration struct.
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

	X1 = ( (temp_raw - calib_data->AC6) * ( calib_data->AC5 / ( pow(2,15) ) ) );
	X2 = ( ( calib_data->MC * ( pow(2,11) ) ) / (X1 + calib_data->MD) );
	B5 = X1 + X2;
	temp_cal = (B5 + 8) / (pow(2,4));
	
	return temp_cal / 10.0;

}