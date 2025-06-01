/*******************************************************************************
 * \file    tipos.h
 *
 * \brief   Definiciones de tipos para el compilador ARM.
 *
 * \detail  Basado en mc3_types.h de MISRA-C 2012.
 */

#ifndef TIPOS_H
#define TIPOS_H
 
#include <LPC407x_8x_177x_8x.h>

#define EA_GLCD
 
/*------------------------------------------------------------------------------
 * Incluir stddef.h, que contiene definiciones para NULL, size_t, ptrdiff_t
 * y wchar_t
 */

#include <stddef.h>

/*------------------------------------------------------------------------------
 * Si el compilador C99 estar�n disponible stdint.h y stdbool.h as� que los
 * aprovechamos. El tipo bool_t de define en base al _Bool de stdbool.h.
 */
#if defined ( __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901L ) /* C99 */

#include <stdint.h>
#include <stdbool.h>

#ifdef  __clang__

//Ignoramos errores de codificaci�n de caracteres ANSI en vez de UTF-8
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

#endif

typedef _Bool bool_t;

/*------------------------------------------------------------------------------
 * Si no estamos en un compilador C99 y no est� definido el s�mbolo __stdint_h,
 * definir los tipos enteros.
 */
#else

#ifndef __stdint_h

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

#endif  /* __stdint_h */

#endif  /* defined ( __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901L ) */

/*------------------------------------------------------------------------------
 * Si no est� definido el s�mbolo __bool_true_false_are_defined definir el tipo
 * bool_t como unsigned char. Si est� definido definir bool_t a partir de bool.
 */
#ifndef __bool_true_false_are_defined
typedef unsigned char       bool_t;
#else
typedef bool                bool_t;
#endif

/*------------------------------------------------------------------------------
 * Si no est�n definidor los s�mbolos FALSE y TRUE, definirlos.
 */
#ifndef FALSE
#define FALSE ((bool_t)0)
#endif

#ifndef TRUE
#define TRUE  ((bool_t)1)
#endif

/*------------------------------------------------------------------------------
 * Definir tipos para datos flotantes que indiquen claramente su tama�o.
 */
typedef float               float32_t;
typedef double              float64_t;

/* Los nombres de los registros de las interrupciones de los pines de los
 * puertos P0 y P2 del fichero LPC407x_8x_177x_8x.h no est�n actualizados
 * a la �ltima versi�n del manual del microcontrolador.
 * Aqu� se redefinen de forma que coincidan con el manual. */

typedef struct{
	__I  uint32_t STATUS;
	__I  uint32_t STATR0;
	__I  uint32_t STATF0;
	__O  uint32_t CLR0;
	__IO uint32_t ENR0;
	__IO uint32_t ENF0;
		 uint32_t RESERVED0[3];
	__I  uint32_t STATR2;
	__I  uint32_t STATF2;
	__O  uint32_t CLR2;
	__IO uint32_t ENR2;
	__IO uint32_t ENF2;
} LPC_GPIOINT_TypeDef_corregido;

#undef  LPC_GPIOINT
#define LPC_GPIOINT ((LPC_GPIOINT_TypeDef_corregido   *) LPC_GPIOINT_BASE  )

extern volatile uint32_t msTicks;

#endif  /* TIPOS_H */
