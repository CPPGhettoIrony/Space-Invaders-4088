/***************************************************************************//**
 * \file    sonido.h
 *
 * \brief   Funciones básicas de sonido.
 */

#ifndef SONIDO_H
#define SONIDO_H

#include "tipos.h"

//===== Constantes =============================================================

#define SONIDO_TIMER TIMER2
#define SONIDO_TIMER_IRQn       				TIMER2_IRQn
#define SONIDO_TIMER_IRQHandler 				TIMER2_IRQHandler
#define SONIDO_PRIORIDAD_INTERRUPCION   31u

// Frecuencia [Hz] de las notas según la notación anglosajona
#define NOTE_END		1
#define NOTE_B0 		31	//Si
#define NOTE_C1 		33
#define NOTE_CS1 		35
#define NOTE_D1 		37
#define NOTE_DS1 		39
#define NOTE_E1 		41
#define NOTE_F1 		44
#define NOTE_FS1 		46
#define NOTE_G1 		49
#define NOTE_GS1 		52
#define NOTE_A1 		55	//La
#define NOTE_AS1 		58
#define NOTE_B1 		62
#define NOTE_C2 		65
#define NOTE_CS2 		69
#define NOTE_D2 		73
#define NOTE_DS2 		78
#define NOTE_E2 		82
#define NOTE_F2 		87
#define NOTE_FS2 		93
#define NOTE_G2 		98
#define NOTE_GS2 		104
#define NOTE_A2 		110
#define NOTE_AS2 		117
#define NOTE_B2 		123
#define NOTE_C3 		131
#define NOTE_CS3 		139
#define NOTE_D3 		147
#define NOTE_DS3 		156
#define NOTE_E3 		165
#define NOTE_F3 		175
#define NOTE_FS3 		185
#define NOTE_G3 		196
#define NOTE_GS3 		208
#define NOTE_A3 		220
#define NOTE_AS3 		233
#define NOTE_B3 		247
#define NOTE_C4 		262
#define NOTE_CS4 		277
#define NOTE_D4 		294
#define NOTE_DS4 		311
#define NOTE_E4 		330
#define NOTE_F4 		349
#define NOTE_FS4 		370
#define NOTE_G4 		392
#define NOTE_GS4 		415
#define NOTE_A4 		440
#define NOTE_AS4 		466
#define NOTE_B4 		494
#define NOTE_C5 		523
#define NOTE_CS5 		554
#define NOTE_D5 		587
#define NOTE_DS5 		622
#define NOTE_E5 		659
#define NOTE_F5 		698
#define NOTE_FS5 		740
#define NOTE_G5 		784
#define NOTE_GS5 		831
#define NOTE_A5 		880
#define NOTE_AS5 		932
#define NOTE_B5 		988
#define NOTE_C6 		1047
#define NOTE_CS6 		1109
#define NOTE_D6 		1175
#define NOTE_DS6 		1245
#define NOTE_E6 		1319
#define NOTE_F6 		1397
#define NOTE_FS6 		1480
#define NOTE_G6 		1568
#define NOTE_GS6 		1661
#define NOTE_A6 		1760
#define NOTE_AS6 		1865
#define NOTE_B6 		1976
#define NOTE_C7 		2093
#define NOTE_CS7 		2217
#define NOTE_D7 		2349
#define NOTE_DS7 		2489
#define NOTE_E7 		2637
#define NOTE_F7 		2794
#define NOTE_FS7  	2960
#define NOTE_G7  		3136
#define NOTE_GS7 		3322
#define NOTE_A7  		3520
#define NOTE_AS7 		3729
#define NOTE_B7  		3951
#define NOTE_C8  		4186
#define NOTE_CS8 		4435
#define NOTE_D8  		4699
#define NOTE_DS8		4978
#define NOTE_PAUSE  0		// Silencio

/* Array de conversión de nota a semiperiodo en microsegundos.
 * El índice 0 no se usa porque la nota 0 indicará silencio.
 */
static const uint16_t nota_a_semiperiodo_us[] = {
        0,
    16198,  // B0
    15289,  // C1
    14431,  // CS1
    13621,  // D1
    12856,  // DS1
    12135,  // E1
    11454,  // F1
    10811,  // FS1
    10204,  // G1
     9631,  // GS1
     9091,  // A1
     8581,  // AS1
     8099,  // B1
     7645,  // C2
     7215,  // CS2
     6810,  // D2
     6428,  // DS2
     6067,  // E2
     5727,  // F2
     5405,  // FS2
     5102,  // G2
     4816,  // GS2
     4545,  // A2
     4290,  // AS2
     4050,  // B2
     3822,  // C3
     3608,  // CS3
     3405,  // D3
     3214,  // DS3
     3034,  // E3
     2863,  // F3
     2703,  // FS3
     2551,  // G3
     2408,  // GS3
     2273,  // A3
     2145,  // AS3
     2025,  // B3
     1911,  // C4
     1804,  // CS4
     1703,  // D4
     1607,  // DS4
     1517,  // E4
     1432,  // F4
     1351,  // FS4
     1276,  // G4
     1204,  // GS4
     1136,  // A4
     1073,  // AS4
     1012,  // B4
      956,  // C5
      902,  // CS5
      851,  // D5
      804,  // DS5
      758,  // E5
      716,  // F5
      676,  // FS5
      638,  // G5
      602,  // GS5
      568,  // A5
      536,  // AS5
      506,  // B5
      478,  // C6
      451,  // CS6
      426,  // D6
      402,  // DS6
      379,  // E6
      358,  // F6
      338,  // FS6
      319,  // G6
      301,  // GS6
      284,  // A6
      268,  // AS6
      253,  // B6
			239,  // C7
      225,  // CS7
      213,  // D7
      201,  // DS7
      190,  // E7
      179,  // F7
      169,  // FS7
      159,  // G7
      150,  // GS7
      142,  // A7
      134,  // AS7
      127,  // B7
      119   // C8
};

typedef struct{
  uint16_t tempo;  
	uint16_t numero_notas;
	const uint16_t *ptr_notas;
	const int8_t *ptr_duracion_nota; 
	/* 4	-> quarter note,
	 * 8	-> eighteenth,
	 * 16	-> sixteenth, etc
	 * !!negative numbers are used to represent dotted notes,
	 * -4	-> dotted quarter note, that is, a quarter plus an eighteenth
	 */
} melodia_t;

//===== Prototipos de funciones ================================================

void sonido_emitir_pitido(uint32_t frecuencia, uint32_t duracion_ms);
void sonido_inicializar(void);
void sonido_reproducir_nota(uint8_t nota, uint32_t duracion_ms);
void sonido_reproducir_melodia(const uint8_t *melodia, uint32_t duracion_nota_ms);
void sonido_iniciar_melodia(const melodia_t melodia);

#endif  /* SONIDO_H */
