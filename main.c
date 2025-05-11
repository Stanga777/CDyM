#include "main.h"
#include "juego.h"
#include "lcd.h"
#include "keypad.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	LCDinit();      // Inicializa el LCD
	JuegoMEF_Init(); // Inicializa la máquina de estados

	while (1) {
		JuegoMEF_Update(); // Ejecuta la lógica del juego
		_delay_ms(10);     // Pequeña espera para evitar rebotes rápidos
	}

	return 0;
}
