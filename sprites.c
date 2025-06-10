// Librería original del repositorio, escrita por Manuel CB
#include "sprites.h"

/*
	Los sprites son punteros a arrays estáticos de ocho bytes (unsigned char o uint8_t)
	Cada byte representa una línea, y cada bit de una línea indica si dibujar el píxel en la coordenada adecuada o no
*/

void dibujar_sprite(uint8_t* sprite, int32_t x, int32_t y, uint16_t color) {
	
	// Se itera cada bit de cada byte
	
	for(uint8_t i = 0; i < 8; ++i)
		for(uint8_t j = 0; j < 8; ++j)
	
	// Si está a uno, se dibuja el píxel correspondiente
	
			if(sprite[i] & (1 << j)) 
				dibujar_pixel(x + j, y + i, color);
}