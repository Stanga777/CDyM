#include "juego.h"
#include "lcd.h"
#include "keypad.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>  // Para rand()
#include <string.h>  // Para strlen()

// Mapa lógico del teclado
const char mapa_teclas[16] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'*', '0', '#', 'D'
};

const char* palabras[] = {
	"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro",
	"Lanza", "Nieve", "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa", "SERIE",
	"SalUd", "Salud", "Silla", "Tecla", "Valor", "Verde", "YnHRz", "hARdD", "silla"
};

EstadoJuego estado;

// Variables internas del juego
static uint8_t clave_ascii[10];      // Clave convertida a dígitos ASCII (5 letras x 2 dígitos)
static uint8_t ingreso[10];          // Ingreso del jugador
static uint8_t indice = 0;           // Índice del ingreso
volatile uint16_t milisegundos = 0;  // Timer control
static const char* palabra_actual;   // Palabra seleccionada
volatile uint16_t errores = 0;

// =================== Timer0 ===================
void Timer0_Init(void) {
	TCCR0A = (1 << WGM01);             // Modo CTC
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
	OCR0A = 249;                       // Comparar cada 1ms (con 16 MHz)
	TIMSK0 = (1 << OCIE0A);           // Habilitar interrupción por comparación
	sei();                            // Habilitar interrupciones globales
}

ISR(TIMER0_COMPA_vect) {
	milisegundos++;
}

// =================== Inicialización ===================
void JuegoMEF_Init(void) {
	LCDclr();
	LCDGotoXY(0, 0);
	LCDstring((uint8_t*)"Presione '*'", 13);

	Timer0_Init();
	estado = INICIO;
}

// =================== Selección y conversión de palabra ===================
void SeleccionarYConvertirPalabra(void) {
	palabra_actual = palabras[rand() % (sizeof(palabras) / sizeof(palabras[0]))];

	for (uint8_t i = 0; i < 5; i++) {
		uint8_t ascii = (uint8_t)palabra_actual[i];
		clave_ascii[2 * i]     = ascii / 10;
		clave_ascii[2 * i + 1] = ascii % 10;
	}
}

// =================== Lógica del Juego ===================
void JuegoMEF_Update(void) {
	uint8_t tecla;

	switch (estado) {
		case INICIO:
			if (KEYPAD_Scan(&tecla)) {
				char caracter = mapa_teclas[tecla];
				if (caracter == '*') {
					SeleccionarYConvertirPalabra();
					milisegundos = 0;
					estado = MOSTRAR_CLAVE;
				}
			}
		break;

		case MOSTRAR_CLAVE:
			if (milisegundos == 0) {
				LCDclr();
				LCDGotoXY(0, 0);
				LCDstring((uint8_t*)palabra_actual, strlen(palabra_actual));
			}

			if (milisegundos >= 2000) {
				milisegundos = 0;
				LCDclr();
				indice = 0;
				estado = ESPERAR_INGRESO;
			}
		break;

		case ESPERAR_INGRESO:
			if (KEYPAD_Scan(&tecla)) {
				char caracter = mapa_teclas[tecla];
				if (caracter >= '0' && caracter <= '9') {
					ingreso[indice] = caracter - '0';
					LCDsendChar(caracter);
					indice++;

					if (indice == 10) {
						estado = VERIFICAR_CARACTER;
					}
				}
			}
		break;

		case VERIFICAR_CARACTER: {
			uint8_t correcto = 1;
			for (uint8_t i = 0; i < 10; i++) {
				if (ingreso[i] != clave_ascii[i]) {
					correcto = 0;
					break;
				}
			}
			
			if (correcto)
			{
				estado = VICTORIA
			} 
			else
			{
				errores++;
				if (errores=3)
				{
					estado = DERROTA 
				}
			}
			
			break;
		}

		case VICTORIA:
			LCDclr();
			LCDGotoXY(0, 0);
			LCDstring((uint8_t*)"\x01Ganaste!", 9);
			
			if (milisegundos >= 5000) {
				milisegundos = 0;
				LCDclr();
				indice = 0;
				estado = INICIO;
			}
		break;

		case DERROTA:
			LCDclr();
			LCDGotoXY(0, 0);
			LCDstring((uint8_t*)"Clave incorrecta", 16);
			estado = GENERAR_CLAVE;
		break;
	}
}
