#include "nunchuk.h"

void nunchuk_inicializar() {
	
	// Inicializar I2C
	i2c_inicializar(I2C0, 100000, PORT0, PIN27, PORT0, PIN28);
	
	i2c_start(I2C0);
	
	i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);
	i2c_transmitir_byte(I2C0, 0x40);
	i2c_transmitir_byte(I2C0, 0x00);
	
	i2c_stop(I2C0);
}

void nunchuk_leer(uint8_t* data) {
	
	i2c_start(I2C0);
	i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);
	i2c_transmitir_byte(I2C0, 0x00);
	i2c_stop(I2C0);
	
	i2c_start(I2C0);
	i2c_transmitir_byte(I2C0, NUNCHUK_DIRECCION);
	for(uint8_t i = 0; i < 6; ++i)
		data[i] = i2c_recibir_byte(I2C0, (bool_t)(i <= 4));
	i2c_stop(I2C0);
	
}