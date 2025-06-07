#ifndef JUGADOR_H
#define JUGADOR_H

#include "sprites.h"
#include "nunchuk.h"

#define altura 	120
#define max_x		144

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

#define n_enemigos 12

typedef struct{
	uint8_t*	sprites[2];
	int32_t		x, y;
	bool_t		vivo;
} enemigo;

enemigo enemigo_crear(int32_t x, int32_t y);

inline static void enemigo_dibujar(enemigo* e, uint8_t sprite) {
	dibujar_sprite(e->sprites[sprite], e->x, e->y, VERDE);
}

inline static void enemigo_borrar(enemigo* e, uint8_t sprite) {
	dibujar_sprite(e->sprites[sprite], e->x, e->y, NEGRO);
}

typedef struct {
	jugador		jugador;
	uint8_t		sprite;
} juego;

juego juego_inicializar();

void juego_actualizar(juego* j, uint8_t* nunchuk_data);

void juego_dibujar(juego* j);

void juego_borrar(juego* j);

void dibujar_panel();

#endif