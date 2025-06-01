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
#include "timer_lpc40xx.h"

//Prototipos de funciones
void bajo_consumo(void);

int main(){
	
	//Definimos si necesitamos alguna variable local en el main
	
	//Desconectamos los periféricos no utilizados para disminuir el consumo
	bajo_consumo();
	
	//Inicializamos el resto de periféricos: i2c, uart, lcd, etc.
	glcd_inicializar();
	
  
	//Habilitamos las interrupciones globalmente
	__enable_irq();
	
	while(1){
		__wfi();	//Pasamos al modo Sleep hasta que ocurra una interrupción pero 
							//siguen funcionando los periféricos (PWM, I2C, etc)
					
		//__wfe(); 	//Pasamos al modo DeepSleep hasta que ocurra una interrupción 
                //NMI, EINT0-3, GPIO, RTC y demás recogidas en el 3.12.2. Se 
                //detiene la ejecución de todos los periféricos excepto el RTC
	}
}

/*******************************************************************************
 * \brief   Función para disminuir el consumo apagando los periféricos
 *					encendidos tras el reset. Tabla 16 del manual (sección 3.3.2.2)
 *          b1 TIMER0 
 *          b2 TIMER1
 *          b3 UART0
 *          b4 UART1
 *          b7 I2C0
 *          b9 RTC
 *          b19 I2C1
 *          b26 I2C2
 */
void bajo_consumo(void){
	LPC_SC->PCONP &= (LPC_SC->PCONP & ~((1u<<1)|(1u<<2)|      //TIMER0  TIMER1
                                      (1u<<3)|(1u<<4)|      //UART0   UART1
                                      (1u<<7)|(1u<<9)|      //I2C0    RTC
                                      (1u<<19)|(1u<<26)));  //I2C1    I2C2
	LPC_SC->PCONP1 = 0;//Comparador
}
