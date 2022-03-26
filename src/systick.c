#include "systick.h"

volatile uint32_t systickCount = 0;

/********* systick_init *******
*  Initializes the SysTick interrupt timer.
*/
void systick_init(void)
{
	// 48MHz int osc
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);

	// 48000000/100000 = 480 overflows per second - every 10 microseconds equals
	// one interrupt.
	// SysTick interrupt every N clock pulses: set reload to N-1 
	systick_set_reload(47900);
	systick_interrupt_enable();
	// Start counting
	systick_counter_enable();
}

/********* _systick_time_get_count *******
*  Returns the elapsed program time based on the count of SysTick interrupts.
*   Inputs: none
*  Outputs: A count of elapsed cycles.
*/
uint32_t _systick_time_get_count(void)
{
    return systickCount;
}

/********* _systick_time_delta *******
*  Provides the difference between two program times
*   Inputs: A start time and an end time.
*  Outputs: The delta between two points in execution time.
*/
uint32_t _systick_time_delta( uint32_t start, uint32_t end )
{
    uint32_t diff;

    // Detect if roll-over has occurred and calculate accordingly.
    if ( end > start )
    {
        diff = end - start;
    }
    else
    {
        diff = UINT32_MAX - ( start - end ) + 1;
    }

    return diff;
}

/********* systick_delay_ticks *******
*  Delays execution for x interrupts
*   Inputs: Number of ticks to delay execution.
*  Outputs: none
*/
void systick_delay_ticks( uint32_t wait_ticks )
{
	volatile uint32_t start, now, diff;

	start = _systick_time_get_count();

	do
	{
		__asm__ __volatile__ ("nop");
		now = _systick_time_get_count();
		
        diff = _systick_time_delta( start, now );
	}
	while ( diff < wait_ticks );
}

/********* sys_tick_handler *******
*  Predefined SysTick ISR function with libopencm3 prototype. Runs the event
*  manager and increments the counter.
*/
void sys_tick_handler(void)
{
	//Sched_runEventManager();

	// Roll over to 0 at UINT32_MAX
	if ( systickCount < UINT32_MAX )
	{
		systickCount++;
	}
	else
	{
		systickCount = 0;
	}

}