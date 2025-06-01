/*******************************************************************************
 *\author			Alejandro Lara Do�a
 *
 *\file 			main.c
 *
 *\brief			Modo sleep del LPC40xx (secci�n 3.12 del manual) y 
 *						deshabilitar perif�ricos activados por defecto
 *						(secci�n 3.3.2.2 del manual)
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
	
	//Desconectamos los perif�ricos no utilizados para disminuir el consumo
	bajo_consumo();
	
	//Inicializamos el resto de perif�ricos: i2c, uart, lcd, etc.
	glcd_inicializar();
	
  
	//Habilitamos las interrupciones globalmente
	__enable_irq();
	
	while(1){
		__wfi();	//Pasamos al modo Sleep hasta que ocurra una interrupci�n pero 
							//siguen funcionando los perif�ricos (PWM, I2C, etc)
					
		//__wfe(); 	//Pasamos al modo DeepSleep hasta que ocurra una interrupci�n 
                //NMI, EINT0-3, GPIO, RTC y dem�s recogidas en el 3.12.2. Se 
                //detiene la ejecuci�n de todos los perif�ricos excepto el RTC
	}
}

/*******************************************************************************
 * \brief   Funci�n para disminuir el consumo apagando los perif�ricos
 *					encendidos tras el reset. Tabla 16 del manual (secci�n 3.3.2.2)
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
