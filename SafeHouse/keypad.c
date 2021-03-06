#include "keypad.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "lcd.h"
#include "md.h"

//od lewego na klawiaturze
//wszystkie do linii GPIOE
#define kp_pin_col_2 GPIO_Pin_15
#define kp_pin_row_1 GPIO_Pin_14
#define kp_pin_col_1 GPIO_Pin_13
#define kp_pin_row_4 GPIO_Pin_12
#define kp_pin_col_3 GPIO_Pin_11
#define kp_pin_row_3 GPIO_Pin_10
#define kp_pin_row_2 GPIO_Pin_9

enum kp_key {	key_none = 0,
	key_1=49, 	key_2, 		key_3,
	key_4, 		key_5, 		key_6,
	key_7, 		key_8, 		key_9,
	key_ast=42,	key_0=48, 	key_hash=35};

char kp_inputBuffer[4];
int kp_inputBufferIt = 0;

void kp_init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//Piny kolumn sa skonfigurowane jako wejscia w trybie przerwan
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = kp_pin_col_1|kp_pin_col_2|kp_pin_col_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//Piny wierszy sa skonfigurowane jako wyjscia o domyslnym stanie wysokim
	GPIO_InitStructure.GPIO_Pin = kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line11|EXTI_Line13|EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_SetBits(GPIOE, kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource11);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource13);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource15);
}

//metoda wywolywana po nacisnieciu przycisku. Argumentem jest kod przycisku
void kp_event(uint8_t key) {
	//# sluzy do wlaczenia alarmu
	if(key==key_hash) {
		lcd_changeScreen(lcd_scr_info_armed);
		for(int i=0;i<50000000;i++);
		lcd_changeScreen(lcd_scr_logo);
		md_arm();
	}

	else {
		if(md_isArmed()) {
			if(kp_inputBufferIt==0) {
				lcd_changeScreen(lcd_scr_psw_entry);
			}

			if(key!=key_ast) {
				//ciag wciscnietych klawiszy jest wczytywany do bufora
				kp_inputBuffer[kp_inputBufferIt++]=key;

				//po wpisaniu 4 znakow nastepuje weryfikacja hasla
				if(kp_inputBufferIt>=4) {
					kp_inputBufferIt=0;

					lcd_changeScreen(lcd_scr_psw_verifying);

					if (verifyPassword(kp_inputBuffer)) {

						lcd_changeScreen(lcd_scr_psw_ok);
						md_disarm();
						for(int i=0; i<50000000; i++);
						lcd_changeScreen(lcd_scr_logo);

					} else {

						lcd_changeScreen(lcd_scr_psw_wrong);
						for(int i=0; i<50000000; i++);
						lcd_changeScreen(lcd_scr_psw_entry);
					}

				} else {
					lcd_cmd(key, 1);
				}
			}
			else {
				//gwiazdka cofa kursor o jedno pole
				if (kp_inputBufferIt) kp_inputBufferIt--;
				lcd_ddramSet(0x46+kp_inputBufferIt);
			}
		}
		else {
			//jezeli alarm jest wylaczony, to nie mozna wpisac hasla
			lcd_changeScreen(lcd_scr_info_already_disarmed);
			for(int i=0; i<50000000; i++);
			lcd_changeScreen(lcd_scr_logo);
		}
	}
}

//obsluga przerwan od pinow kolumn. Ma na celu zidentyfikowanie, ktory przycisk zostal nacisniety.
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET||
			EXTI_GetITStatus(EXTI_Line13)!=RESET||
			EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		for(int i=0; i<100; i++);
		GPIO_ResetBits(GPIOE, kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4);

		uint8_t kp_rawInput;

		//Sprawdzanie po kolei, czy przycisk jest nacisniety.
		GPIO_SetBits(GPIOE, kp_pin_row_1);
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_1)) kp_rawInput=key_1;
		else if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_2)) kp_rawInput=key_2;
		else if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_3)) kp_rawInput=key_3;
		else {
			GPIO_ResetBits(GPIOE, kp_pin_row_1);
			GPIO_SetBits(GPIOE, kp_pin_row_2);
			if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_1)) kp_rawInput=key_4;
			else if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_2)) kp_rawInput=key_5;
			else if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_3)) kp_rawInput=key_6;
			else {
				GPIO_ResetBits(GPIOE, kp_pin_row_2);
				GPIO_SetBits(GPIOE, kp_pin_row_3);
				if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_1)) kp_rawInput=key_7;
				else if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_2)) kp_rawInput=key_8;
				else if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_3)) kp_rawInput=key_9;
				else {
					GPIO_ResetBits(GPIOE, kp_pin_row_3);
					GPIO_SetBits(GPIOE, kp_pin_row_4);
					if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_1)) kp_rawInput=key_ast;
					else if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_2)) kp_rawInput=key_0;
					else if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_3)) kp_rawInput=key_hash;
					else kp_rawInput = key_none;
				}
			}
		}

		//przeslanie kodu klawisza do obslugi
		if(kp_rawInput) kp_event(kp_rawInput);

		GPIO_SetBits(GPIOE, kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4);

		EXTI_ClearITPendingBit(EXTI_Line11);
		EXTI_ClearITPendingBit(EXTI_Line13);
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

