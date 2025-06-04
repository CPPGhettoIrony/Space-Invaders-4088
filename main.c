/*******************************************************************************
 *\author			Alejandro Lara Doña
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
#include "glcd.h"
#include "nunchuk.h"
#include "timer_lpc40xx.h"

//Prototipos de funciones
void bajo_consumo(void);

int main(){
	
	//Habilitamos las interrupciones globalmente
	__enable_irq();
	
	//Definimos si necesitamos alguna variable local en el main
	uint8_t nunchuk_data[6];
	nunchuk_inicializar();
	
	timer_inicializar(TIMER0);
	timer_retardo_us(TIMER0, 10);
	
	//Inicializamos el resto de periféricos: i2c, uart, lcd, etc.
	glcd_inicializar();

	while(1){
		
		nunchuk_leer(nunchuk_data);
		
		glcd_xprintf(0, 0, BLANCO, NEGRO, FUENTE8X16, "Nunchuk:  [%u, %u, %u, %u, %u, %u]", 
								nunchuk_data[0], nunchuk_data[1], nunchuk_data[2], nunchuk_data[3], nunchuk_data[4], nunchuk_data[5]);	

		timer_retardo_us(TIMER0, 10);
				
	}
}
