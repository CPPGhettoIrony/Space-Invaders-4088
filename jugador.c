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
	
	if(boton_z && !j->bala) {
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

void enemigos_dibujar();

juego juego_inicializar() {  
	
	jugador j = jugador_crear();
	
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		enemigos[i] = enemigo_crear(4 + (i % n_enem_linea) * 12 , 4 + (i / n_enem_linea) * 12);
	
	juego ret = {
		j, 
		0, 
		n_enemigos,
		1, 0,
		1,
		0, 0, 0, 1000,
		FALSE
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

void TIMER1_IRQHandler(void) {
	
		enemigos_borrar();
	
		
	
		if(j_g->x + (n_enem_linea - j_g->offset_x_b) * 12 > max_x || j_g->x + j_g->offset_x_a * 12 <= 0) {
			j_g->direccion = -j_g->direccion;
			j_g->y+=2;
		}
		
		j_g->x += j_g->direccion; 
		j_g->sprite = !j_g->sprite;
	
		enemigos_dibujar();
		
		if(!j_g->game_over && j_g->y + (n_enemigos/n_enem_linea - j_g->offset_y) * 12 > altura)
			j_g->game_over = TRUE;
	
		LPC_TIMER1->IR = 1;
		NVIC_ClearPendingIRQ(TIMER1_IRQn);
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

void juego_actualizar(juego* j, uint8_t* nunchuk_data) {
	
	jugador_actualizar(&j->jugador, nunchuk_data);
	
	for(uint8_t i = 0; i < n_enemigos; ++i) {
		
		int32_t x = enemigos[i].x + j->x,
						y = enemigos[i].y + j->y;
		
		if(enemigos[i].vivo && j->jugador.bala && dentro_2d(x, y, 8, 8, j->jugador.bala_x, j->jugador.bala_y, 4, 3)) {
			
				enemigo_borrar(&enemigos[i], j->sprite, j->x, j->y);
				enemigos[i].vivo 	= FALSE;
			  j->jugador.bala 	= FALSE;
		
				--j->enemigos;
				j->periodo *= 0.91;
		
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
}

void juego_borrar(juego* j) {
	jugador_borrar(&j->jugador);
	if(j->jugador.bala)
		bala_borrar(&j->jugador);
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