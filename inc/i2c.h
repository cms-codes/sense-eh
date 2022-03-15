#ifndef I2C_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/g0/i2c.h>
#include <libopencm3/stm32/g0/dma.h>
#include <libopencm3/stm32/g0/dmamux.h>
#include <libopencm3/stm32/g0/nvic.h>

#include "sys.h"

void _i2c_init(void);

#define I2C_H 1
#endif

