/*
 * LED.h
 *
 * Created: 28/02/2015 11:46:40 a.m.
 *  Author: Manuel Alejandro
 */ 


#ifndef LED_H_
#define LED_H_

#define HT16K33_SLA	0x70

#define HT16K33_OSCILLATOR_OFF 0x20
#define HT16K33_OSCILLATOR_ON 0x21
#define HT16K33_DISPLAY_SETUP 0x80
#define HT16K33_DISPLAY_ON 1
#define HT16K33_DISPLAY_OFF 0
#define HT16K33_DISPLAY_BLINK(x) (x << 1)
#define HT16K33_DIMMING_SET(x) (0xE0 | x)

/* Prototype */
void LED_init(void);
void write_display(unsigned char*, unsigned char);
void dim_display(unsigned char);
void blink_display(unsigned char);
void on_off_display(unsigned char);
void slide_display(unsigned char*, unsigned char);
int strlength(unsigned char*);

#endif /* LED_H_ */