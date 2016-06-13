#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "lcd.h"
#include "keypad.h"
#include "wifi.h"
#include "md.h"

int main(void)
{
	SystemInit();

	kp_init();

	lcd_init();
	lcd_changeScreen(lcd_scr_logo);

	wifi_init();

	md_init();


	for(;;){
	}//nie usuwaæ

}
