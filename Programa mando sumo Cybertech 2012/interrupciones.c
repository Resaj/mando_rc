#ifndef INTERRUPCIONES_C
#define INTERRUPCIONES_C

#include <lpc213x.h>
#include <stdio.h>
#include <stdlib.h>
#include "LPClib.h"
#include "target.h"
#include "declaraciones.h"


void EINT_0(void)__irq
{

	EXTINT |= 1<<0;
	VICVectAddr = 0;
}

void EINT_1(void)__irq
{

	EXTINT |= 1<<1;
	VICVectAddr = 0;
}

void EINT_2(void)__irq
{

	EXTINT |= 1<<2;
	VICVectAddr = 0;
}

void EINT_3(void)__irq
{

	EXTINT |= 1<<3;
	VICVectAddr = 0;
}

void TIMER_0(void)__irq
{
	char mensaje[15];
	char /*button_j1 = OFF, button_j2 = OFF,*/ button_b1 = OFF, button_b2 = OFF;
	int jv1 = 0, /*jh1 = 0,*/ jv2 = 0, /*jh2 = 0,*/ pot_1 = 0, pot_2 = 0;
	int mot_1 = 0, mot_2 = 0;

	if(T0IR&(1<<0))
	{

		T0IR |= 1<<0;		// Activar flag de interrupción del Match0.0
	}
	else if(T0IR&(1<<1))
	{
		// Leer pulsadores, joysticks, potenciómetros y batería
		button_b1 = BUTTON_status(BUTTON_B1);
		button_b2 = BUTTON_status(BUTTON_B2);
//		button_j1 = BUTTON_status(JB1);
//		button_j2 = BUTTON_status(JB2);
	
		jv1 = ADC_read(JV1);
//		jh1 = ADC_read(JH1);
		jv2 = ADC_read(JV2);
//		jh2 = ADC_read(JH2);
	
		pot_1 = ADC_read(POT_1);
		pot_2 = ADC_read(POT_2);
		if(pot_1 < 100)
			pot_1 = 100;
		if(pot_2 < 100)
			pot_2 = 100;

//		if(cont_1 >= BAT_PERIOD)
//		{
//			blinky_1 = LIPO_TENSION_read(BAT_ADC, 3.2);
//		
//			cont_1 = 0;
//		}
//		cont_1 += PER_PERIOD;
	
		// Calcular velocidad de los motores y encender leds en función de la velocidad
		if(button_b1 == OFF)
		{
			if(jv1 < ref_v1)
				mot_1 = (ref_v1 - jv1)*255/ref_v1 * pot_1/1023;
			else
				mot_1 = (ref_v1 - jv1)*255/(1023 - ref_v1) * pot_1/1023;

			if(jv2 < ref_v2)
				mot_2 = (ref_v2 - jv2)*255/ref_v2 * pot_1/1023;
			else
				mot_2 = (ref_v2 - jv2)*255/(1023 - ref_v2) * pot_1/1023;
		}
		else
		{		
			if(jv1 < ref_v1)
				mot_1 = (ref_v1 - jv1)*255/ref_v1 * pot_2/1023;
			else
				mot_1 = (ref_v1 - jv1)*255/(1023 - ref_v1) * pot_2/1023;

			if(jv2 < ref_v2)
				mot_2 = (ref_v2 - jv2)*255/ref_v2 * pot_2/1023;
			else
				mot_2 = (ref_v2 - jv2)*255/(1023 - ref_v2) * pot_2/1023;
		}

		if(abs(mot_1) > 255*4/5 || abs(mot_2) > 255*4/5)
		{
			LED_set(LED_2, ON);
			LED_set(LED_3, ON);
			LED_set(LED_4, ON);
			LED_set(LED_5, ON);
		}
		else if(abs(mot_1) > 255*3/5 || abs(mot_2) > 255*3/5)
		{
			LED_set(LED_2, ON);
			LED_set(LED_3, ON);
			LED_set(LED_4, ON);
			LED_set(LED_5, OFF);
		}
		else if(abs(mot_1) > 255*2/5 || abs(mot_2) > 255*2/5)
		{
			LED_set(LED_2, ON);
			LED_set(LED_3, ON);
			LED_set(LED_4, OFF);
			LED_set(LED_5, OFF);
		}
		else if(abs(mot_1) > 255/5 || abs(mot_2) > 255/5)
		{
			LED_set(LED_2, ON);
			LED_set(LED_3, OFF);
			LED_set(LED_4, OFF);
			LED_set(LED_5, OFF);
		}
		else
		{
			LED_set(LED_2, OFF);
			LED_set(LED_3, OFF);
			LED_set(LED_4, OFF);
			LED_set(LED_5, OFF);
		}

		// Enviar velocidad de los motores y posición del servo
		sprintf(mensaje, "L%dR%dS", mot_1, mot_2);
		UART0_send(mensaje);
	
		if(button_b2 == ON && servo_enable >= 1.5 && servo_pos != 'U')
			servo_pos = 'U';
		else if(button_b2 == OFF && servo_enable >= 1.5 && servo_pos != 'D')
			servo_pos = 'D';

		if(servo_pos == 'U')
			UART0_send("U");
		else
			UART0_send("D");

		if(servo_pos != servo_pos_ant && servo_enable >= 1.5)
			servo_enable = 0;

		servo_enable += PER_PERIOD;
		servo_pos_ant = servo_pos;
	
		// Si las baterías están bajas, hacer parpadear un led
		if(/*blinky_1 == '1' ||*/ blinky_2 == '1')
		{
			if(cont_2 <= 0.1)
			{
//				if(blinky_1 == '1')
//					LED_set(LED_1, ON);
//				if(blinky_2 == '1')
					LED_set(LED_6, ON);
			}
			else if(cont_2 < 0.2)
			{
//				if(blinky_1 == '1')
//					LED_set(LED_1, OFF);
//				if(blinky_2 == '1')
					LED_set(LED_6, OFF);
			}
			else
				cont_2 = 0;

			cont_2 += PER_PERIOD;
		}

		T0IR |= 1<<1;		// Activar flag de interrupción del Match0.1
	}
	else if(T0IR&(1<<2))
	{

		T0IR |= 1<<2;		// Activar flag de interrupción del Match0.2
	}
	else if(T0IR&(1<<3))
	{

		T0IR |= 1<<3;		// Activar flag de interrupción del Match0.3
	}
	else if(T0IR&(1<<4))
	{

		T0IR |= 1<<4;		// Activar flag de interrupción del Capture0.0
	}
	else if(T0IR&(1<<5))
	{

		T0IR |= 1<<5;		// Activar flag de interrupción del Capture0.1
	}
	else if(T0IR&(1<<6))
	{

		T0IR |= 1<<6;		// Activar flag de interrupción del Capture0.2
	}
	else if(T0IR&(1<<7))
	{

		T0IR |= 1<<7;		// Activar flag de interrupción del Capture0.3
	}

	VICVectAddr = 0;	 
}

void TIMER_1(void)__irq
{
	if(T1IR&(1<<0))
	{

		T1IR |= 1<<0;		// Activar flag de interrupción del Match1.0
	}
	else if(T1IR&(1<<1))
	{

		T1IR |= 1<<1;		// Activar flag de interrupción del Match1.1
	}
	else if(T1IR&(1<<2))
	{

		T1IR |= 1<<2;		// Activar flag de interrupción del Match1.2
	}
	else if(T1IR&(1<<3))
	{

		T1IR |= 1<<3;		// Activar flag de interrupción del Match1.3
	}
	else if(T1IR&(1<<4))
	{

		T1IR |= 1<<4;		// Activar flag de interrupción del Capture1.0
	}
	else if(T1IR&(1<<5))
	{

		T1IR |= 1<<5;		// Activar flag de interrupción del Capture1.1
	}
	else if(T1IR&(1<<6))
	{

		T1IR |= 1<<6;		// Activar flag de interrupción del Capture1.2
	}
	else if(T1IR&(1<<7))
	{

		T1IR |= 1<<7;		// Activar flag de interrupción del Capture1.3
	}

	VICVectAddr = 0;	 
}

void UART0(void)__irq
{
	char uart0_dato;

	switch(U0IIR & 0x0E)
	{
		case 0x04:	// Recepción
			uart0_dato = U0RBR;

			if(uart0_dato == 'W')
				blinky_2 = '1';

			break;
		case 0x02:	// Transmisión
    		if(*uart0_ptr_tx == 0)
			{
				if(uart0_msg == '1') // Quedan mensajes por enviar
				{
					uart0_ptr_tx = uart0_buf_tx[uart0_rd]; // Se busca el nuevo mensaje para enviar
					uart0_rd++;
					uart0_rd &= N_MSG_UART0-1;
					if(uart0_rd == uart0_wr)
						uart0_msg = '0';	// Se va a enviar el último mensaje del buffer
				   	U0THR = *uart0_ptr_tx;
					uart0_ptr_tx++;
				}
				else
					uart0_repose = '1';
			}
			else	// Se continúa mandando el mensaje
				U0THR = *uart0_ptr_tx++;
	}	
    
	VICVectAddr = 0;	 
}

    
#endif
