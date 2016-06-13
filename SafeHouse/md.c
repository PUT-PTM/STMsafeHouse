#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "lcd.h"
#include "wifi.h"

//Wyjscie NC z czujki -> pin PA0

//zmienna przyjmuje wartosc 1, jezeli alarm jest uzbrojony
volatile int md_armed = 0;

void md_init(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // pin od czujki wlacza timer alarmu
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);


	// timer dopuszcza pewien czas na wpisanie hasla przed wyslaniem ostrzezenia
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 41999;
	TIM_TimeBaseStructure.TIM_Prescaler = 39999;//20s
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}

// uzbrojenie alarmu
void md_arm() {
	md_armed = 1;
}

// rozbrojenie alarmu
void md_disarm() {
	md_armed = 0;
	TIM_Cmd(TIM2, DISABLE);
	TIM2->CNT=0;
}

int md_isArmed() {
	return md_armed;
}

// przerwanie od czujki. uruchamia timer, jezeli alarm jset uzbrojony
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		if(md_armed) {
			TIM_Cmd(TIM2, ENABLE);
			lcd_changeScreen(lcd_scr_psw_entry);
		}

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

// po okreslonym czasie timer uruchamia wysylanie e-maila do uzytkownika, o ile nie zostanie wylaczony
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		TIM_Cmd(TIM2, DISABLE);
		TIM2->CNT=0;
		sendmail();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}


