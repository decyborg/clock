/*
 * LED.c
 *
 * Created: 28/02/2015 11:45:57 a.m.
 *  Author: Manuel Alejandro
 */ 
#include "C:\Users\Manuel Alejandro\Documents\GitHub\Clock\Clock\Clock\Drivers\I2C\I2C.h"
#include "C:\Users\Manuel Alejandro\Documents\GitHub\Clock\Clock\Clock\Drivers\LED_controller\LED.h"
#include <util\delay.h>

static const uint16_t alphafonttable[] =  {

	0b0000000000000001,
	0b0000000000000010,
	0b0000000000000100,
	0b0000000000001000,
	0b0000000000010000,
	0b0000000000100000,
	0b0000000001000000,
	0b0000000010000000,
	0b0000000100000000,
	0b0000001000000000,
	0b0000010000000000,
	0b0000100000000000,
	0b0001000000000000,
	0b0010000000000000,
	0b0100000000000000,
	0b1000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0001001011001001,
	0b0001010111000000,
	0b0001001011111001,
	0b0000000011100011,
	0b0000010100110000,
	0b0001001011001000,
	0b0011101000000000,
	0b0001011100000000,
	0b0000000000000000, //
	0b0000000000000110, // !
	0b0000001000100000, // "
	0b0001001011001110, // #
	0b0001001011101101, // $
	0b0000110000100100, // %
	0b0010001101011101, // &
	0b0000010000000000, // '
	0b0010010000000000, // (
	0b0000100100000000, // )
	0b0011111111000000, // *
	0b0001001011000000, // +
	0b0000100000000000, // ,
	0b0000000011000000, // -
	0b0000000000000000, // .
	0b0000110000000000, // /
	0b0000110000111111, // 0
	0b0000000000000110, // 1
	0b0000000011011011, // 2
	0b0000000010001111, // 3
	0b0000000011100110, // 4
	0b0010000001101001, // 5
	0b0000000011111101, // 6
	0b0000000000000111, // 7
	0b0000000011111111, // 8
	0b0000000011101111, // 9
	0b0001001000000000, // :
	0b0000101000000000, // ;
	0b0010010000000000, // <
	0b0000000011001000, // =
	0b0000100100000000, // >
	0b0001000010000011, // ?
	0b0000001010111011, // @
	0b0000000011110111, // A
	0b0001001010001111, // B
	0b0000000000111001, // C
	0b0001001000001111, // D
	0b0000000011111001, // E
	0b0000000001110001, // F
	0b0000000010111101, // G
	0b0000000011110110, // H
	0b0001001000000000, // I
	0b0000000000011110, // J
	0b0010010001110000, // K
	0b0000000000111000, // L
	0b0000010100110110, // M
	0b0010000100110110, // N
	0b0000000000111111, // O
	0b0000000011110011, // P
	0b0010000000111111, // Q
	0b0010000011110011, // R
	0b0000000011101101, // S
	0b0001001000000001, // T
	0b0000000000111110, // U
	0b0000110000110000, // V
	0b0010100000110110, // W
	0b0010110100000000, // X
	0b0001010100000000, // Y
	0b0000110000001001, // Z
	0b0000000000111001, // [
	0b0010000100000000, //
	0b0000000000001111, // ]
	0b0000110000000011, // ^
	0b0000000000001000, // _
	0b0000000100000000, // `
	0b0001000001011000, // a
	0b0010000001111000, // b
	0b0000000011011000, // c
	0b0000100010001110, // d
	0b0000100001011000, // e
	0b0000000001110001, // f
	0b0000010010001110, // g
	0b0001000001110000, // h
	0b0001000000000000, // i
	0b0000000000001110, // j
	0b0011011000000000, // k
	0b0000000000110000, // l
	0b0001000011010100, // m
	0b0001000001010000, // n
	0b0000000011011100, // o
	0b0000000101110000, // p
	0b0000010010000110, // q
	0b0000000001010000, // r
	0b0010000010001000, // s
	0b0000000001111000, // t
	0b0000000000011100, // u
	0b0010000000000100, // v
	0b0010100000010100, // w
	0b0010100011000000, // x
	0b0010000000001100, // y
	0b0000100001001000, // z
	0b0000100101001001, // {
	0b0001001000000000, // |
	0b0010010010001001, // }
	0b0000010100100000, // ~
	0b0011111111111111,

};



/* LED_init()
*
*/
void LED_init(void)
{	
	// Turn on internal oscillator
	I2C_write_byte(HT16K33_SLA, HT16K33_OSCILLATOR_ON);
	// Turn on display
	I2C_write_byte(HT16K33_SLA, HT16K33_DISPLAY_SETUP | HT16K33_DISPLAY_ON);
	// Brightness maximum 
	I2C_write_byte(HT16K33_SLA, HT16K33_DIMMING_SET(15));
}

/*	write_display(string, dot)
*	Writes to the display the string that is passed (string must be 4 characters long), and sets the dot of the last segment if dot = 1
*/
void write_display(unsigned char* string, unsigned char dot)
{
	unsigned char displayBuffer[9];
	displayBuffer[0] = 0x00;
	for(int i = 1, j = 0; i < 9; i++)
	{	
		if(i%2)
			displayBuffer[i] = alphafonttable[string[j]] & 0xFF;
		else{
			displayBuffer[i] = alphafonttable[string[j]] >> 8;
			++j;
		}
	}
	if(dot)
		displayBuffer[8] |= 1 << 6;
	I2C_write(HT16K33_SLA, displayBuffer, 9);	
}

/* dim_display(dimming)
*  Takes a value (dimming) from 0 to 15 and sets the corresponding brightness
*  with 0 being the dimmest and 15 the brightest
*/
void dim_display(unsigned char dimming)
{
	if(dimming > 15)
		dimming = 15;
	I2C_write_byte(HT16K33_SLA, HT16K33_DIMMING_SET(dimming));
}

/* blink_display(blink)
* Sets the blinking frequency according to the parameter "blink"
*	0 -> No blinking
*	1 -> 2Hz blinking (2 blinks per second)
*	2 -> 1Hz blinking (1 blink per second)
*	3 -> .5Hz blinking (1 blink every two seconds)
*/
void blink_display(unsigned char blink)
{
	if(blink > 3)
		blink = 3;
	I2C_write_byte(HT16K33_SLA, HT16K33_DISPLAY_SETUP | HT16K33_DISPLAY_BLINK(blink) | HT16K33_DISPLAY_ON);
}

/*	on_off_display(on_off)
*	Turns on the display if on_off is true and turns it off if on_off = 0
*/
void on_off_display(unsigned char on_off)
{
	if(on_off)
		I2C_write_byte(HT16K33_SLA, HT16K33_DISPLAY_SETUP | HT16K33_DISPLAY_ON);
	else
		I2C_write_byte(HT16K33_SLA, HT16K33_DISPLAY_SETUP | HT16K33_DISPLAY_OFF);
}

/* slide_display
*	Writes a string to the display while sliding it useful for strings larger than the display 
*
*	string-> String to display
*	slide-> number of times to slide
*/
void slide_display(unsigned char* string, unsigned char slide)
{
	unsigned char len;
	int spaces;
	len = strlength(string);
	spaces = (len + 4);
	for(char j = 0; j < slide; ++j)
	{
		unsigned char display[4] = {' ', ' ', ' ', ' '};
		for(int i = 0;i < spaces ;++i)
		{
			display[0] = display[1];
			display[1] = display[2];
			display[2] = display[3];
			if(i < len)
				display[3] = string[i];
			else
				display[3] = ' ';
			write_display(display, 0);
			_delay_ms(500);
		}
	}
}

/* strlength
*	Returns the length of a string
*	string-> string to calculate the length from
*/
int strlength(unsigned char* string)
{
	int tmp;
	for(tmp = 0; string[tmp] != '\0'; ++tmp);
	return tmp;
}