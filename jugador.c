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

juego* j_g;

juego juego_inicializar() {
	
	__enable_irq();
	
	timer_inicializar(TIMER1);
	
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	LPC_TIMER1->IR |= 1;	
	
	timer_iniciar_ciclos_ms(TIMER1, 1000);
	
	NVIC_SetPriority(TIMER1_IRQn, 1);
	NVIC_EnableIRQ(TIMER1_IRQn);    
	
	jugador j = jugador_crear();
	
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		enemigos[i] = enemigo_crear(4 + (i % n_enem_linea) * 12 , 4 + (i / n_enem_linea) * 12);
	
	juego ret = {
		j, 
		0, 
		n_enemigos,
		0, 0
	};
	
	j_g = &ret;
	
	return ret;
}

void juego_actualizar(juego* j, uint8_t* nunchuk_data) {
	jugador_actualizar(&j->jugador, nunchuk_data);
}

void enemigos_dibujar() {
	for(uint8_t i = 0; i < n_enemigos; ++i)
		enemigo_dibujar(&enemigos[i], j_g->sprite, j_g->x, j_g->y);
}

void enemigos_borrar() {
	for(uint8_t i = 0; i < n_enemigos; ++i)
		enemigo_borrar(&enemigos[i], j_g->sprite, j_g->x, j_g->y);
}

void juego_dibujar(juego* j) {
	jugador_dibujar(&j->jugador);
}

void juego_borrar(juego* j) {
	jugador_borrar(&j->jugador);
}

void TIMER1_IRQHandler(void) {
	
		enemigos_borrar();
	
		++j_g->x; 
		j_g->sprite = !j_g->sprite;
	
		enemigos_dibujar();
	
		LPC_TIMER1->IR = 1;
		NVIC_ClearPendingIRQ(TIMER1_IRQn);
}

uint8_t samplear_rango(float in, float samples) {
	
	uint8_t out = 0;
	
	for(float f = 0; f < 1; f += 1./samples)
		out += (in >= f);
	
	return out;
	
}

void dibujar_panel() {
	
	static const int32_t width =  GLCD_TAMANO_X - (max_x + 10) * TAM_PIXEL;
	
	for(int32_t x = (max_x + 10) * TAM_PIXEL; x <= GLCD_TAMANO_X; ++x)
		for(int32_t y = 0; y <= GLCD_TAMANO_Y; ++y) {
			
			float col_green 		= (float)(x - (max_x + 10) * TAM_PIXEL) / (float)width,
						col_intensity	= (float) y / (float) GLCD_TAMANO_Y;
			
			uint8_t green 			= (samplear_rango(col_green * col_intensity, 16.) & 0xF) << 4,
							intensity		= samplear_rango(col_intensity, 						16.)  & 0xF;
			
			//printf("%f, %f, %u, %u\n\r", col_green, col_intensity, green, intensity);
			glcd_punto(x, y, green | intensity);
			
		}
}