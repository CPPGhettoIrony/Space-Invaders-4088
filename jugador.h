// Librería original del repositorio, escrita por Manuel CB
#ifndef JUGADOR_H
#define JUGADOR_H

#include "sprites.h"
#include "nunchuk.h"
#include "sonido.h"

// Altura del jugador
#define altura 	120

// Límite entre la pantalla de juego y el panel (que contiene información sobre la puntuación, nivel y vidas)
#define max_x		168

// Estructura del jugador:

typedef struct {
	
	uint8_t		*sprite, *spr_bala;					// Sprites del propio jugador y de las balas que dispara
	int32_t  	posicion, bala_x, bala_y;		// Posición horizontal y coordenadas de la bala disparada 
	
	/*
		En Space Invaders, hasta que la bala no haya salido de la pantalla o disparado a un enemigo, no se puede disparar otra
		Básicamente el jugador tiene una sola bala
	*/

	bool_t		bala;												// Indica si la bala está siendo disparada (si existe y se está moviendo hacia arriba)
	
} jugador;

// Función que crea el jugador en su estado inicial y lo devuelve
jugador jugador_crear();

// Actualizar el jugador
void jugador_actualizar(jugador* j, uint8_t* nunchuk_data);

// Dibujar el jugador y borrarlo (dibujar su sprite en negro)

inline static void jugador_dibujar(jugador* j) {
	dibujar_sprite(j->sprite, j->posicion, altura, BLANCO);
}

inline static void jugador_borrar(jugador* j) {
	dibujar_sprite(j->sprite, j->posicion, altura, NEGRO);
}

// Se aplica lo mismo a la bala

inline static void bala_dibujar(jugador* j) {
	dibujar_sprite(j->spr_bala, j->bala_x, j->bala_y, BLANCO);
}

inline static void bala_borrar(jugador* j) {
	dibujar_sprite(j->spr_bala, j->bala_x, j->bala_y, NEGRO);
}

// Número de enemigos por línea y número de enemigos estático (cada vez que empieza el nivel)

#define n_enem_linea	12
#define n_enemigos 		48

// Siempre se crearán 48 enemigos repartidos en 4 filas de 12 al empezar un nivel


// Estructura del enemigo

typedef struct{
	
	uint8_t*	sprites[2];				// Sprites para la animación del movimiento
	int32_t		x, y;							// Coordenadas relativas
	
	/*
		En Space Invaders, los enemigos siempre se mueven a la vez, si ignoramos este movimiento, un enemigo nunca cambia de posición con respecto a los demás.
	
		Cuando una columna vertical de enemigos es eliminada, si dicha columna estaba en uno de los bordes (izquierda o derecha), los enemigos seguirán moviéndose
		hasta que alguno toque uno de los bordes de la pantalla de juego
	
		Se puede abstraer todo esto en una sola coordenada para la posición absoluta del grupo, y las coordenadas estáticas relativas de cada enemigo con respecto a esta
	*/
	
	bool_t		vivo;
	
	/*
		Lo explicado anteriormente sobre las balas del jugador también aplica aquí
	*/
	int32_t		bala_x, bala_y;
	bool_t		bala;
	
} enemigo;

enemigo enemigo_crear(int32_t x, int32_t y);

// Dibujar y borrar el sprite de un enemigo. Los "offsets" consisten en la posición total de todo el grupo, a la que se le suma una cantidad por fila y columna
// (posición de un enemigo relativo al primer enemigo de la primera fila)

inline static void enemigo_dibujar(enemigo* e, uint8_t sprite, int32_t offset_x, int32_t offset_y) {
	dibujar_sprite(e->sprites[sprite], e->x + offset_x, e->y + offset_y, VERDE);
}

inline static void enemigo_borrar(enemigo* e, uint8_t sprite, int32_t offset_x, int32_t offset_y) {
	dibujar_sprite(e->sprites[sprite], e->x + offset_x, e->y + offset_y, NEGRO);
}

// Estructura de todo el juego en sí

typedef struct {

	uint8_t		nivel;															// Nivel actual en el que el jugador está
	uint8_t		vidas;															// Las vidas que le quedan al jugador
	uint32_t	puntuacion;													// La puntuación actual

	jugador		jugador;														// Instancia del jugador (la struct jugador)
	
	uint8_t		sprite;															// ID del sprite de la animación del enemigo a dibujar
																								// En Space Invaders, todos los enemigos de la misma especie están en el mismo frame de la animación
																								// Básicamente compartiendo este atributo
	
	uint8_t		enemigos;														// Número de enemigos restante

	int32_t		x, y;																// Posición del primer enemigo de la primera fila, de la que se parte para procesar las posiciones de los demás
																								// estos se usan de argumentos para offset_x y offset_y en las funciones para dibujar y borrar enemigos
	
	int32_t		direccion;													// La dirección en la que se mueven los enemigos, cambia cuando tocan un límite		
	
	int8_t		offset_x_a, offset_x_b, offset_y;		// Indica las columnas y filas enteras seguidas (antes del primer enemigo vivo) de los enemigos
	
																								// Esto sirve para que el grupo cambie de dirección cuando el límite (tanto por la derecha como por la izquierda)
																								// sea tocado por un enemigo vivo.
																								
																								// También permite que cuando un enemigo vivo llegue abajo, se pierda una vida y se reinicie el nivel
																								
																								// Sin esto, los enemigos cambiarán de dirección (y se perderá una vida) antes de tiempo

	uint32_t  periodo;														// El periodo de temporizador hardware que los enemigos esperan antes de moverse
																								// En el juego original todos los enemigos se mueven cada cierto tiempo, 
																								// cada vez que un enemigo mueve, el periodo se reduce, aumentando la velocidad del movimiento
																								
	bool_t		game_over;													// En realidad, sólamente indica si el jugador está muerto, un verdadero game over se da además cuando el número de enemigos
																								// restante es cero

	uint8_t		balas, max_balas;										// El número de balas expulsadas por los enemigos y el máximo de balas enemigas permitidas, este último valor aumenta una unidad
																								// cada vez que se completa un nivel
	
} juego;


// Crear y devolver la estructura de juego con los valores por defecto
juego juego_inicializar();

// Actualizar el jugador, las muertes de los enemigos y las balas
void juego_actualizar(juego* j, uint8_t* nunchuk_data);

// Dibujarlas
void juego_dibujar(juego* j);

// Borrarlas
void juego_borrar(juego* j);

// Escena inicial donde se escucha una melodía y se dibuja poco a poco el panel de la izquierda
void secuencia_inicial();

// Imprimir los textos de puntuación, nivel y vidas
void dibujar_panel();


// Funciones que emiten cada uno de los sonidos del juego, excepto la de la secuencia inicial

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