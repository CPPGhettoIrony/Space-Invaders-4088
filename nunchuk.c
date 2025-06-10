// Librería original del repositorio, escrita por Manuel CB
#include "nunchuk.h"

// Inicializar la comunicación I2C con el nunchuk
bool_t nunchuk_inicializar() {
	
	// Inicializar I2C
	i2c_inicializar(I2C0, 100000, PORT0, PIN27, PORT0, PIN28);
	
	// Handshake para inicializar la comunicación I2C 
	// Se efectúan dos handshakes debido a la naturaleza no oficial del nunchuk para inicializar la comunicación con este
	
	i2c_start(I2C0);
	bool_t a0 = i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);  
	bool_t a1 = i2c_transmitir_byte(I2C0, 0xF0);
	bool_t a2 = i2c_transmitir_byte(I2C0, 0x55);
	i2c_stop(I2C0);

	timer_retardo_ms(TIMER0, 10); 	// Espera obligatoria, darle tiempo al nunchuk para procesar

	i2c_start(I2C0);
	bool_t a3 = i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);  
	bool_t a4 = i2c_transmitir_byte(I2C0, 0xFB);
	bool_t a5 = i2c_transmitir_byte(I2C0, 0x00);
	i2c_stop(I2C0);

	timer_retardo_ms(TIMER0, 10);		// Otra espera obligatoria
	
	// Si alguna transferencia ha fallado (se recibe un NACK), la función retorna cero
	return a0 && a1 && a2 && a3 && a4 && a5;
}

// Leer el estado de los giroscopios, botones y el joystick del nunchuk
bool_t nunchuk_leer(uint8_t* data) {
	
	// Transmisión necesaria para inicializar la transferencia de datos
	// Básicamente le estamos pidiendo al nunchuk que nos transmita la información explicada con anterioridad
	i2c_start(I2C0);
	bool_t a0 = i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);
	bool_t a1 = i2c_transmitir_byte(I2C0, 0x00);
	i2c_stop(I2C0);
	
	timer_retardo_ms(TIMER0, 1);	// Espera obligatoria, por los mismos motivos

	// Recibir los datos
	i2c_start(I2C0);
	bool_t a2 = i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION | 1);
	for(uint8_t i = 0; i < 6; ++i)
		data[i] = i2c_recibir_byte(I2C0, (bool_t)(i < 5));				// Hacemos 5 ACKs y un NACK para indicar el fin de la transmisión
	i2c_stop(I2C0);
	
	// Si alguna transferencia ha sido rechazada (NACK), se devuelve cero
	return a0 && a1 && a2;
	
}