#include "jugador.h"

uint8_t sprite_jugador[8] = {
	0b00011000,
	0b00011000,
	0b00111100,
	0b00111100,
	0b01100110,
	0b01000010,
	0b11000011,
	0b11111111
};

jugador jugador_crear() {
	jugador ret = {sprite_jugador, 3};
	return ret;
}

void jugador_actualizar(jugador* j, uint8_t* nunchuk_data) {
	
	int8_t	joystick	= (int8_t)nunchuk_data[0] + 120;
	bool_t	boton_c 	= !(nunchuk_data[5] & 2u),
					boton_z 	= !(nunchuk_data[5] & 1u);
	
	int8_t	direccion = (joystick > 30) - (joystick < -30);
	
	j->posicion += direccion;
	j->posicion = j->posicion * (j->posicion > 2 && j->posicion < max_x) + 3 * (j->posicion <=2) + (max_x - 1) * (j->posicion >= max_x);
	
}