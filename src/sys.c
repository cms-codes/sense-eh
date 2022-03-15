#include "sys.h"

/********* Sys_init *******
*  Sets up clocks, pins, and peripherals.
*   Inputs: none
*  Outputs: none
*/
void Sys_init(void) {
	rcc_init();
	gpio_init();
	//nvic_init();

}

/********* rcc_init *******
*  Enables the clock for GPIO and other peripherals.
*   Inputs: none
*  Outputs: none
*/
void rcc_init(void) {

	

	rcc_clock_setup(&rcc_clock_config[RCC_CLOCK_CONFIG_HSI_16MHZ]);

	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOC);

	rcc_periph_clock_enable(RCC_DMA1);
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_USART2);
	

}

/********* gpio_init *******
*  Initializes the GPIO registers and sets up peripheral and other output pins.
*   Inputs: none
*  Outputs: none
*/
void gpio_init(void) {

	/* USART peripheral for serial debug (via virtual COM port /tty/ACMx) */	
	gpio_mode_setup( PORT_USART2, GPIO_MODE_AF, GPIO_PUPD_NONE, PIN_USART2_TX | PIN_USART2_RX );
	gpio_set_af( PORT_USART2, GPIO_AF1, PIN_USART2_TX | PIN_USART2_RX );
	gpio_set_output_options( PORT_USART2, GPIO_OTYPE_PP, GPIO_OSPEED_MED, PIN_USART2_TX | PIN_USART2_RX );

	/* I2C1 peripheral */
	gpio_mode_setup( PORT_I2C1, GPIO_MODE_AF, GPIO_PUPD_NONE, PIN_I2C1_SDA | PIN_I2C1_SCL );
	gpio_set_af( PORT_I2C1, GPIO_AF6, PIN_I2C1_SDA | PIN_I2C1_SCL );
	gpio_set_output_options( PORT_I2C1, GPIO_OTYPE_OD, GPIO_OSPEED_MED, PIN_I2C1_SDA | PIN_I2C1_SCL );

	/* LED for debugging purposes */
	gpio_mode_setup( PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED );
	gpio_set_output_options( PORT_LED, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, PIN_LED );

}

// ******* nvic_init *******
// Initializes the Nested Vector Interrupt Controller.
//  Inputs: none
// Outputs: none
void nvic_init(void) 
{

	nvic_set_priority( NVIC_DMA1_CHANNEL2_3_IRQ, 2);
	nvic_enable_irq( NVIC_DMA1_CHANNEL2_3_IRQ );

	// nvic_set_priority( NVIC_USART2_IRQ, 0);
	// nvic_enable_irq( NVIC_USART2_IRQ );

}

// ******* dma1_channel2_3_isr *******
// ISR function for DMA channels 2 & 3. Clears interrupt flags and signals that 
// transfer has completed.
//  Inputs: none
// Outputs: none
void dma1_channel2_3_isr(void)
{

	uint32_t isr = DMA1_ISR;
	// Channel 3 is USART2_TX
	if ( isr & DMA_ISR_TCIF3 )
	{
		// clear interrupt flags
		DMA1_IFCR |= DMA_IFCR_CGIF3;
		
		
	}
	
}

void usart2_isr(void)
{

}
