#include <lpc213x.h>
#include <stdbool.h>
#include <stdio.h>
#include "LPClib.h"
#include "declaraciones.h"
#include "target.h"


/************************ VARIABLES ************************/
// Variables auxiliares
float /*cont_1 = 0,*/ cont_2 = 0, servo_enable = 0; 
char /*blinky_1 = '0',*/ blinky_2 = '0';
int ref_v1 = 0, ref_v2 = 0;
char servo_pos = 'D';
char servo_pos_ant = 'D';

// Batería
struct analog BAT_ADC	= {ADC_1,ADC_CH7};	// Número y canal de ADC para la batería

// Joysticks
struct analog JV1	= {ADC_1,ADC_CH4};	// Número y canal de ADC para el potenciómetro vertical del joystick 1
struct analog JH1	= {ADC_1,ADC_CH2};	// Número y canal de ADC para el potenciómetro horizontal del joystick 1
struct data JB1		= {0,14};			// Puerto y pin del pulsador del joystick 1
struct analog JV2	= {ADC_0,ADC_CH0};	// Número y canal de ADC para el potenciómetro vertical del joystick 2
struct analog JH2	= {ADC_0,ADC_CH1};	// Número y canal de ADC para el potenciómetro horizontal del joystick 2
struct data JB2		= {0,25};			// Puerto y pin del pulsador del joystick 2

// Potenciómetros
struct analog POT_1	= {ADC_1,ADC_CH5};	// Número y canal de ADC para el potenciómetro 1
struct analog POT_2	= {ADC_1,ADC_CH6};	// Número y canal de ADC para el potenciómetro 2

// Pulsadores
struct data BUTTON_B1	= {0,8};		// Puerto y pin del pulsador 1
struct data BUTTON_B2	= {0,30};		// Puerto y pin del pulsador 2

// Leds
struct data LED_1	= {0,9};			// Puerto y pin del led 1
struct data LED_2	= {1,23};			// Puerto y pin del led 2
struct data LED_3	= {1,24};			// Puerto y pin del led 3
struct data LED_4	= {1,31};			// Puerto y pin del led 4
struct data LED_5	= {0,29};			// Puerto y pin del led 5
struct data LED_6	= {1,16};			// Puerto y pin del led 6


/************************ FUNCIONES DE CONFIGURACIÓN ************************/
void config_MANDO(void)
{
	config_ADC();
	config_BUTTONS();
	config_LEDS();
	UART0_config(115200, UART_PRIO);
	config_PERIODIC_CONTROL();
}

void config_BATTERY_ADC(void)
{
	ADC_CH_config(BAT_ADC);
}

void config_JOYSTICKS(void)
{
	ADC_CH_config(JV1);
	ADC_CH_config(JH1);
	GPIO_config(JB1, IN);
	ADC_CH_config(JV2);
	ADC_CH_config(JH2);
	GPIO_config(JB2, IN);
}

void config_POTENCIOMETERS(void)
{
	ADC_CH_config(POT_1);
	ADC_CH_config(POT_2);
}

void config_ADC(void)
{
	config_BATTERY_ADC();
	config_JOYSTICKS();
	config_POTENCIOMETERS();

	ADC_enable(ADC_0);
	ADC_enable(ADC_1);
}

void config_BUTTONS(void)
{
	GPIO_config(BUTTON_B1, IN);
	GPIO_config(BUTTON_B2, IN);
}

void config_LEDS(void)
{
	GPIO_config(LED_1, OUT);
	GPIO_config(LED_2, OUT);
	GPIO_config(LED_3, OUT);
	GPIO_config(LED_4, OUT);
	GPIO_config(LED_5, OUT);
	GPIO_config(LED_6, OUT);
}

void config_PERIODIC_CONTROL(void)
{
	MATCH_CHANNEL_config(PERIODIC_CH, PER_PERIOD);
	TIMER_config(MATCH_TIMER, MATCH_MODE, MATCH_PRIO);
}


/************************** FUNCIÓN MAIN **************************/
int main (void)
{
	int i = 0;

	config_MANDO();

	for(i=0; i<10; i++)
	{
		ref_v1 += ADC_read(JV1);
		ref_v2 += ADC_read(JV2);
	}

	ref_v1 = ref_v1/10;
	ref_v2 = ref_v2/10;

	LED_set(LED_1, ON);

	while(1);
}
