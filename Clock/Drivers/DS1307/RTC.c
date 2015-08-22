/*
 * RTC.c
 *
 * Created: 22/04/2015 07:54:50 p.m.
 *  Author: Manuel Alejandro
 */ 

#include "C:\Users\Manuel Alejandro\Documents\GitHub\Clock\Clock\Clock\Drivers\DS1307\RTC.h"

void write2RTC( struct Dates initialize)
{	
	unsigned char init_array[8];
	init_array[0] = 0x00; /* Start address */
	init_array[1] = Dec2BCD(initialize.seconds);
	init_array[2] = Dec2BCD(initialize.minutes);
	init_array[3] = Dec2BCD(initialize.hours);
	init_array[4] = initialize.day;
	init_array[5] = Dec2BCD(initialize.date);
	init_array[6] = Dec2BCD(initialize.month);
	init_array[7] = Dec2BCD(initialize.year);
	I2C_write(DS1307_SLA, init_array, 8);
}

void readRTC(struct Dates* current_date)
{
	char tmp[10];
	unsigned char read_array[7];
	I2C_write_byte(DS1307_SLA, 0x00);
	I2C_read(DS1307_SLA, &read_array, 7);
	current_date->seconds = read_array[0];
	current_date->minutes = read_array[1];
	current_date->hours = read_array[2];
	current_date->day = read_array[3];
	current_date->date = read_array[4];
	current_date->month = read_array[5];
	current_date->year = read_array[6];
}

char Dec2BCD(char dec)
{
	if(dec > 9)
	{
		char tmp = dec;
		while(tmp/10)
			tmp = dec/10;
		return ((tmp << 4) | dec % 10);
	}
	return dec;
}

