#ifndef SYS_H_

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/g0/rcc.h>
#include <libopencm3/stm32/g0/syscfg.h>
#include <libopencm3/stm32/g0/nvic.h>
#include <libopencm3/stm32/g0/dma.h>

/* LED for debugging */
#define PORT_LED GPIOC
#define PIN_LED GPIO6

/* I2C - PORT A */
#define PORT_I2C1 GPIOA
#define PIN_I2C1_SDA GPIO10
#define PIN_I2C1_SCL GPIO9

/* UART2 - PORT A */
#define PORT_USART2 GPIOA
#define PIN_USART2_TX GPIO2
#define PIN_USART2_RX GPIO3

/* BMP180 I2C */
#define BMP_READ 0xEF
#define BMP_WRITE 0xEE
#define BMP_CMD_TEMP 0xF4

/********* Sys_init *******
*  Sets up clocks, pins, and peripherals.
*   Inputs: none
*  Outputs: none
*/
void Sys_init(void);

/********* rcc_init *******
*  Enables the clock for GPIO and other peripherals.
*   Inputs: none
*  Outputs: none
*/
void rcc_init(void);

/********* gpio_init *******
*  Initializes the GPIO registers and sets up peripheral and other interface pins.
*   Inputs: none
*  Outputs: none
*/
void gpio_init(void);

/********* nvic_init *******
*  Initilizes the Nested Vector Interrupt Controller.
*   Inputs: none
*  Outputs: none
*/
void nvic_init(void);


#define SYS_H_ 1
#endif
