/*
 * RTC.h
 *
 * Created: 22/04/2015 07:54:37 p.m.
 *  Author: Manuel Alejandro
 */ 


#ifndef RTC_H_
#define RTC_H_

#include "C:\Users\Manuel Alejandro\Documents\GitHub\Clock\Clock\Clock\Drivers\I2C\I2C.h"

#define DS1307_SLA			0x68
#define DS1307_ch_disable	(1 << 7)
#define DS1307_AMPM			(1 << 6)

struct Dates
{
	char seconds;
	char minutes;
	char hours;
	char day;
	char date;
	char month;
	char year;		
};

/* Prototypes */
char Dec2BCD(char);
void write2RTC( struct Dates);
void readRTC(struct Dates* current_date);

#endif /* RTC_H_ */