// Librer�a original del repositorio, escrita por Manuel CB
#include "sprites.h"

/*
	Los sprites son punteros a arrays est�ticos de ocho bytes (unsigned char o uint8_t)
	Cada byte representa una l�nea, y cada bit de una l�nea indica si dibujar el p�xel en la coordenada adecuada o no
*/

void dibujar_sprite(uint8_t* sprite, int32_t x, int32_t y, uint16_t color) {
	
	// Se itera cada bit de cada byte
	
	for(uint8_t i = 0; i < 8; ++i)
		for(uint8_t j = 0; j < 8; ++j)
	
	// Si est� a uno, se dibuja el p�xel correspondiente
	
			if(sprite[i] & (1 << j)) 
				dibujar_pixel(x + j, y + i, color);
}