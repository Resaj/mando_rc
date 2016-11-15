#ifndef DECLARACIONES_H
#define DECLARACIONES_H

#include <stdbool.h>
#include "LPClib.h"


// Variables auxiliares
extern float /*cont_1,*/ cont_2, servo_enable;
extern char /*blinky_1,*/ blinky_2;
extern int ref_v1, ref_v2;
extern char servo_pos;
extern char servo_pos_ant;


// Prioridades de interrupciones
#define UART_PRIO	0			// Prioridad de la uart
#define MATCH_PRIO	1			// Prioridad del timer en modo match para el control periódico

// Batería
extern struct analog BAT_ADC;	// Número y canal de ADC para la batería

// Joysticks
extern struct analog JV1;		// Número y canal de ADC para el potenciómetro vertical del joystick 1
extern struct analog JH1;		// Número y canal de ADC para el potenciómetro horizontal del joystick 1
extern struct data JB1;			// Puerto y pin del pulsador del joystick 1
extern struct analog JV2;		// Número y canal de ADC para el potenciómetro vertical del joystick 2
extern struct analog JH2;		// Número y canal de ADC para el potenciómetro horizontal del joystick 2
extern struct data JB2;			// Puerto y pin del pulsador del joystick 2

// Potenciómetros
extern struct analog POT_1;		// Número y canal de ADC para el potenciómetro 1
extern struct analog POT_2;		// Número y canal de ADC para el potenciómetro 2

// Pulsadores
extern struct data BUTTON_B1;	// Puerto y pin del pulsador 1
extern struct data BUTTON_B2;	// Puerto y pin del pulsador 2

// Leds
extern struct data LED_1;		// Puerto y pin del led 1
extern struct data LED_2;		// Puerto y pin del led 2
extern struct data LED_3;		// Puerto y pin del led 3
extern struct data LED_4;		// Puerto y pin del led 4
extern struct data LED_5;		// Puerto y pin del led 5
extern struct data LED_6;		// Puerto y pin del led 6

// Timers
#define MATCH_TIMER		TIMER0		// Timer a habilitar como MATCH
#define PERIODIC_CH		MATCH0_1	// Canal de MATCH de la interrupción periódica


// Función general de configuración
void config_MANDO(void);

// Funciones de configuración de los dispositivos
void config_BATTERY_ADC(void);
void config_JOYSTICKS(void);
void config_POTENCIOMETERS(void);
void config_ADC(void);
void config_BUTTONS(void);
void config_LEDS(void);
void config_PERIODIC_CONTROL(void);


#endif
