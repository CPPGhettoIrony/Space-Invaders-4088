// Librería original del repositorio, escrita por Manuel CB
#ifndef SPRITES_H
#define SPRITES_H

#include "glcd.h"

// El tamaño en píxeles reales de cada píxel de los sprites a dibujar
#define TAM_PIXEL 2

// Se utilizará esta función para dibujar los píxeles de cada sprite
inline static void dibujar_pixel(int32_t x, int32_t y, uint16_t color) {
	// Cada unidad se dibujará como un cuadrado relleno con un tamaño TAM_PIXEL x TAM_PIXEL
	glcd_rectangulo_relleno(x * TAM_PIXEL, y * TAM_PIXEL, (x+1) * TAM_PIXEL, (y+1) * TAM_PIXEL, color);
}

// Dibujar el sprite en la pantalla
void dibujar_sprite(uint8_t* sprite, int32_t x, int32_t y, uint16_t color);

#endif