#include "keypad.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

//od lewego na klawiaturze
#define kp_pin_col_2 GPIO_Pin_8
#define kp_pin_row_1 GPIO_Pin_7
#define kp_pin_col_1 GPIO_Pin_10
#define kp_pin_row_4 GPIO_Pin_9
#define kp_pin_col_3 GPIO_Pin_12
#define kp_pin_row_3 GPIO_Pin_11
#define kp_pin_row_2 GPIO_Pin_14

enum kp_key {key_none = 0,
	key_1, key_2, key_3,
	key_4, key_5, key_6,
	key_7, key_8, key_9,
	key_ast, key_0, key_hash};

volatile uint8_t kp_rawInput;
volatile uint8_t kp_prevState;

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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = 699;
	TIM_TimeBaseStructure.TIM_Prescaler = 599;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	kp_rawInput = 0;
	kp_prevState = 0;
}

void TIM3_IRQHandler(void) {
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET) {
		kp_prevState = kp_rawInput;

		GPIO_SetBits(GPIOE, kp_pin_row_1);
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_1)) {kp_rawInput=key_1; goto endPolling;}
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_2)) {kp_rawInput=key_2; goto endPolling;}
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_3)) {kp_rawInput=key_3; goto endPolling;}
		GPIO_ResetBits(GPIOE, kp_pin_row_1);
		GPIO_SetBits(GPIOE, kp_pin_row_2);
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_1)) {kp_rawInput=key_4; goto endPolling;}
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_2)) {kp_rawInput=key_5; goto endPolling;}
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_3)) {kp_rawInput=key_6; goto endPolling;}
		GPIO_ResetBits(GPIOE, kp_pin_row_2);
		GPIO_SetBits(GPIOE, kp_pin_row_3);
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_1)) {kp_rawInput=key_7; goto endPolling;}
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_2)) {kp_rawInput=key_8; goto endPolling;}
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_3)) {kp_rawInput=key_9; goto endPolling;}
		GPIO_ResetBits(GPIOE, kp_pin_row_3);
		GPIO_SetBits(GPIOE, kp_pin_row_4);
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_1)) {kp_rawInput=key_ast; goto endPolling;}
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_2)) {kp_rawInput=key_0; goto endPolling;}
		if (GPIO_ReadInputDataBit(GPIOE, kp_pin_col_3)) {kp_rawInput=key_hash; goto endPolling;}
		kp_rawInput = key_none;

		endPolling: GPIO_ResetBits(GPIOE, kp_pin_row_1|kp_pin_row_2|kp_pin_row_3|kp_pin_row_4);


		if (kp_rawInput && !kp_prevState) GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

		GPIO_WriteBit(GPIOD, GPIO_Pin_12, kp_rawInput);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}
