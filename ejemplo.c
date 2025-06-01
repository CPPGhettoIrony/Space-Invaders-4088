/*********************************************************************
 * \file    ejemplo.c
 * \brief   Descripción de la libreria.
 */

#include <LPC407x_8x_177x_8x.h>
#include "tipos.h"
#include "i2c_lpc40xx.h"
#include "error.h"
#include "ejemplo.h"

static LPC_I2C_TypeDef *dispositivo_interfaz_i2c;
static uint8_t          dispositivo_dir_i2c;


/*******************************************************************************
 * \brief     Incializar la comunicación con el dispositivo.
 *
 * \param[in] i2c_regs									Puntero a regs. del interfaz a inicializar.
 * \param[in] frecuencia_scl						Frecuencia de la señal SCL.
 * \param[in] puerto_sda								Puerto que se desea para la función SDA.
 * \param[in] pin_sda										Pin que se desea para la función SDA.
 * \param[in] puerto_scl								Puerto que se desea para la función SCL.
 * \param[in] pin_scl										Pin que se desea para la función SCL.
 * \param[in] direccion_i2c_dispositivo	Dirección I2C del dispositivo.
 */
void dispositivo_i2c_inicializar(LPC_I2C_TypeDef *i2c_regs, uint32_t frecuencia_scl,                       
										 LPC_GPIO_TypeDef *puerto_sda, uint32_t mascara_pin_sda,
                     LPC_GPIO_TypeDef *puerto_scl, uint32_t mascara_pin_scl,
                     uint8_t direccion_i2c_dispositivo){
											 
}
/*******************************************************************************
 * \brief       Escribir en un registro interno del dispositivo.
 *
 * \param[in]   registro_a_escribir     Número del registro a escribir.
 * \param[in]   dato_a_escribir         Dato a escribir en el registro.
 */
void dispositivo_escribir_registro(uint8_t registro_a_escribir, uint16_t dato_a_escribir){
	
}

/*******************************************************************************
 * \brief       Leer un registro interno del dispositivo.
 *
 * \param[in]   registro_a_leer     Número del registro a leer.
 *
 * \return      Valor leído del registro.
 */
uint16_t dispositivo_leer_registro(uint8_t registro_a_leer){
	
	return UINT16_MAX;
}

/*******************************************************************************
 * \brief   Leer/escribir los registros necesarios y retornar el valor adecuado.
 *
 * \return 	Valor.
 */
float dispositivo_funcion1(void){
	
	return 0.0f;
}