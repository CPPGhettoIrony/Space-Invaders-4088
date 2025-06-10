/*******************************************************************************
 *\author			Manuel CB
 *
 *\file 			main.c
 *
 */

#include <LPC407x_8x_177x_8x.h>
#include <string.h>
#include "jugador.h"

int main(){

	// Los seis bytes devueltos por el controlador i2c del nunchuk
	uint8_t nunchuk_data[6] = {0,0,0,0,0,0};
	
	// Cadena de "GAME OVER", se crea previamente para calcular su tamaño en la imagen y centrar el texto
	const char* game_over_str = "GAME OVER";
	
	// Inicializar Timer 0 (Para producir retardos en muchas de las funciones del código)
	timer_inicializar(TIMER0);
	
	// Inicialización del I2C0 para controlar el nunchuk
	nunchuk_inicializar();
	
	// Leer los datos del nunchuk (conseguir dirección del joystick y estado del botón z)
	nunchuk_leer(nunchuk_data);
	
	// Inicializar pantalla
	glcd_inicializar();
	
	// Secuencia inicial, al encender el dispositivo/comenzar el juego, se dibuja poco a poco el panel de la izquierda
	// mientras tanto, se emite por el altavoz una melodía.
	secuencia_inicial();
	
	// Se inicializa el juego en sí. En el bucle, se procesan el jugador y los disparos, los enemigos, su movimiento, y la mayoría
	// de características relevantes a ellos se dibujan/actualizan mediante interrupciones del temporizador uno.
	juego j = juego_inicializar();
	
	// Bucle principal
	while(1){
		
		// Se imprimen por pantalla la información sobre las vidas, el nivel y la puntuación actuales.
		dibujar_panel();
		
		// Se comprueba si el jugador está muerto (game_over) y si no quedan vidas
		if(j.vidas == 0 && j.game_over) {
			
			// Se imprime de manera centrada el texto de "GAME OVER"
			glcd_xprintf(GLCD_TAMANO_X/2 - strlen(game_over_str)*16, GLCD_TAMANO_Y/2 - 16, ROJO, NEGRO, FUENTE16X32, "%s", game_over_str);
			
			// Se emite la melodía fúnebre de fin de juego
			sonido_game_over();
			
			// El programa termina, se pulsa reset para reiniciar
			break;
		}
		
		// Se actualiza el juego y sus elementos (el jugador, las muertes de los enemigos y las balas (tanto del propio jugador como de los enemigos))
		juego_actualizar(&j, nunchuk_data);
		
		// Se dibuja en la pantalla el jugador y las balas mencionadas anteriormente
		juego_dibujar(&j);
		
		// Obtener información del nunchuk
		nunchuk_leer(nunchuk_data);
		
		// Esperar 25 milisegundos
		timer_retardo_ms(TIMER0, 25);
	
		// Se borran el jugador y las balas para que se dibujen en la siguiente iteración
		
		// La pantalla LCD del kit de desarrollo carece de la potencia para actualizar su pantalla entera cada ciclo de reloj,
		// Así que se ponen todos los píxeles dibujados en la pantalla de juego (aparte del panel) en negro 
		
		juego_borrar(&j);
				
	}
	
}
