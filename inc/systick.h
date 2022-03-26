#ifndef SYSTICK_H_

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/g0/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/cortex.h>
#include <stdint.h>

/********* systick_init *******
*  Initializes the SysTick interrupt timer.
*/
void systick_init(void);

/********* _systick_time_get_count *******
*  Returns the elapsed program time based on the count of SysTick interrupts.
*   Inputs: none
*  Outputs: A count of elapsed cycles.
*/
uint32_t _systick_time_get_count(void);

/********* _systick_time_delta *******
*  Provides the difference between two program times
*   Inputs: A start time and an end time.
*  Outputs: The delta between two points in execution time.
*/
uint32_t _systick_time_delta( uint32_t start, uint32_t end );

/********* systick_delay_ticks *******
*  Delays execution for x interrupts
*   Inputs: Number of ticks to delay execution.
*  Outputs: none
*/
void systick_delay_ticks( uint32_t wait_ticks );


#define SYSTICK_H_ 1
#endif