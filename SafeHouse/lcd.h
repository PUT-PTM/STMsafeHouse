#include "stdint.h"

void lcd_cmd(uint8_t cmd, uint8_t RS);
void lcd_init();

#define lcd_shiftWindow 0x01
#define lcd_shiftCursor 0x00
#define lcd_dirRight 0x02
#define lcd_dirLeft 0x00

#define lcd_on 0x04
#define lcd_off 0x00
#define lcd_cursorOn 0x02
#define lcd_cursorOff 0x00
#define lcd_blinkingOn 0x01
#define lcd_blinkingOff 0x00

#define lcd_interface8bit 0x10
#define lcd_interface4bit 0x00
#define lcd_dspOneRow 0x00
#define lcd_dspTwoRows 0x08
#define lcd_matrix5x10 0x04
#define lcd_matrix5x7 0x00

void lcd_clear();
void lcd_cursorHome();
void lcd_entryModeSet(uint8_t mode);
void lcd_onOff(uint8_t params);
void lcd_cursorShift(uint8_t mode);
void lcd_functionSet(uint8_t mode);
void lcd_cgramSet(uint8_t charAddr, uint8_t lineAddr);
void lcd_ddramSet(uint8_t addr);
uint8_t lcd_busyFlagRead();
void lcd_write(char* c);
void lcd_write_n(uint8_t* c, int n);
