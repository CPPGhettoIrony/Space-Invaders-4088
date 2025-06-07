#include "nunchuk.h"

bool_t nunchuk_inicializar() {
	
	// Inicializar I2C
	i2c_inicializar(I2C0, 100000, PORT0, PIN27, PORT0, PIN28);
	
	timer_inicializar(TIMER0);
	
	i2c_start(I2C0);
	bool_t a0 = i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);  // Dirección + write
	bool_t a1 = i2c_transmitir_byte(I2C0, 0xF0);
	bool_t a2 = i2c_transmitir_byte(I2C0, 0x55);
	i2c_stop(I2C0);

	timer_retardo_ms(TIMER0, 10); // espera obligatoria

	i2c_start(I2C0);
	bool_t a3 = i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);  // Dirección + write
	bool_t a4 = i2c_transmitir_byte(I2C0, 0xFB);
	bool_t a5 = i2c_transmitir_byte(I2C0, 0x00);
	i2c_stop(I2C0);

	timer_retardo_ms(TIMER0, 10);
	
	return a0 && a1 && a2 && a3 && a4 && a5;
}

bool_t nunchuk_leer(uint8_t* data) {
	
	i2c_start(I2C0);
	bool_t a0 = i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);
	bool_t a1 = i2c_transmitir_byte(I2C0, 0x00);
	i2c_stop(I2C0);
	
	timer_retardo_ms(TIMER0, 1);
	
	i2c_start(I2C0);
	bool_t a2 = i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION | 1);
	for(uint8_t i = 0; i < 6; ++i)
		data[i] = i2c_recibir_byte(I2C0, (bool_t)(i < 5));
	i2c_stop(I2C0);
	
	return a0 && a1 && a2;
	
}