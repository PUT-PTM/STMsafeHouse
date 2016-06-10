#include "lcd.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stdint.h"

#define lcd_4bit_mode

//od lewego na wyswietlaczu
#define _RS  GPIOE, GPIO_Pin_5
#define _RW
#define _E   GPIOE, GPIO_Pin_4
#define _DB0
#define _DB1
#define _DB2
#define _DB3
#define _DB4 GPIOE, GPIO_Pin_3
#define _DB5 GPIOE, GPIO_Pin_2
#define _DB6 GPIOE, GPIO_Pin_1
#define _DB7 GPIOE, GPIO_Pin_0

int lcd_currentScreen;

void lcd_cmd(uint8_t cmd, uint8_t RS) {
#ifdef lcd_4bit_mode
	lcd_cmd4(cmd,RS);
	lcd_cmd4(cmd<<4,RS);
#else
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
	for(int i=0;i<30000;i++);
	GPIO_SetBits(_E);
#endif
}

void lcd_cmd4(uint8_t cmd4, uint8_t RS) {
	GPIO_WriteBit(_RS, RS);
	GPIO_WriteBit(_DB7, cmd4&0b10000000);
	GPIO_WriteBit(_DB6, cmd4&0b01000000);
	GPIO_WriteBit(_DB5, cmd4&0b00100000);
	GPIO_WriteBit(_DB4, cmd4&0b00010000);
	GPIO_ResetBits(_E);
	for(int i=0;i<30000;i++);
	GPIO_SetBits(_E);
}

void lcd_init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(_E);

	for(int i=0; i<5000000;i++);//czekaj na ustabilizowanie napiêcia

#ifdef lcd_4bit_mode
	lcd_cmd4(0b00110000,0);
	for(int i=0; i<100000;i++);
	lcd_cmd4(0b00110000,0);
	for(int i=0; i<10000;i++);
	lcd_cmd4(0b00110000,0);//3x Fset
	for(int i=0; i<10000;i++);
	lcd_cmd4(0b00100000,0);
#else
	lcd_cmd(0b00110000,0);
	for(int i=0; i<100000;i++);
	lcd_cmd(0b00110000,0);
	for(int i=0; i<10000;i++);
	lcd_cmd(0b00110000,0);//3x Fset
	for(int i=0; i<10000;i++);
#endif

	lcd_functionSet(lcd_interface4bit|lcd_dspTwoRows|lcd_matrix5x8);
	lcd_onOff(lcd_off);
	lcd_entryModeSet(lcd_dirRight|lcd_shiftCursor);
	lcd_loadCustomChars();

	lcd_clear();
}

void lcd_loadCustomChars() {
	lcd_cgramSet(0,0);
	uint8_t znak[8*8] = {
			0,31,31,31,31,31,0,0,//
			0,0,14,1,15,17,15,2,//¹
			2,4,14,16,16,17,14,0, //æ
			0,0,14,17,31,16,14,2, //ê
			12,4,6,12,4,4,14,0, //³
			2,4,22,25,17,17,17,0, //ñ
			2,4,14,17,17,17,14,0, //ó
			2,4,14,16,14,1,30,0, //œ
			//2,4,31,2,4,8,31,0, //Ÿ
			//4,0,31,2,4,8,31,0 //¿
	};
	lcd_write_n(znak,8*8);
	lcd_ddramSet(0);
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

void lcd_write(char* c) {
	for(int i=0; c[i]; i++) {
		if(c[i]=='\8') lcd_cmd(0,1);
		else if(c[i]=='\n') lcd_ddramSet(0x40);
		else lcd_cmd(c[i],1);
	}
}

void lcd_write_n(uint8_t* c, int n) {
	for(int i=0;i<n;i++) {
		lcd_cmd(c[i],1);
	}
}

int lcd_changeScreen(int newscr) {
	if (lcd_currentScreen!=newscr) {

		switch(newscr) {
		case lcd_scr_logo:
			lcd_clear();
			lcd_onOff(lcd_on|lcd_cursorOff|lcd_blinkingOff);
			lcd_write("   SafeHouse");
			lcd_ddramSet(0x03);
			lcd_write("SafeHouse");
			break;

		case lcd_scr_psw_entry:
			lcd_clear();
			lcd_onOff(lcd_on|lcd_cursorOn);
			lcd_write("  Podaj has\3o:\n      ");
			lcd_ddramSet(0x02);
			lcd_write("Podaj has\4o:");
			lcd_ddramSet(0x46);
			break;

		case lcd_scr_psw_ok:
			lcd_clear();
			lcd_onOff(lcd_on|lcd_blinkingOff);
			lcd_ddramSet(0x07);
			lcd_write("OK");
			lcd_ddramSet(0x40);
			lcd_write("alarm wy\4\1czony");
			break;

		case lcd_scr_psw_wrong:
			lcd_clear();
			lcd_onOff(lcd_on|lcd_blinkingOff);
			lcd_ddramSet(0x03);
			lcd_write("Z\4e has\4o!");
			break;

		case lcd_scr_info_armed:
			lcd_clear();
			lcd_onOff(lcd_on|lcd_blinkingOff);
			lcd_ddramSet(0x02);
			lcd_write("Alarm zosta\4");
			lcd_ddramSet(0x44);
			lcd_write("wl\1czony");
			break;

		case lcd_scr_info_already_disarmed:
			lcd_clear();
			lcd_onOff(lcd_on|lcd_blinkingOff);
			lcd_write("Alarm wy\4\1czony!");
			lcd_ddramSet(0x43);
			lcd_write("wci\7nij #");
			break;

		case lcd_scr_info_connecting:
			lcd_clear();
			lcd_onOff(lcd_on|lcd_blinkingOff);
			lcd_write("\4\1czenie z wifi");
			break;

		}

		lcd_currentScreen = newscr;
		return 1;
	} else return 0;
}
