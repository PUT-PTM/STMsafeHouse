#include "keypad.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "lcd.h"

//od lewego na klawiaturze
#define kp_pin_col_2 GPIO_Pin_14
#define kp_pin_row_1 GPIO_Pin_11
#define kp_pin_col_1 GPIO_Pin_12
#define kp_pin_row_4 GPIO_Pin_9
#define kp_pin_col_3 GPIO_Pin_10
#define kp_pin_row_3 GPIO_Pin_7
#define kp_pin_row_2 GPIO_Pin_8

enum kp_key {	key_none = 0,
	key_1=49, 	key_2, 		key_3,
	key_4, 		key_5, 		key_6,
	key_7, 		key_8, 		key_9,
	key_ast=42,	key_0=48, 	key_hash=35};

volatile uint8_t kp_rawInput;

void kp_init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = kp_pin_col_1|kp_pin_col_2|kp_pin_col_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line10|EXTI_Line12|EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	SYSCFG_EXTILineConfig(GPIOE,EXTI_PinSource10|EXTI_PinSource12|EXTI_PinSource14);

	kp_rawInput = 0;
	GPIO_SetBits(GPIOE, GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11);
	GPIO_ResetBits(GPIOE, GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14);
}

void EXTI15_10_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line10)||EXTI_GetITStatus(EXTI_Line12)||EXTI_GetITStatus(EXTI_Line14)) {

		GPIO_ResetBits(GPIOE, kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4);

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

		GPIO_ResetBits(GPIOE, kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4);

		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		lcd_cmd(kp_rawInput,1);
	}

	EXTI_ClearITPendingBit(EXTI_Line10);
	EXTI_ClearITPendingBit(EXTI_Line12);
	EXTI_ClearITPendingBit(EXTI_Line14);
}
