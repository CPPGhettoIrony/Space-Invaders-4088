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
#include "sprites.h"
#include "nunchuk.h"

//Prototipos de funciones
void bajo_consumo(void);

int main(){

	//Definimos si necesitamos alguna variable local en el main
	
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
	
	int32_t posicion 				= 0;
	const int32_t altura		= 120;
	const int32_t max_x			= 230;
	
	uint8_t nunchuk_data[6] = {0,0,0,0,0,0};
	
	nunchuk_inicializar();
	nunchuk_leer(nunchuk_data);
	
	glcd_inicializar();

	while(1){
		

		int8_t	joystick	= (int8_t)nunchuk_data[0] + 120;
		bool_t	boton_c 	= !(nunchuk_data[5] & 2u),
						boton_z 	= !(nunchuk_data[5] & 1u);
		
		int8_t	direccion = (joystick > 30) - (joystick < -30);
		
		posicion += direccion;
		posicion = posicion * (posicion > 2 && posicion < max_x) + 3 * (posicion <=2) + (max_x - 1) * (posicion >= max_x);
		
		dibujar_sprite(sprite_jugador, posicion, altura, BLANCO);
		
		nunchuk_leer(nunchuk_data);
		timer_retardo_ms(TIMER0, 10);
		
		dibujar_sprite(sprite_jugador, posicion, altura, NEGRO);
				
	}
}
