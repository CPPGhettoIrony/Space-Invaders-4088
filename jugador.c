#include <stdlib.h>
#include "jugador.h"

juego* j_g;

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

uint8_t sprite_bala[8] = {
	0b01000000,
	0b10100000,
	0, 0, 0, 0, 0, 0
};

jugador jugador_crear() {
	jugador ret = {sprite_jugador, sprite_bala, 3, 0, 0, FALSE};
	return ret;
}

void jugador_actualizar(jugador* j, uint8_t* nunchuk_data) {
	
	int8_t	joystick	= (int8_t)nunchuk_data[0] + 120;
	bool_t	boton_c 	= !(nunchuk_data[5] & 2u),
					boton_z 	= !(nunchuk_data[5] & 1u);
	
	int8_t	direccion = (joystick > 30) - (joystick < -30);
	
	if(boton_z && !j->bala && !j_g->game_over) {
		
		sonido_jugador_bala();
		
		j->bala 	= TRUE;
		j->bala_x =	j->posicion - 1;
		j->bala_y	= altura - 3;
	}
	
	j->bala_y -= (j->bala)*2;
	
	if(j->bala && j->bala_y <= 0)
		j->bala = FALSE;
	
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

uint8_t sprite_enemigo_bala[8] = {
	0b10100000,
	0b01000000,
	0,0,0,0,0,0
};

enemigo enemigos[n_enemigos];

enemigo enemigo_crear(int32_t x, int32_t y) {
	
	enemigo ret = {
		{sprite_enemigo_a, sprite_enemigo_b}, 
		x, y,
		TRUE,
		0, 0,
		FALSE
	};
	
	return ret;
}

void enemigos_dibujar();

juego juego_inicializar() {  
	
	jugador j = jugador_crear();
	
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		enemigos[i] = enemigo_crear(4 + (i % n_enem_linea) * 12 , 4 + (i / n_enem_linea) * 12);
	
	juego ret = {
		0,
		3,
		0,
		j, 
		0, 
		n_enemigos,
		1, 0,
		1,
		0, 0, 0, 1000,
		FALSE, 0, 4
	};
	
	__enable_irq();
	
	timer_inicializar(TIMER1);
	
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	LPC_TIMER1->IR |= 1;	
	
	timer_iniciar_ciclos_ms(TIMER1, ret.periodo);
	
	NVIC_SetPriority(TIMER1_IRQn, 1);
	NVIC_EnableIRQ(TIMER1_IRQn);  
	
	j_g = &ret;
	
	enemigos_dibujar();
	
	return ret;
}

void enemigos_dibujar() {
	for(uint8_t i = 0; i < n_enemigos; ++i)
		if(enemigos[i].vivo)
			enemigo_dibujar(&enemigos[i], j_g->sprite, j_g->x, j_g->y);
}

void enemigos_borrar() {
	for(uint8_t i = 0; i < n_enemigos; ++i)
		if(enemigos[i].vivo)
			enemigo_borrar(&enemigos[i], j_g->sprite, j_g->x, j_g->y);
}


inline static bool_t dentro_rango(int32_t x, int32_t a, int32_t b) {
		return x >= a && x <= b;
}

inline static bool_t dentro_1d(int32_t xa, int32_t wa, int32_t xb, int32_t wb) {
		return dentro_rango(xa, xb, xb + wb) || dentro_rango(xa + wa, xb, xb + wb);
}

inline static bool_t dentro_2d(int32_t xa, int32_t ya, int32_t wa, int32_t ha, int32_t xb, int32_t yb, int32_t wb, int32_t hb) {
	return dentro_1d(xa, wa, xb, wb) && dentro_1d(ya, ha, yb, hb);
}	

void matar_jugador(juego* j);

void TIMER1_IRQHandler(void) {
	
		LPC_TIMER1->IR = 1;
		NVIC_ClearPendingIRQ(TIMER1_IRQn);
	
		enemigos_borrar();
	
		if(j_g->enemigos == 0 || j_g->game_over)
			return;
	
		if(j_g->x + (n_enem_linea - j_g->offset_x_b) * 12 > max_x || j_g->x + j_g->offset_x_a * 12 <= 0) {
			j_g->direccion = -j_g->direccion;
			j_g->y+=2;
		}
		
		j_g->x += j_g->direccion; 
		j_g->sprite = !j_g->sprite;
		
		uint8_t idm = rand()%n_enemigos, id = 0;
		
		enemigos_dibujar();
		
		if(j_g->balas < j_g->max_balas) {
			
			for(; id < n_enemigos; ++id)			
				if(id == idm) {
					
					if(!enemigos[id].vivo || enemigos[id].bala) {
						++idm;
						continue;
					}
					
					sonido_enemigo_bala();
					
					enemigos[id].bala = TRUE;
					enemigos[id].bala_x = enemigos[id].x + j_g->x + 4;
					enemigos[id].bala_y = enemigos[id].y + j_g->y + 8;
			
					++j_g->balas;
					break;
					
				}
			
		}
		
		if(j_g->enemigos > 0 && !j_g->game_over && j_g->y + (n_enemigos/n_enem_linea - j_g->offset_y) * 12 > altura)
			matar_jugador(j_g);
}

void enemigo_eliminar_columnas() {
		
	bool_t 	eliminar_columna_izq = TRUE,
					eliminar_columna_der = TRUE,
					eliminar_fila				 = TRUE;
	
	for(uint8_t i = 0; i < n_enemigos / n_enem_linea; ++i) {
		eliminar_columna_izq = eliminar_columna_izq && !enemigos[i*n_enem_linea + j_g->offset_x_a].vivo;
		eliminar_columna_der = eliminar_columna_der && !enemigos[(i+1)*n_enem_linea - j_g->offset_x_b - 1].vivo;
	}
	
	for(uint8_t i = 0; i < n_enem_linea; ++i)
		eliminar_fila = eliminar_fila && !enemigos[ n_enemigos - ((j_g->offset_y + 1) * n_enem_linea) + i].vivo;
	
	j_g->offset_x_a += eliminar_columna_izq;
	j_g->offset_x_b += eliminar_columna_der;
	
	j_g->offset_y		+= eliminar_fila;
	
}

void resetear_nivel(juego* j) {
	
	jugador_dibujar(&j->jugador);
	timer_retardo_ms(TIMER0, 3000);
	
	timer_poner_contador_a_0(TIMER1);
	timer_iniciar_ciclos_ms(TIMER1, 1000);
	
	for(uint8_t i = 0; i < n_enemigos; ++i) {
		enemigos[i].x 		= 4 + (i % n_enem_linea) * 12;
		enemigos[i].y 		= 4 + (i / n_enem_linea) * 12;
		enemigos[i].vivo 	= TRUE;
		enemigos[i].bala 	= FALSE;
	}
	
	j->sprite 					= 0;
	j->enemigos 				= n_enemigos;
	j->jugador.bala			= FALSE;
	j->x								= 1;
	j->y								= 0;
	j->direccion				= 1;
	j->offset_x_a				= 0;
	j->offset_x_b				= 0;
	j->offset_y					= 0;
	j->periodo					= 1000;
	j->balas						= 0;
	
}

void avanzar_nivel(juego* j) {

	sonido_avanzar_nivel();
	
	resetear_nivel(j);
	
	enemigos_dibujar();
	
	++j->nivel;
	++j->max_balas;
	
}

void matar_jugador(juego* j) {

	j->game_over = TRUE;
	
	enemigos_borrar();
	
	sonido_perder_vida();
	
	resetear_nivel(j);
	
	--j->vidas;
	j->game_over = j->vidas == 0;
	
	if(!j->game_over) {
		enemigos_dibujar();
		return;
	}
	
}

void juego_actualizar(juego* j, uint8_t* nunchuk_data) {
	
	jugador_actualizar(&j->jugador, nunchuk_data);
	
	if(j->enemigos == 0)
		avanzar_nivel(j);
	
	for(uint8_t i = 0; i < n_enemigos; ++i) {
		
		if(enemigos[i].bala) {
			
			if(dentro_2d(enemigos[i].bala_x, enemigos[i].bala_y, 3, 2, j->jugador.posicion, altura, 8, 8))
				matar_jugador(j);
			
			++enemigos[i].bala_y;
			if(enemigos[i].bala_y >= GLCD_TAMANO_Y) {
				--j->balas;
				enemigos[i].bala = FALSE;
			}
			
		}
		
		int32_t x = enemigos[i].x + j->x,
						y = enemigos[i].y + j->y;
		
		if(enemigos[i].vivo && j->jugador.bala && dentro_2d(j->jugador.bala_x, j->jugador.bala_y, 3, 2, x, y, 8, 8)) {
			
				enemigo_borrar(&enemigos[i], j->sprite, j->x, j->y);
		
				enemigos[i].vivo 	= FALSE;
			  j->jugador.bala 	= FALSE;
		
				--j->enemigos;
				j->periodo *= (0.92 - (float)j->nivel * 0.01);
		
				j->puntuacion += 50;
		
				timer_poner_contador_a_0(TIMER1);
				timer_iniciar_ciclos_ms(TIMER1,j->periodo);
		
		}
		
	}
	
	enemigo_eliminar_columnas();
	
}

void juego_dibujar(juego* j) {
	
	jugador_dibujar(&j->jugador);
	if(j->jugador.bala)
		bala_dibujar(&j->jugador);
	
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		if(enemigos[i].bala)
			dibujar_sprite(sprite_enemigo_bala, enemigos[i].bala_x, enemigos[i].bala_y, AMARILLO);
	
}

void juego_borrar(juego* j) {
	
	jugador_borrar(&j->jugador);
	if(j->jugador.bala)
		bala_borrar(&j->jugador);
	
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		if(enemigos[i].bala)
			dibujar_sprite(sprite_enemigo_bala, enemigos[i].bala_x, enemigos[i].bala_y, NEGRO);
	
}

uint8_t samplear_rango(float in, float samples) {
	
	uint8_t out = 0;
	
	for(float f = 0; f < 1; f += 1./samples)
		out += (in >= f);
	
	return out;
	
}

void secuencia_inicial() {
	
	static const uint32_t melodia[48] = {
		NOTE_D5, NOTE_F5,  NOTE_A5, NOTE_C6,
		NOTE_D6, NOTE_F6,  NOTE_A6, NOTE_C7,
		NOTE_D5, NOTE_F5,  NOTE_A5, NOTE_C6,
		NOTE_D6, NOTE_F6,  NOTE_A6, NOTE_C7,
		NOTE_D5, NOTE_F5,  NOTE_A5, NOTE_C6,
		NOTE_D6, NOTE_F6,  NOTE_A6, NOTE_C7,
		NOTE_D5, NOTE_FS5, NOTE_A5, NOTE_CS6,
		NOTE_D6, NOTE_FS6, NOTE_A6, NOTE_CS7,
		NOTE_D5, NOTE_FS5, NOTE_A5, NOTE_CS6,
		NOTE_D6, NOTE_FS6, NOTE_A6, NOTE_CS7,
		NOTE_D5, NOTE_FS5, NOTE_A5, NOTE_CS6,
		NOTE_D6, NOTE_FS6, NOTE_A6, NOTE_CS7,
	};
	
	uint32_t note = 0;
	
	static const int32_t offset	= (max_x + 10) * TAM_PIXEL;
	static const int32_t width 	= GLCD_TAMANO_X - offset;
	
	for(int32_t x = offset; x <= GLCD_TAMANO_X; ++x) {
	
		int32_t i = x - offset;
		
		if(!(i % 3))
			sonido_emitir_pitido(melodia[note++ % 48], 30);
	
		for(int32_t y = 0; y <= GLCD_TAMANO_Y; ++y) {
			
			float col_green 		= (float)(x - (max_x + 10) * TAM_PIXEL) / (float)width,
						col_intensity	= (float) y / (float) GLCD_TAMANO_Y;
			
			uint8_t green 			= (samplear_rango(col_green * col_intensity, 16.) & 0xF) << 4,
							intensity		= samplear_rango(col_intensity, 						16.)  & 0xF;
			
			//printf("%f, %f, %u, %u\n\r", col_green, col_intensity, green, intensity);
			glcd_punto(x, y, green | intensity);
			
		}
	}
		
}

void dibujar_panel() {
	glcd_xprintf((max_x + 10) * TAM_PIXEL + 8,  8, BLANCO, NEGRO, FUENTE8X16, "Vidas: %u", 	j_g->vidas);
	glcd_xprintf((max_x + 10) * TAM_PIXEL + 8, 28, BLANCO, NEGRO, FUENTE8X16, "Nivel: %u", 	j_g->nivel);
	glcd_xprintf((max_x + 10) * TAM_PIXEL + 8, 98, BLANCO, NEGRO, FUENTE8X16, "Puntos: %u", j_g->puntuacion);
}