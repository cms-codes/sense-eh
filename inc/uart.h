#ifndef UART_H


#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/g0/usart.h>
#include <libopencm3/stm32/g0/dma.h>
#include <libopencm3/stm32/g0/dmamux.h>
#include <libopencm3/stm32/g0/nvic.h>

#include "sys.h"

void _uart_init(void);

void Uart_send( volatile void* data, int length );

#define UART_H 1
#endif
