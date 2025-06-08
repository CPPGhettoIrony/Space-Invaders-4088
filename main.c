/*******************************************************************************
 *\author			Manuel CB
 *
 *\file 			main.c
 *
 */

#include <LPC407x_8x_177x_8x.h>
#include "jugador.h"

int main(){

	uint8_t nunchuk_data[6] = {0,0,0,0,0,0};
	
	nunchuk_inicializar();
	nunchuk_leer(nunchuk_data);
	
	glcd_inicializar();
	
	dibujar_panel();
	
	juego j = juego_inicializar();
	
	while(1){
		
		juego_actualizar(&j, nunchuk_data);
		
		juego_dibujar(&j);
		
		nunchuk_leer(nunchuk_data);
		timer_retardo_ms(TIMER0, 25);
	
		juego_borrar(&j);
				
	}

}
