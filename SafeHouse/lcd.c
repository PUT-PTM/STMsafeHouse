#include "lcd.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stdint.h"

#define _RS GPIOC, GPIO_Pin_0
#define _RW GPIOC, GPIO_Pin_1
#define _E GPIOC, GPIO_Pin_2
#define _DB0 GPIOC, GPIO_Pin_3
#define _DB1 GPIOA, GPIO_Pin_0
#define _DB2 GPIOA, GPIO_Pin_1
#define _DB3 GPIOA, GPIO_Pin_2
#define _DB4 GPIOA, GPIO_Pin_3
#define _DB5 GPIOA, GPIO_Pin_4
#define _DB6 GPIOA, GPIO_Pin_5
#define _DB7 GPIOA, GPIO_Pin_6

//for(long i=0;i<100000000;i++); //7.15s

void lcd_cmd(uint8_t cmd, uint8_t RS) {
	GPIO_WriteBit(_RS, RS);
	GPIO_WriteBit(_DB7, cmd&0b10000000);
	GPIO_WriteBit(_DB6, cmd&0b01000000);
	GPIO_WriteBit(_DB5, cmd&0b00100000);
	GPIO_WriteBit(_DB4, cmd&0b00010000);
	GPIO_WriteBit(_DB3, cmd&0b00001000);
	GPIO_WriteBit(_DB2, cmd&0b00000100);
	GPIO_WriteBit(_DB1, cmd&0b00000010);
	GPIO_WriteBit(_DB0, cmd&0b00000001);
	GPIO_ResetBits(_E);
	for(int i=0;i<50000;i++);
	GPIO_SetBits(_E);
}

void lcd_init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(_E);

	for(int i=0; i<5000000;i++);
	lcd_cmd(0b00110000,0);
	for(int i=0; i<100000;i++);
	lcd_cmd(0b00110000,0);
	for(int i=0; i<10000;i++);
	lcd_cmd(0b00110000,0);//3x Fset
	for(int i=0; i<10000;i++);

	lcd_functionSet(lcd_interface8bit|lcd_dspTwoRows|lcd_matrix5x7);
	lcd_onOff(lcd_off|lcd_cursorOff|lcd_blinkingOff);
	lcd_clear();
	lcd_entryModeSet(lcd_dirRight|lcd_shiftCursor);

	lcd_cgramSet(0,0);
	uint8_t znak[8*8] = {
		0,0,14,1,15,17,15,2,//¹
		2,4,14,16,16,17,14,0, //æ
		0,0,14,17,31,16,14,2, //ê
		12,4,6,12,4,4,14,0, //³
		2,4,22,25,17,17,17,0, //ñ
		2,4,14,17,17,17,14,0, //ó
		2,4,14,16,14,1,30,0, //œ
		2,4,31,2,4,8,31,0, //Ÿ
		4,0,31,2,4,8,31,0}; //¿
	lcd_write_n(znak,8*8);
	lcd_ddramSet(0);

	lcd_onOff(lcd_on|lcd_cursorOff|lcd_blinkingOn);
}

void lcd_clear() {
	lcd_cmd(0x01, 0);
}

void lcd_cursorHome() {
	lcd_cmd(0x02, 0);
}

void lcd_entryModeSet(uint8_t mode) {
	lcd_cmd(0x04|mode, 0);
}

void lcd_onOff(uint8_t params) {
	lcd_cmd(0x08|params, 0);
}

void lcd_cursorShift(uint8_t mode) {
	lcd_cmd(0x10|(mode<<2), 0);
}

void lcd_functionSet(uint8_t mode) {
	lcd_cmd(0x20|mode, 0);
}

void lcd_cgramSet(uint8_t charAddr, uint8_t lineAddr) {
	lcd_cmd(0x40|((0x07&charAddr)<<3)|(0x07&lineAddr), 0);
}

void lcd_ddramSet(uint8_t addr) {
	lcd_cmd(0x80|addr, 0);
}

uint8_t lcd_busyFlagRead() {
	GPIOA->MODER &= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER4_0
			| GPIO_MODER_MODER3_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER0_0;
	GPIOC->MODER &= GPIO_MODER_MODER3_0;

	GPIO_SetBits(_RW);
	GPIO_ResetBits(_E);

	for(int i=0;i<50000;i++);
	uint8_t retval = 0x00;
	if (GPIO_ReadInputDataBit(_DB7)) retval |= 0b10000000;
	if (GPIO_ReadInputDataBit(_DB6)) retval |= 0b01000000;
	if (GPIO_ReadInputDataBit(_DB5)) retval |= 0b00100000;
	if (GPIO_ReadInputDataBit(_DB4)) retval |= 0b00010000;
	if (GPIO_ReadInputDataBit(_DB3)) retval |= 0b00001000;
	if (GPIO_ReadInputDataBit(_DB2)) retval |= 0b00000100;
	if (GPIO_ReadInputDataBit(_DB1)) retval |= 0b00000010;
	if (GPIO_ReadInputDataBit(_DB0)) retval |= 0b00000001;

	GPIOA->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER4_0
				| GPIO_MODER_MODER3_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER0_0;
	GPIOC->MODER |= GPIO_MODER_MODER3_0;

	for(int i=0;i<10000;i++);
	GPIO_ResetBits(_RW);
	GPIO_SetBits(_E);
	return retval;
}

void lcd_write(char* c) {
	for(int i=0; c[i]; i++) {
		if(c[i]=='\n') lcd_ddramSet(0x40);
		else lcd_cmd(c[i],1);
	}
}

void lcd_write_n(uint8_t* c, int n) {
	for(int i=0;i<n;i++) lcd_cmd(c[i],1);
}
