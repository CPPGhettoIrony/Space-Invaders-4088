#ifndef JUGADOR_H
#define JUGADOR_H

#include "sprites.h"
#include "nunchuk.h"
#include "sonido.h"

#define altura 	120
#define max_x		168

typedef struct {
	uint8_t		*sprite, *spr_bala;
	int32_t  	posicion, bala_x, bala_y;
	bool_t		bala;
} jugador;

jugador jugador_crear();

void jugador_actualizar(jugador* j, uint8_t* nunchuk_data);

inline static void jugador_dibujar(jugador* j) {
	dibujar_sprite(j->sprite, j->posicion, altura, BLANCO);
}

inline static void jugador_borrar(jugador* j) {
	dibujar_sprite(j->sprite, j->posicion, altura, NEGRO);
}

inline static void bala_dibujar(jugador* j) {
	dibujar_sprite(j->spr_bala, j->bala_x, j->bala_y, BLANCO);
}

inline static void bala_borrar(jugador* j) {
	dibujar_sprite(j->spr_bala, j->bala_x, j->bala_y, NEGRO);
}

#define n_enem_linea	12
#define n_enemigos 		48

typedef struct{
	uint8_t*	sprites[2];
	int32_t		x, y;
	bool_t		vivo;
	int32_t		bala_x, bala_y;
	bool_t		bala;
} enemigo;

enemigo enemigo_crear(int32_t x, int32_t y);

inline static void enemigo_dibujar(enemigo* e, uint8_t sprite, int32_t offset_x, int32_t offset_y) {
	dibujar_sprite(e->sprites[sprite], e->x + offset_x, e->y + offset_y, VERDE);
}

inline static void enemigo_borrar(enemigo* e, uint8_t sprite, int32_t offset_x, int32_t offset_y) {
	dibujar_sprite(e->sprites[sprite], e->x + offset_x, e->y + offset_y, NEGRO);
}

typedef struct {
	uint8_t		nivel;
	uint8_t		vidas;
	uint32_t	puntuacion;
	jugador		jugador;
	uint8_t		sprite;
	uint8_t		enemigos;
	int32_t		x, y, direccion;
	int8_t		offset_x_a, offset_x_b, offset_y;
	uint32_t  periodo;
	bool_t		game_over;
	uint8_t		balas, max_balas;
} juego;

juego juego_inicializar();

void juego_actualizar(juego* j, uint8_t* nunchuk_data);

void juego_dibujar(juego* j);

void juego_borrar(juego* j);

void juego_resetear(juego* j);

void secuencia_inicial();

void dibujar_panel();

inline static void sonido_enemigo_bala() {
	sonido_emitir_pitido(900,   8);
	sonido_emitir_pitido(1000, 10);
	sonido_emitir_pitido(1200, 15);
}

inline static void sonido_jugador_bala() {
	sonido_emitir_pitido(600, 15);
	sonido_emitir_pitido(550, 10);
	sonido_emitir_pitido(450, 8);
}

inline static void sonido_perder_vida() {
	sonido_emitir_pitido(NOTE_C3,  100);
	sonido_emitir_pitido(NOTE_G2,  100);
	sonido_emitir_pitido(NOTE_FS2, 100);	
}

inline static void sonido_avanzar_nivel() {
	sonido_emitir_pitido(NOTE_A5,	 110);
	sonido_emitir_pitido(NOTE_F5,	 110);
	sonido_emitir_pitido(NOTE_C6,	 110);
	sonido_emitir_pitido(NOTE_F6,	 110);
}

inline static void sonido_game_over() {
	
	sonido_emitir_pitido(NOTE_C3,  250);
	timer_retardo_ms(TIMER0, 50);
	
	sonido_emitir_pitido(NOTE_C3,  300);
	sonido_emitir_pitido(NOTE_C3,  100);
	
	sonido_emitir_pitido(NOTE_C3,  250);
	timer_retardo_ms(TIMER0, 50);
	
	sonido_emitir_pitido(NOTE_DS3, 200);
	sonido_emitir_pitido(NOTE_D3,  100);
	
	sonido_emitir_pitido(NOTE_D3,  200);
	sonido_emitir_pitido(NOTE_C3,  100);
	
	sonido_emitir_pitido(NOTE_C3,  200);
	sonido_emitir_pitido(NOTE_B2,  100);
	
	sonido_emitir_pitido(NOTE_C3,  300);

}

#endif