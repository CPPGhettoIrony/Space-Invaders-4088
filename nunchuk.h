#ifndef NUNCHUK_H
#define NUNCHUK_H

#include <LPC407x_8x_177x_8x.h>
#include "tipos.h"
#include "i2c_lpc40xx.h"
#include "gpio_lpc40xx.h"

#define NUNCHUK_DIRECCION 0x52

void nunchuk_inicializar();
void nunchuk_leer(uint8_t* data);

#endif