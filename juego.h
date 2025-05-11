#ifndef JUEGO_H
#define JUEGO_H

#include <stdint.h>

// Estados de la máquina de estados del juego
typedef enum {
	INICIO,
	MOSTRAR_CLAVE,
	ESPERAR_INGRESO,
	VERIFICAR_CARACTER,
	VICTORIA,
	DERROTA
} EstadoJuego;

extern EstadoJuego estado;
extern volatile uint16_t milisegundos;

// Funciones principales del juego
void JuegoMEF_Init(void);
void JuegoMEF_Update(void);

#endif // JUEGO_H
