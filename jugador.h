#ifndef JUGADOR_H
#define JUGADOR_H

#include "sprites.h"
#include "nunchuk.h"

#define altura 	120
#define max_x		230

typedef struct {
	uint8_t*	sprite;
	int32_t  	posicion;
} jugador;

jugador jugador_crear();

void jugador_actualizar(jugador* j, uint8_t* nunchuk_data);

inline static void jugador_dibujar(jugador* j) {
	dibujar_sprite(j->sprite, j->posicion, altura, BLANCO);
}

inline static void jugador_borrar(jugador* j) {
	dibujar_sprite(j->sprite, j->posicion, altura, NEGRO);
}

#endif