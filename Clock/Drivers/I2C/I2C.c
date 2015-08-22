/*
 * I2C.c
 *
 * Created: 16/02/2015 09:19:25 p.m.
 *  Author: Manuel Alejandro
 */ 


#include "C:\Users\Manuel Alejandro\Documents\GitHub\Clock\Clock\Clock\Drivers\I2C\I2C.h"
#include <util/delay.h>

/* I2C_init
*	Set I2C clock frequency
*/
void I2C_init(void)
{	
	/* Set frequency to 100kHz based on a 16MHz crystal */
	TWSR = 0;
	TWBR = 82;
	
	/* Enable I2C*/
	TWCR = ( 1 << TWEN); 	
}

/* I2C Write 
* Returns a value corresponding to the state of the write
*
* int Slave_addres - the address of the slave to write
* int* data - A buffer of data to be written
* int data_lenght - the number of bytes to be written
*
*/
unsigned char I2C_write(unsigned char Slave_addres, unsigned char* data, int data_length)
{
	
	unsigned char slave_addres_write = (Slave_addres << 1);
	/* Send START */
	SEND_START;
	TWINT_READY;
	if ((TWSR & 0xF8) != START)
	{
		TWCR = (1 << TWINT)|(1 << TWSTO);
		return (TWSR & 0xF8);
	}
	/* Send Slave address + W */
	TWDR = slave_addres_write;
	TRANSMIT;
	TWINT_READY;
	if ((TWSR & 0xF8) !=MT_SLAW_ACK)
	{
		TWCR = (1 << TWINT)|(1 << TWSTO);
		return (TWSR & 0xF8);
	}
	/* Write bytes */
	for(int i = 0; i < data_length; i++)
	{
		TWDR = *(data + i);
		TRANSMIT;
		TWINT_READY;
		if ((TWSR & 0xF8) !=MT_DATAW_ACK)
		{
			TWCR = (1 << TWINT)|(1 << TWSTO);
			return (TWSR & 0xF8);
		}
	}
	/* Send STOP */
	STOP;
	_delay_ms(1);
	//while(TWCR & (1 << TWSTO));
	return 0;
			
}

/* I2C Write byte
* Returns a value corresponding to the state of the write
*
* int Slave_addres - the address of the slave to write
* int data - data to be written
*
*/
unsigned char I2C_write_byte(unsigned char Slave_addres, unsigned char data)
{
	
	unsigned char slave_addres_write = (Slave_addres << 1);
	/* Send START */
	SEND_START;
	TWINT_READY;
	if ((TWSR & 0xF8) != START)
	{
		TWCR = (1 << TWINT)|(1 << TWSTO);
		return (TWSR & 0xF8);
	}
	/* Send Slave address + W */
	TWDR = slave_addres_write;
	TRANSMIT;
	TWINT_READY;
	if ((TWSR & 0xF8) !=MT_SLAW_ACK)
	{
		TWCR = (1 << TWINT)|(1 << TWSTO);
		return (TWSR & 0xF8);
	}
	/* Write byte */
	TWDR = data;
	TRANSMIT;
	TWINT_READY;
	if ((TWSR & 0xF8) != MT_DATAW_ACK)
	{
		TWCR = (1 << TWINT)|(1 << TWSTO);
		return (TWSR & 0xF8);
	}
	/* Send STOP */
	STOP;
	_delay_ms(1);
	//while(TWCR & (1 << TWSTO));
	return 0;	
}

/* I2C Read
*	Returns 0 if Read is successful returns the state of the read otherwise
*
*	Slave_address -> Slave to read from
*	data->	Array to store read data
*	data_length-> Number of bytes to be read (if just one byte is desired this can be done by just setting data_length to one)
*/
unsigned char I2C_read(unsigned char Slave_addres, unsigned char* data, int data_length)
{
	
	unsigned char slave_addres_read = (Slave_addres << 1) | 0b1;
	/* Send START */
	SEND_START;
	TWINT_READY;
	if ((TWSR & 0xF8) != START)
	{
		TWCR = (1 << TWINT)|(1 << TWSTO);
		return (TWSR & 0xF8);
	}
	/* Send Slave address + R */
	TWDR = slave_addres_read;
	TRANSMIT;
	TWINT_READY;
	if ((TWSR & 0xF8) !=MT_SLAR_ACK)
	{
		TWCR = (1 << TWINT)|(1 << TWSTO);
		return (TWSR & 0xF8);
	}
	/* Read bytes */
	int i = 0;
	for(; i < data_length - 1; ++i)
	{
		RETURN_ACK;
		CLEAR_TWINT;
		TWINT_READY;
		data[i] = TWDR;
		if ((TWSR & 0xF8) != MT_DATAR_ACK)
		{
			TWCR = (1 << TWINT)|(1 << TWSTO);
			return (TWSR & 0xF8);
		}
	}
	/* Read Last byte */
	RETURN_NACK;
	CLEAR_TWINT;
	TWINT_READY;
	if(data_length > 1)
		data[i + 1] = TWDR;
	else
		*data = TWDR;
	/* Send STOP */
	if((TWSR & 0xF8) == MT_DATAR_NACK){
		STOP;
		_delay_ms(1);
		//while(TWCR & (1 << TWSTO));
		return 0;
	}
	else
	{
		TWCR = (1 << TWINT)|(1 << TWSTO);
		return (TWSR & 0xF8);
	}
}