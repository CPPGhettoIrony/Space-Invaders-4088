/*********************************************************************
 * \file    ejemplo.h
 * \brief   Descripción de la libreria.
 */
 
#ifndef EJEMPLO_H // IMPORTANTE CAMBIARLO Y QUE SEA EN CADA .h DIFERENTE
#define EJEMPLO_H

#include <LPC407x_8x_177x_8x.h>
#include "tipos.h"

#define ETIQUETA1		"OK"
#define ETIQUETA2		"ERROR"

//====== Prototipos de funciones =====================================
void dispositivo_i2c(LPC_I2C_TypeDef *i2c_regs, uint32_t frecuencia_scl,                       
										 LPC_GPIO_TypeDef *puerto_sda, uint32_t mascara_pin_sda,
                     LPC_GPIO_TypeDef *puerto_scl, uint32_t mascara_pin_scl,
                     uint8_t direccion_i2c_dispositivo);

void dispositivo_escribir_registro(uint8_t registro_a_escribir, uint16_t dato_a_escribir);
uint16_t dispositivo_leer_registro(uint8_t registro_a_leer);
float dispositivo_funcion1(void);


#endif  /* EJEMPLO_H */
