#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include "avr/pgmspace.h"
#define F_CPU 16000000UL
#include <util/delay.h>

#include "lcd.h"
#include "keypad.h"
#include <avr/interrupt.h>
#include <stdint.h>

// ---------- Definiciones ----------
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_PIN PB0

#define TICK_MS 10      // Cada interrupción vale 10 ms
#define TICKS_2S 200    // 200 * 10 ms = 2 segundos

// ---------- Enumeración de Estados ----------
typedef enum {
	INICIO,
	MUESTRA,
} Estado_t;

// ---------- Variables globales ----------
extern volatile uint8_t flag_tick;
extern Estado_t estado;
extern uint16_t contador;
extern uint8_t mostrar_mensaje;

// ---------- Funciones ----------
void init_timer0(void);
void init_IO(void);
void init_LCD(void);
void MEF(void);

#endif /* MAIN_H_ */
