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
	
	//Definimos si necesitamos alguna variable local en el main
	uint8_t nunchuk_data[6] = {0,0,0,0,0,0};
	bool_t inicializado = nunchuk_inicializar();
	
	
	glcd_inicializar();

	while(1){
		
		bool_t lectura = nunchuk_leer(nunchuk_data);
		
		glcd_xprintf(0, 0, BLANCO, NEGRO, FUENTE8X16, "Inicialización: %u\r\nLectura: %u\n\rNunchuk:  [%u, %u, %u, %u, %u, %u]", 
								inicializado, lectura, nunchuk_data[0], nunchuk_data[1], nunchuk_data[2], nunchuk_data[3], nunchuk_data[4], nunchuk_data[5]);	
				
	}
}
