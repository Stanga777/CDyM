#include <avr/io.h>
#include "keypad.h"

// Macro para escribir filas (F0–F3)
#define ESCRIBIR_FILAS(Data) \
PORTB = (PORTB & 0xE6) \
| ((Data & (1<<0)) << 4)  /* F0 ? PB4 */ \
| ((Data & (1<<1)) << 2)  /* F1 ? PB3 */ \
| ((Data & (1<<2)) >> 2); /* F2 ? PB0 */ \
PORTD = (PORTD & 0x7F) \
| ((Data & (1<<3)) << 4)  /* F3 ? PD7 */

// data			1111-1111
// ~(1<<0)		1111-1110
// ~(1<<1)		1111-1101
// ~(1<<2)		1111-1011
// ~(1<<3)		1111-0111


// Macro para leer columnas (C0–C3)
#define LEER_COLUMNAS() \
((((PIND >> 3) & 0x01) << 0) | \
(((PIND >> 5) & 0x01) << 1) | \
(((PIND >> 4) & 0x01) << 2) | \
(((PIND >> 2) & 0x01) << 3))

void init_keypad(void) {
	// Filas (salida)
	DDRB |= (1<<4) | (1<<3) | (1<<0);
	DDRD |= (1<<7);
	
	// Columnas (entrada con pull-up)
	DDRD &= ~((1<<3)|(1<<5)|(1<<4)|(1<<2));
	PORTD |= (1<<3)|(1<<5)|(1<<4)|(1<<2);
}

uint8_t KepadUpdate(void) {
	uint8_t fila, columnas;

	for(fila = 0; fila < 4; fila++) {
		ESCRIBIR_FILAS(~(1<<fila));
		columnas = LEER_COLUMNAS();

		if(columnas != 0x0F) {
			for(uint8_t col = 0; col < 4; col++) {
				if (!(columnas & (1<<col)))
				return fila * 4 + col;
			}
		}
	}
	return 0xFF; // Ninguna tecla
}

uint8_t KEYPAD_Scan(uint8_t *pkey) {
	static uint8_t Old_key = 0xFF, Last_valid_key = 0xFF;
	uint8_t Key = KepadUpdate();

	if (Key == 0xFF) {
		Old_key = 0xFF;
		Last_valid_key = 0xFF;
		return 0;
	}

	if (Key == Old_key) {
		if (Key != Last_valid_key) {
			*pkey = Key;
			Last_valid_key = Key;
			return 1;
		}
	}

	Old_key = Key;
	return 0;
}
