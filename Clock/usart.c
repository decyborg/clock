/*
 * CFile1.c
 *
 * Created: 15/12/2014 09:51:42 p.m.
 *  Author: Manuel Alejandro
 */ 
#include "usart.h"


void USART_ini(unsigned int baud_rate)
{
	/* Set Baud rate */
	UBRR0H = (unsigned char) (baud_rate >> 8);
	UBRR0L = (unsigned char) baud_rate;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_put(const char* str)
{
	for(int i = 0; str[i] != '\0'; i++)
		USART_Transmit(str[i]);
}