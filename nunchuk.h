// Librería original del repositorio, escrita por Manuel CB

#ifndef NUNCHUK_H
#define NUNCHUK_H

#include <LPC407x_8x_177x_8x.h>
#include "tipos.h"
#include "i2c_lpc40xx.h"
#include "gpio_lpc40xx.h"
#include "timer_lpc40xx.h"

// Dirección I2C del nunchuk
#define NUNCHUK_DIRECCION 0xA4

bool_t nunchuk_inicializar();
bool_t nunchuk_leer(uint8_t* data);

#endif