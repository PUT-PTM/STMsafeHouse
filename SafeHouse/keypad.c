#include "keypad.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"

//od lewego na klawiaturze
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

void kp_init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = kp_pin_col_1|kp_pin_col_2|kp_pin_col_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
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

void kp_event(uint8_t key) {
	lcd_cmd(key, 1);
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET||
	   EXTI_GetITStatus(EXTI_Line13)!=RESET||
	   EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		for(int i=0; i<100; i++);
		GPIO_ResetBits(GPIOE, kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4);

		uint8_t kp_rawInput;

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

		if(kp_rawInput) kp_event(kp_rawInput);

		GPIO_SetBits(GPIOE, kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4);

		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		EXTI_ClearITPendingBit(EXTI_Line11);
		EXTI_ClearITPendingBit(EXTI_Line13);
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

