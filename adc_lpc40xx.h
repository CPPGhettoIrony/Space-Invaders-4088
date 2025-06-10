/*******************************************************************************
	LA SIGUIENTE LIBRERÍA HA SIDO DESCARGADA DE LA UNIVERSIDAD DE CÁDIZ
	Autor original: Alejandro Lara Doña
	
 * \file    adc_lpc40xx.h
 *
 * \brief   Funciones de manejo del ADC del LPC40xx.
 */

#ifndef ADC_LPC40XX_H
#define ADC_LPC40XX_H

#include "tipos.h"

//===== Constantes =============================================================
 
#define SEL_CANAL_0 	(1u << 0)
#define SEL_CANAL_1 	(1u << 1)
#define SEL_CANAL_2 	(1u << 2)
#define SEL_CANAL_3 	(1u << 3)
#define SEL_CANAL_4 	(1u << 4)
#define SEL_CANAL_5 	(1u << 5)
#define SEL_CANAL_6 	(1u << 6)
#define SEL_CANAL_7 	(1u << 7)
#define INT_GLOBAL		(1u << 8)

#define MODO_BURST		(1u << 16)

#define ADC_STOP			(0x0 << 24)
#define START_AHORA		(0x1 << 24)
#define START_P2_10		(0x2 << 24)
#define START_P1_27		(0x3 << 24)
#define START_MAT0_1	(0x4 << 24)
#define START_MAT0_3	(0x5 << 24)
#define START_MAT1_0	(0x6 << 24)
#define START_MAT1_1	(0x7 << 24)

#define FLANCO_SUBIDA	(0u << 27)
#define FLANCO_BAJADA	(1u << 27)

//===== Prototipos de funciones ================================================

void adc_inicializar(uint32_t frecuencia_adc, uint32_t canales);
uint32_t adc_convertir(uint8_t canal);
float32_t adc_traducir_a_tension(uint32_t resultado_adc);
void adc_seleccionar_canales(uint32_t canales);
void adc_configurar_interrupciones(uint32_t canales);
void adc_modo_burst(bool_t estado);
void adc_modo_conversion(uint32_t modo, uint32_t flanco);

#endif  /* ADC_LPC40XX_H */
