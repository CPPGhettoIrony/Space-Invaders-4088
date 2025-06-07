#include "sprites.h"

void dibujar_sprite(uint8_t* sprite, int32_t x, int32_t y, uint16_t color) {
	for(uint8_t i = 0; i < 8; ++i)
		for(uint8_t j = 0; j < 8; ++j)
			if(sprite[i] & (1 << j)) 
				dibujar_pixel(x + j, y + i, color);
}