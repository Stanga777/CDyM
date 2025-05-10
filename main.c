#include "main.h"

// ---------- Variables globales ----------
volatile uint8_t flag_tick = 0;
Estado_t estado = INICIO;
uint16_t contador = 0;
uint8_t mostrar_mensaje = 1;
uint8_t sistema_activo = 0;

// ---------- Interrupción del TIMER0 cada 10ms ----------
ISR(TIMER0_COMPA_vect) {
	flag_tick = 1;
}

// ---------- Inicialización del Timer0 ----------
void init_timer0(void) {
	TCCR0A = (1 << WGM01);                  // Modo CTC
	TCCR0B = (1 << CS01) | (1 << CS00);     // Prescaler 64
	OCR0A = 249;                            // 10 ms
	TIMSK0 |= (1 << OCIE0A);                // Habilita interrupción por comparación
	sei();                                  // Habilita interrupciones globales
}

// ---------- Inicialización de I/O ----------
void init_IO(void) {
	LED_DDR |= (1 << LED_PIN);
	LED_PORT &= ~(1 << LED_PIN);
}

// ---------- Inicialización del LCD ----------
void init_LCD(void) {
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_gotoxy(0, 0);
	lcd_puts("Presione *");
}

// ---------- Máquina de Estados ----------
void MEF(void) {
	switch (estado) {
		case INICIO:
		if (mostrar_mensaje) {
			lcd_clrscr();
			lcd_gotoxy(0, 0);
			lcd_puts("Esperando 2s...");
			mostrar_mensaje = 0;
		}

		contador++;
		if (contador >= TICKS_2S) {
			contador = 0;
			estado = PARPADEO;
		}
		break;

		case ESPERA:
		LED_PORT ^= (1 << LED_PIN);
		estado = ESPERA;
		mostrar_mensaje = 1;
		break;
	}
}

// ---------- Programa Principal ----------
int main(void) {
	init_IO();
	init_LCD();
	init_timer0();
	keypad_init();

	while (1) {
		// Esperar a que se presione '*'
		if (!sistema_activo) {
			char tecla = keypad_getkey();
			if (tecla == '*') {
				sistema_activo = 1;
				lcd_clrscr();
				lcd_gotoxy(0, 0);
				lcd_puts("Sistema activo");
				_delay_ms(1000);
				lcd_clrscr();
			}
		}

		// Ejecutar la MEF solo si el sistema está activo
		if (sistema_activo && flag_tick) {
			flag_tick = 0;
			MEF();
		}
	}
}
