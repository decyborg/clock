/*
 * usart.h
 *
 * Created: 15/12/2014 09:52:38 p.m.
 *  Author: Manuel Alejandro
 */ 


#ifndef USART_H_
#define USART_H_
#include <avr/io.h>
/*
#define F_CPU 16000000UL 
#define BAUD 9600
#define BAUD_REGISTER ((F_CPU / (BAUD * 16UL)) - 1)*/

/* Prototypes */
void USART_ini(unsigned int);
void USART_Transmit(unsigned char);
unsigned char USART_Receive( void );
void USART_put(const char*);


#endif /* USART_H_ */