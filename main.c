/*******************************************************************************
 *\author			Manuel CB
 *
 *\file 			main.c
 *
 *\brief			Modo sleep del LPC40xx (sección 3.12 del manual) y 
 *						deshabilitar periféricos activados por defecto
 *						(sección 3.3.2.2 del manual)
 *
 */

#include <LPC407x_8x_177x_8x.h>
#include "tipos.h"
#include "jugador.h"
#include "nunchuk.h"

//Prototipos de funciones
void bajo_consumo(void);

int main(){

	//Definimos si necesitamos alguna variable local en el main
	
	jugador j = jugador_crear();
	
	uint8_t nunchuk_data[6] = {0,0,0,0,0,0};
	
	nunchuk_inicializar();
	nunchuk_leer(nunchuk_data);
	
	glcd_inicializar();

	while(1){
		
		jugador_actualizar(&j, nunchuk_data);
		
		jugador_dibujar(&j);
		
		nunchuk_leer(nunchuk_data);
		timer_retardo_ms(TIMER0, 10);
	
		jugador_borrar(&j);
				
	}
}
