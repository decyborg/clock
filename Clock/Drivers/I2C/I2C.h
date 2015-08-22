/*
 * I2C.h
 *
 * Created: 16/02/2015 09:20:08 p.m.
 *  Author: Manuel Alejandro
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#define F_CPU 16000000UL

#define SEND_START TWCR = (1 << TWEN)|(1 << TWSTA)|(1 << TWINT)
#define TWINT_READY while(!(TWCR & (1 << TWINT)))
#define TRANSMIT TWCR = (1 << TWINT) | (1 << TWEN)
#define STOP TWCR = ((1<<TWINT)|(1<<TWEN)|(1<<TWSTO))
#define RETURN_ACK TWCR = ((1 << TWEA) | (1 << TWINT) | (1 << TWEN))
#define RETURN_NACK TWCR = (1 << TWINT | (1 << TWEN))
#define CLEAR_TWINT TWCR |= 1 << TWINT
#define START 0x08
#define RSTART 0x10
#define MT_SLAW_ACK 0x18
#define MT_SLAW_NACK 0x20
#define MT_DATAW_ACK 0x28
#define MT_DATAW_NACK 0x30
#define ARB_LOST 0x38
#define MT_SLAR_ACK 0x40
#define MT_SLAR_NACK 0x48
#define MT_DATAR_ACK 0x50
#define MT_DATAR_NACK 0x58

/* Prototypes */
unsigned char I2C_write(unsigned char Slave_addres, unsigned char* data, int data_length);
unsigned char I2C_read(unsigned char Slave_addres, unsigned char* data, int data_length);
unsigned char I2C_write_byte(unsigned char Slave_addres, unsigned char data);
void I2C_init(void);


#endif /* I2C_H_ */