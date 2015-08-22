/*
 * Clock.c
 *
 * Created: 16/02/2015 09:12:20 p.m.
 *  Author: Manuel Alejandro
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "C:\Users\Manuel Alejandro\Documents\GitHub\Clock\Clock\Clock\Drivers\LED_controller\LED.h"
#include "C:\Users\Manuel Alejandro\Documents\GitHub\Clock\Clock\Clock\Drivers\DS1307\RTC.h"
#include "C:\Users\Manuel Alejandro\Documents\GitHub\Clock\Clock\Clock\Drivers\USART\usart.h"
#include <avr\interrupt.h>

/* Global variables */
struct Dates init;
volatile char config_clock_alarm, alarm_active, enter, up_down;
static unsigned char config = 1;

/* Prototypes */
void configure(void);

/* Macro definitions */
#define interrupt_init	DDRD &= ~((1 << DDD2) | (1 << DDD3));		/* Set PD2 and PD3 as inputs */\
						PORTD |= (1 << PORTD2) | (1 << PORTD3);		/* Select Pull up enabled */\
						EICRA |= (1 << ISC00)  | (1 << ISC10);		/* Set INT0 and INT1 to trigger on falling edge */\
						EIMSK |= (1 << INT0)   | (1 << INT1);		/* Enable INT0 and INT1 */\
						sei();										/* Enable interrupts */

#define display_time_alarm	readRTC(&init);\
							time[3] = (init.seconds & 0xF) + '0';\
							time[2] = (init.seconds >> 4) + '0';\
							time[1] =  (init.minutes & 0xF) + '0';\
							time[0] = (init.minutes >> 4) + '0';\
							write_display(time, 1);
							
#define display_time		readRTC(&init);\
							time[3] = (init.seconds & 0xF) + '0';\
							time[2] = (init.seconds >> 4) + '0';\
							time[1] =  (init.minutes & 0xF) + '0';\
							time[0] = (init.minutes >> 4) + '0';\
							write_display(time, 0);

#define display_up_down		buffer[3] = (Dec2BCD(up_down) & 0xF) + '0';\
							buffer[2] = (Dec2BCD(up_down) >> 4) + '0';\
							buffer[1] = ' ';\
							buffer[0] = ' ';\
							write_display(buffer, 0);

#define MINUTES	 1
#define HOURS	 2
#define DAY		 3
#define DATE	 4
#define MONTH	 5
#define YEAR	 6
#define EXIT	 7



int main(void)
{	
	/* variable used to configure the clock */
	config_clock_alarm = 0;
	alarm_active = 0;
	enter = 0;
	up_down = 0;
	/* Initialize i2c interface */
	I2C_init();
	/* Avoid transmission for at least 1ms */
	_delay_ms(1000);
	/* Initialize LED controller */
	LED_init();
	/* Initialize I/Os */
	DDRB &=~((1 << DDB0) | (1 << DDB1) | (1 << DDB2));		/* Enable B0, B1 and B2 as inputs */
	PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);	/* Enable Pull up resistors */
	PCICR |= (1 << PCIE0);		/* Enable PMSK0 scan */
	PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2);	/* Set to trigger on state change */
	interrupt_init;

	/* Debugging purposes only */
	USART_ini(BAUD_REGISTER);
	dim_display(0);
	write_display("I<3U", 0);
	unsigned char time[4], buffer[4];
	_delay_ms(500);
	init.seconds = 20;
	init.minutes = 50;
	init.hours = 8;
	init.date = 3;
	init.day = 3;
	init.month = 5;
	init.year = 15;
	//write2RTC(init);
	while(1)
    {
		if(alarm_active)
		{
			display_time_alarm
			_delay_ms(500);
		} else {
			display_time
			_delay_ms(500);
		}
		
		/* Configure clock */
		if(config_clock_alarm == 1)
		{
			slide_display("Configure el reloj", 1);
			slide_display("Ingrese minutos", 1);
			config = 1;
			blink_display(2);
			while(config_clock_alarm == 1)
			{
				display_up_down;
				_delay_ms(500);
				if(enter)
				{
					configure();
					++config;
					enter = 0;
					up_down = 0;
				}
				if(config == EXIT)
					break;
			}
			blink_display(0);
			slide_display("Reloj configurado", 1);
			config_clock_alarm = 0;
		}
		/* Configure alarm */
		while(config_clock_alarm == 2)
		{
			slide_display("Configure la alarma", 1);
			_delay_ms(1000);
		}
    }
}

ISR(INT0_vect)
{
	char button = 0, i = 0;
	while(i < 30)
	{
		if (!(PIND & (1 << PD2)))
			++button;
		_delay_ms(10);
		++i;
	}
	if (button > 10)
	{
		config_clock_alarm++;
		config_clock_alarm %= 3;
	}
}

ISR(INT1_vect)
{
	char button = 0, i = 0;
	while(i < 30)
	{
		if (!(PIND & (1 << PD3)))
		++button;
		_delay_ms(10);
		++i;
	}
	if (button > 10)
		alarm_active = ~alarm_active;
}

ISR(PCINT0_vect)
{
	char button_up = 0, button_down = 0, button_enter = 0;
	for(char i = 0; i < 30; ++i)
	{
		switch(PINB & 0b111)
		{
			case 0b110:
				++button_down;
				break;
			case 0b101:
				++button_up;
				break;
			case 0b011:
				++button_enter;
				break;
		}
	}
	if (button_down > 10){
		--up_down;
		if(up_down < 0)
			up_down = 0;
	} else if (button_up > 10){
		++up_down;
		switch(config)
		{
			case MINUTES:
				up_down %= 60;
				break;
			case HOURS:
				up_down %= 24;
				break;
			case DAY:
				up_down %= 8;
				break;
			case DATE:
				up_down %= 32;
				break;
			case MONTH:
				up_down %= 13;
				break;
			case YEAR:
				up_down %= 100;
				break;
			default:
				break;
		}
	} else if (button_enter > 10) {
		enter = 1;
	}
}

void configure(void)
{
	blink_display(0);
	switch(config)
	{
		case MINUTES:
			init.minutes = up_down;
			slide_display("Ingrese horas", 1);
			break;
		case HOURS:
			init.hours = up_down;
			slide_display("Ingrese dia de la semana", 1);
			break;
		case DAY:
			init.day = up_down;
			slide_display("Ingrese dia del mes", 1);
			break;
		case DATE:
			init.date = up_down;
			slide_display("Ingrese mes", 1);
			break;
		case MONTH:
			init.month = up_down;
			slide_display("Ingrese año", 1);
			break;
		case YEAR:						
			init.year = up_down;
			init.seconds = 0;
			write2RTC(init);
			break;
	}
	blink_display(1);	
}