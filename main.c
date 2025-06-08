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

	uint8_t nunchuk_data[6] = {0,0,0,0,0,0};
	
	const char* game_over_str = "GAME OVER";
	
	nunchuk_inicializar();
	nunchuk_leer(nunchuk_data);
	
	glcd_inicializar();
	
	dibujar_panel();
	
	juego j = juego_inicializar();
	
	while(1){
		
		if(j.game_over) {
			glcd_xprintf(GLCD_TAMANO_X/2 - strlen(game_over_str)*16, GLCD_TAMANO_Y/2 - 16, ROJO, NEGRO, FUENTE16X32, "%s", game_over_str);
			continue;
		}
		
		juego_actualizar(&j, nunchuk_data);
		
		juego_dibujar(&j);
		
		nunchuk_leer(nunchuk_data);
		timer_retardo_ms(TIMER0, 25);
	
		juego_borrar(&j);
				
	}

}
