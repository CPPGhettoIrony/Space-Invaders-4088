#ifndef SPRITES_H
#define SPRITES_H

#include "glcd.h"

#define TAM_PIXEL 2

inline static void dibujar_pixel(int32_t x, int32_t y, uint16_t color) {
	glcd_rectangulo_relleno(x * TAM_PIXEL, y * TAM_PIXEL, (x+1) * TAM_PIXEL, (y+1) * TAM_PIXEL, color);
}

void dibujar_sprite(uint8_t* sprite, int32_t x, int32_t y, uint16_t color);

#endif