// Librería original del repositorio, escrita por Manuel CB
#include <stdlib.h>
#include "jugador.h"

// Se necesita procesar la estructura de juego dentro de la interrupción del timer 1
// como no se pueden pasar argumentos por dicha función, al crear la estructura de juego, se setea el puntero para que apunte a esta,
// de esta forma se puede acceder a la estructura desde la función IRQ
juego* j_g;

// Sprites del jugador y la bala correspondiente

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


// Crear el objeto jugador
jugador jugador_crear() {
	jugador ret = {sprite_jugador, sprite_bala, 3, 0, 0, FALSE};
	return ret;
}

// Actualizar el jugador en cada fotograma
void jugador_actualizar(jugador* j, uint8_t* nunchuk_data) {
	
	// El primer byte devuelto por el nunchuk en una transmisión I2C contiene el valor asociado con la posición horizontal del joystick
	// dicha posición se centraliza (0 = centro, >30 = izquierda, <-30 = derecha)
	int8_t	joystick	= (int8_t)nunchuk_data[0] + 120;
	
	// El último byte contiene información extra por bit, el bit menos significativo indica si el botón z está pulsado
	// (1 = no pulsado, 0 = pulsado)
	
	bool_t	boton_z 	= !(nunchuk_data[5] & 1u); // De esta forma, boton_z = TRUE cuando el botón está pulsado
	
	// Usando la maravillosa característica de C que interpreta booleanos como los números 0 y 1, nos saltamos bloques if/else innecesarios
	// de esta forma, direccion es 0 si el joystick está en el centro, 1 si se mueve a la izquierda y -1 si se mueve a la derecha
	int8_t	direccion = (joystick > 30) - (joystick < -30);
	
	// Si el jugador está vivo, se presiona Z y no hay ninguna bala existente
	if(boton_z && !j->bala && !j_g->game_over) {
		
		// Se reproduce el sonido de disparo del jugador
		sonido_jugador_bala();
		
		// Se indica que ya hay de por sí una bala existente, se posiciona de manera en el que se da la ilusión de que la bala sale del jugador
		j->bala 	= TRUE;
		j->bala_x =	j->posicion - 1;
		j->bala_y	= altura - 3;
		
	}
	
	// Esta línea mueve hacia arriba dos unidades la bala cada fotograma si esta existe
	j->bala_y -= (j->bala)*2;
	
	// Si la bala se sale de la pantalla por arriba, esta deja de existir y se puede disparar otra
	if(j->bala && j->bala_y <= 0)
		j->bala = FALSE;
	
	// Mover el jugador dependiendo de la dirección del joystick
	j->posicion += direccion * 2;
	
	// Mantener la posición dentro de los límites
	j->posicion = j->posicion * (j->posicion > 2 && j->posicion < max_x) + 3 * (j->posicion <=2) + (max_x - 1) * (j->posicion >= max_x);
	
}

// Primer sprite de la animación del enemigo

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

// Segundo sprite

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

// Sprite de las balas disparadas por los enemigos

uint8_t sprite_enemigo_bala[8] = {
	0b10100000,
	0b01000000,
	0,0,0,0,0,0
};

// Array estático donde se almacenan todos los enemigos

enemigo enemigos[n_enemigos];


// Crear un enemigo en la posición x,y con sus valores por defecto
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

// Declaración previa de la función para llamarla en funciones anteriores a su definición
void enemigos_dibujar();

// Iniciar el juego y devolver su objeto
juego juego_inicializar() {  
	
	// Crear al jugador y a los enemigos
	
	jugador j = jugador_crear();
	
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		enemigos[i] = enemigo_crear(4 + (i % n_enem_linea) * 12 , 4 + (i / n_enem_linea) * 12);
	
	// Crear la estructura del juego
	
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
	
	// Activar interrupciones
	
	__enable_irq();
	
	// Activar el timer para la dibujar, borrar y actualizar enemigos cada (juego.periodo) milisegundos
	
	timer_inicializar(TIMER1);
	
	// Código necesario para configurar el timer 1
	
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	LPC_TIMER1->IR |= 1;	
	
	timer_iniciar_ciclos_ms(TIMER1, ret.periodo);
	
	NVIC_SetPriority(TIMER1_IRQn, 1);
	NVIC_EnableIRQ(TIMER1_IRQn);  
	
	// Asignar el juego a crear al puntero j_g para acceder a este en el handler de la interrupción del timer 1
	
	j_g = &ret;
	
	// Dibujar todos los enemigos para no esperar un segundo a que sean visibles
	
	enemigos_dibujar();
	
	return ret;
}

// Dibujar y borrar a todos los enemigos (que estén vivos)

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

// Funciones necesarias para detectar colisiones entre rectángulos (detección de colisión AABB)

// Devuelve si x está dentro del rango [a, b]
inline static bool_t dentro_rango(int32_t x, int32_t a, int32_t b) {
		return x >= a && x <= b;
}

// Devuelve si hay una intersección entre los rangos [xa, xa + wa] y [xa, xa + wa]
inline static bool_t dentro_1d(int32_t xa, int32_t wa, int32_t xb, int32_t wb) {
		return dentro_rango(xa, xb, xb + wb) || dentro_rango(xa + wa, xb, xb + wb);
}

// Detectar una colisión entre dos rectángulos de dimensiones wa,ha y wb,hb en las posiciones xa,ya y xb,yb
inline static bool_t dentro_2d(int32_t xa, int32_t ya, int32_t wa, int32_t ha, int32_t xb, int32_t yb, int32_t wb, int32_t hb) {
	return dentro_1d(xa, wa, xb, wb) && dentro_1d(ya, ha, yb, hb);
}	

// Declaración previa de la función a la que se llama cuando se pierde una vida
void matar_jugador(juego* j);

// Esta función permite actualizar la posición de los enemigos (y cuáles disparan al jugador) cada vez que se deben mover
// Se llama cada vez que el temporizador uno alcanza el período configurado en la estructura del juegos
void TIMER1_IRQHandler(void) {
	
		// Esto permite que se llame de nuevo a esta función
		LPC_TIMER1->IR = 1;
		NVIC_ClearPendingIRQ(TIMER1_IRQn);
	
		// Antes de mover a los enemigos, borrarlos de la pantalla
		enemigos_borrar();
	
		// Si el jugador está muerto, no se hace nada
		if(j_g->enemigos == 0 || j_g->game_over)
			return;
	
		// Cambiar de dirección y avanzar hacia abajo si se ha tocado borde
		if(j_g->x + (n_enem_linea - j_g->offset_x_b) * 12 > max_x || j_g->x + j_g->offset_x_a * 12 <= 0) {
			j_g->direccion = -j_g->direccion;
			j_g->y+=2;
		}
		
		// Avanzar en la dirección configurada y pasar al siguiente sprite de la animación
		j_g->x += j_g->direccion; 
		j_g->sprite = !j_g->sprite;
		
		// Volver a dibujar a los enemigos en pantalla en su nueva posición
		enemigos_dibujar();
		
		// Se decide, de manera aleatoria, cuales enemigos son los que disparan
		if(j_g->balas < j_g->max_balas) {
			
			// Se decide que el enemigo número (idm) disparará
			uint8_t idm = rand()%n_enemigos, id = 0;
			
			for(; id < n_enemigos; ++id)			
				if(id == idm) {
					
					// Se comprueba si está disparando o si está muerto
					if(!enemigos[id].vivo || enemigos[id].bala) {
						// en este caso, se intenta con el siguiente
						++idm;
						continue;
					}
					
					// Si se llega a esta parte, el enemigo al que se ha llegado está listo para disparar
					
					// Se reproduce el efecto de sonido correspondiente
					sonido_enemigo_bala();
					
					// Se indica que, en efecto, este enemigo está disparando hasta que dicha bala llegue al suelo
					enemigos[id].bala = TRUE;
					
					// Se posiciona la bala debajo del enemigo
					enemigos[id].bala_x = enemigos[id].x + j_g->x + 4;
					enemigos[id].bala_y = enemigos[id].y + j_g->y + 8;
			
					// Queda una bala menos por disparar
					++j_g->balas;
					break;
					
				}
			
		}
		
		// Si los enemigos han llegado a tocar suelo, se pierde una vida
		if(j_g->enemigos > 0 && !j_g->game_over && j_g->y + (n_enemigos/n_enem_linea - j_g->offset_y) * 12 > altura)
			matar_jugador(j_g);
}

// Esta función se encarga de determinar cuántas columnas vacías hay desde cada borde hacia el primer enemigo vivo
// (y cuántas filas desde abajo hay quedado vacías)

// De esta forma, estos (los que estén vivos) rebotarán al tocar cualquiera de los bordes horizontales y cuando el grupo de enemigos
// más cercano al suelo lo toque, indicará la pérdida de una vida y el reinicio del nivel

void enemigo_eliminar_columnas() {
		
	bool_t 	eliminar_columna_izq = TRUE,		// Indica si la primera columna desde la izquierda previamente viva ha sido aniquilada
					eliminar_columna_der = TRUE,		// Indica si la primera columna desde la derecha   previamente viva ha sido aniquilada
					eliminar_fila				 = TRUE;		// Indica si la primera fila    desde abajo				 previamente viva ha sido aniquilada
	
	// Se va comprobando si cada condición, asumida cierta en un principio, sigue siendo verdad
	
	for(uint8_t i = 0; i < n_enemigos / n_enem_linea; ++i) {
		eliminar_columna_izq = eliminar_columna_izq && !enemigos[i*n_enem_linea + j_g->offset_x_a].vivo;
		eliminar_columna_der = eliminar_columna_der && !enemigos[(i+1)*n_enem_linea - j_g->offset_x_b - 1].vivo;
	}
	
	for(uint8_t i = 0; i < n_enem_linea; ++i)
		eliminar_fila = eliminar_fila && !enemigos[ n_enemigos - ((j_g->offset_y + 1) * n_enem_linea) + i].vivo;
	
	// Si la columna o fila ha sido aniquilada, se da por eliminada, permitiendo colisiones adecuadas
	
	j_g->offset_x_a += eliminar_columna_izq;
	j_g->offset_x_b += eliminar_columna_der;
	
	j_g->offset_y		+= eliminar_fila;
	
}

// Esta función se llama cada vez que el nivel debe reiniciarse (por muerte o victoria)

void resetear_nivel(juego* j) {
	
	// Se vuelve a dibujar el jugador
	jugador_dibujar(&j->jugador);
	
	// Se esperan tres segundos
	timer_retardo_ms(TIMER0, 3000);
	
	// Se resetea el contador uno y su período, reseteando la velocidad del enemigo
	timer_poner_contador_a_0(TIMER1);
	timer_iniciar_ciclos_ms(TIMER1, 1000);
	
	// Se reviven y resetean todos los enemigos
	for(uint8_t i = 0; i < n_enemigos; ++i) {
		enemigos[i].x 		= 4 + (i % n_enem_linea) * 12;
		enemigos[i].y 		= 4 + (i / n_enem_linea) * 12;
		enemigos[i].vivo 	= TRUE;
		enemigos[i].bala 	= FALSE;
	}
	
	// Se resetea el juego a la condición inicial sin perder progreso
	
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

// Función que avanza al nivel siguiente cuando se destruyen a todos los enemigos

void avanzar_nivel(juego* j) {

	// Se reproduce el sonido adecuado
	sonido_avanzar_nivel();
	
	// Se resetea el nivel
	resetear_nivel(j);
	
	// Se vuelven a dibujar los enemigos para que en el primer ciclo del temporizador no estén invisibles 
	enemigos_dibujar();
	
	// Se avanza al siguiente nivel, en el que se dispararán más balas
	++j->nivel;
	++j->max_balas;
	
}

// La siguiente función reinicia el nivel y resta una vida

void matar_jugador(juego* j) {

	// Durante este período de tiempo, el jugador muere
	// Esto se realiza aquí y no en avanzar_nivel debido a que, en el caso anterior, no quedan enemigos, 
	// al contrario que en esta situación, en la que, al haber enemigos restantes y al estos actualizarse mediante interrupciones aparte,
	// estos necesitan desactivarse mientras se esté reiniciando el nivel
	
	j->game_over = TRUE;
	
	// Se borran los enemigos de la pantalla
	
	enemigos_borrar();
	
	// Efecto de sonido correspondiente
	
	sonido_perder_vida();
	
	// Se resetea el nivel
	
	resetear_nivel(j);
	
	// Se pierde una vida
	
	--j->vidas;
	
	// Se comrpueba si quedan más vidas
	
	j->game_over = j->vidas == 0;
	
	// En caso de que queden, se vuelven a dibujar los enemigos en pantalla
	
	if(!j->game_over)
		enemigos_dibujar();
	
}

/*
	Mientras los enemigos se actualizan en el IRQ Handler, la siguiente función se encarga de actualizar:
		·	El jugador
		·	Las balas del jugador
		·	Las balas de los enemigos
		·	Las muertes de los enemigos
*/

void juego_actualizar(juego* j, uint8_t* nunchuk_data) {
	
	// Actualizar el jugador
	jugador_actualizar(&j->jugador, nunchuk_data);
	
	// Avanzar nivel si no quedan más enemigos
	if(j->enemigos == 0)
		avanzar_nivel(j);
	
	// Por cada enemigo...
	for(uint8_t i = 0; i < n_enemigos; ++i) {
		
		// Si está disparando
		if(enemigos[i].bala) {
			
			// Matar al jugador si su bala colisiona con este
			if(dentro_2d(enemigos[i].bala_x, enemigos[i].bala_y, 3, 2, j->jugador.posicion, altura, 8, 8))
				matar_jugador(j);
			
			// Descender la bala una unidad
			++enemigos[i].bala_y;
			
			// Si sale de la pantalla
			if(enemigos[i].bala_y >= GLCD_TAMANO_Y) {
				// Se elimina la bala
				--j->balas;
				// El enemigo ya no está disparando
				enemigos[i].bala = FALSE;
			}
			
		}
		
		// Posición absoluta del enemigo en la pantalla
		int32_t x = enemigos[i].x + j->x,
						y = enemigos[i].y + j->y;
		
		// Si está vivo y está colisionando con la bala del jugador
		if(enemigos[i].vivo && j->jugador.bala && dentro_2d(j->jugador.bala_x, j->jugador.bala_y, 3, 2, x, y, 8, 8)) {
			
				// El enemigo se elimina instantáneamente de la pantalla
				enemigo_borrar(&enemigos[i], j->sprite, j->x, j->y);
		
				// El enemigo está muerto
				enemigos[i].vivo 	= FALSE;
		
				// La bala del jugador se destruye
			  j->jugador.bala 	= FALSE;
		
				// Queda un enemigo menos
				--j->enemigos;
		
				// Se reduce el periodo de actualización de los enemigos, moviéndose más rápido
				// En cada nivel, el periodo se reduce más, haciendo que los enemigos se hagan más rápidos en cada nivel	
				j->periodo *= (0.92 - (float)j->nivel * 0.01);
		
				// El periodo nunca será cero
				j->periodo += (j->periodo == 0);
		
				// Si se prefiere que no sea menor que 10 (por si acaso se vuelve el juego demasiado difícil, comentar la línea anterior y descomentar esta
				// (sustituir 10 por el periodo deseado)
				//j->periodo = 10*(j->periodo < 10) + j->periodo*(j->periodo >= 10);
		
				// Se obtienen 50 puntos
				j->puntuacion += 50;
		
				// El timer 1 se actualiza con el nuevo periodo
				timer_poner_contador_a_0(TIMER1);
				timer_iniciar_ciclos_ms(TIMER1,j->periodo);
		
		}
		
	}
	
	// Se actualizan los offsets de colisión de los enemigos
	enemigo_eliminar_columnas();
	
}

// Se dibujan todos los elementos
void juego_dibujar(juego* j) {
	
	// Se empieza por el jugador
	jugador_dibujar(&j->jugador);
	
	// Se dibuja su bala si existe
	if(j->jugador.bala)
		bala_dibujar(&j->jugador);
	
	// Si un enemigo está disparando, se dibuja su bala
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		if(enemigos[i].bala)
			dibujar_sprite(sprite_enemigo_bala, enemigos[i].bala_x, enemigos[i].bala_y, AMARILLO);
	
}

// Idéntica a la función anterior, pero esta borra los elementos
void juego_borrar(juego* j) {
	
	jugador_borrar(&j->jugador);
	if(j->jugador.bala)
		bala_borrar(&j->jugador);
	
	for(uint8_t i = 0; i < n_enemigos; ++i) 
		if(enemigos[i].bala)
			dibujar_sprite(sprite_enemigo_bala, enemigos[i].bala_x, enemigos[i].bala_y, NEGRO);
	
}


// La siguiente función tiene de entrada un valor real de 0 a uno,
// y de salida devolverá un valor entero entre cero y samples
/*
	Por ejemplo, con samples=4:

		in:0		-	0.24999...			out:0	
		in:0.25	-	0.04999...			out:1
		in:0.5	-	0.76999...			out:2
		in:0.75	-	0.99999...			out:3
*/
uint8_t samplear_rango(float in, float samples) {
	
	uint8_t out = 0;
	
	for(float f = 0; f < 1; f += 1./samples)
		out += (in >= f);
	
	return out;
	
}

// Se encarga de producir la bonita escena inicial al encender el dispositivo
// Se dibuja el panel de la izquierda mientras se hace sonar una melodía de introducción
void secuencia_inicial() {
	
	// Las notas de la melodía
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
	
	// La nota a emitir en un momento dado
	uint32_t note = 0;
	
	// Por dónde se empieza a dibujar el panel
	static const int32_t offset	= (max_x + 10) * TAM_PIXEL;
	
	// El ancho del panel
	static const int32_t width 	= GLCD_TAMANO_X - offset;
	
	// Se itera horizontalmente desde el inicio hasta el final del panel
	for(int32_t x = offset; x <= GLCD_TAMANO_X; ++x) {
	
		// Coordenada horizontal de cada línea vertical de píxeles con respecto al panel
		int32_t i = x - offset;
		
		// Cada tres líneas dibujadas, se emite una nota de la melodía
		if(!(i % 3))
			sonido_emitir_pitido(melodia[note++ % 48], 30);
	
		// Se itera por cada línea vertical de píxeles
		for(int32_t y = 0; y <= GLCD_TAMANO_Y; ++y) {
			
			// Estos cálculos proporciona el aspecto procedural del panel, dibujando el fondo azul con las líneas curvas
			// a partir de funciones matemáticas
			
			float col_green 		= (float)(x - (max_x + 10) * TAM_PIXEL) / (float)width,
						col_intensity	= (float) y / (float) GLCD_TAMANO_Y;
			
			// Se obtiene el azul y el verde a partir de los floats anteriores, sampleándolos en 16 valores posibles
			// Para comprimir cada float en un solo byte
			uint8_t green 			= (samplear_rango(col_green * col_intensity, 16.) & 0xF) << 4,
							intensity		= samplear_rango(col_intensity, 						 16.) & 0xF;
			
			// Los bytes se concatenan para formar el color de cada píxel
			glcd_punto(x, y, green | intensity);
			
		}
	}
		
}

// Imprimir las vidas, el nivel y la puntuación
void dibujar_panel() {
	glcd_xprintf((max_x + 10) * TAM_PIXEL + 8,  8, BLANCO, NEGRO, FUENTE8X16, "Vidas: %u", 	j_g->vidas);
	glcd_xprintf((max_x + 10) * TAM_PIXEL + 8, 28, BLANCO, NEGRO, FUENTE8X16, "Nivel: %u", 	j_g->nivel);
	glcd_xprintf((max_x + 10) * TAM_PIXEL + 8, 98, BLANCO, NEGRO, FUENTE8X16, "Puntos: %u", j_g->puntuacion);
}