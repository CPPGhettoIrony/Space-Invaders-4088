#include "jugador.h"

uint8_t sprite_jugador[8] = {
	0b00011000,
	0b00011000,
	0b00111100,
	0b00111100,
	0b01100110,
	0b01000010,
	0b11000011,
	0b11111111
};

jugador jugador_crear() {
	jugador ret = {sprite_jugador, 3};
	return ret;
}

void jugador_actualizar(jugador* j, uint8_t* nunchuk_data) {
	
	int8_t	joystick	= (int8_t)nunchuk_data[0] + 120;
	bool_t	boton_c 	= !(nunchuk_data[5] & 2u),
					boton_z 	= !(nunchuk_data[5] & 1u);
	
	int8_t	direccion = (joystick > 30) - (joystick < -30);
	
	j->posicion += direccion * 2;
	j->posicion = j->posicion * (j->posicion > 2 && j->posicion < max_x) + 3 * (j->posicion <=2) + (max_x - 1) * (j->posicion >= max_x);
	
}

uint8_t sprite_enemigo_a[8] = {
	0b00111100,
	0b01111110,
	0b01011010,
	0b01011010,
	0b01111110,
	0b01100110,
	0b10000001,
	0b01000010
};

uint8_t sprite_enemigo_b[8] = {
	0b00111100,
	0b01111110,
	0b01011010,
	0b01011010,
	0b01111110,
	0b00101000,
	0b01000010,
	0b11000011
};

enemigo enemigos[n_enemigos];

enemigo enemigo_crear(int32_t x, int32_t y) {
	
	enemigo ret = {
		{sprite_enemigo_a, sprite_enemigo_b}, 
		x, y,
		TRUE
	};
	
	return ret;
}

juego juego_inicializar() {
	
	jugador j = jugador_crear();
	
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		enemigos[i] = enemigo_crear(4 + i*12, 4);
	
	juego ret = {
		j, 0
	};
	
	return ret;
}

void juego_actualizar(juego* j, uint8_t* nunchuk_data) {
	jugador_actualizar(&j->jugador, nunchuk_data);
}

void juego_dibujar(juego* j) {
	
	jugador_dibujar(&j->jugador);
	
	for(uint8_t i = 0; i < n_enemigos; ++i)
		enemigo_dibujar(&enemigos[i], j->sprite);
	
}

void juego_borrar(juego* j) {
	
	jugador_borrar(&j->jugador);
	
	for(uint8_t i = 0; i < n_enemigos; ++i)
		enemigo_borrar(&enemigos[i], j->sprite);
	
}

void dibujar_panel() {
	glcd_rectangulo_relleno((max_x + 8) * TAM_PIXEL, 0, GLCD_TAMANO_X, GLCD_TAMANO_Y, AZUL_OSCURO);
}