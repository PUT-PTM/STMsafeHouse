#include "wifi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_conf.h"
#include "misc.h"
#include <stdio.h>

char *data;
int i=0;

void USART_put(char* s){

    while(*s){
        // wait until data register is empty
       // while( !(USARTx->SR & 0x00000040) );
    	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
    	//printf("%c", *s);
    	USART_SendData(USART2, *s++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
      //  *(s++);
    }
}

void USART3_IRQHandler(void){
		 i=0;
		//sprawdzenieflagizwiazanejzodebraniemdanychprzezUSART
		if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
		{
			char temp=USART_ReceiveData(USART2);
			if(temp != "\r" || temp != "\n"){
				data[i]=temp;
				i++;
			}else i=0;
			i++;
	}
		USART_put(*data);
}

void wifi_init(){

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//wlaczenie taktowania wybranego portu
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	//wlaczenie taktowani awybranego ukadu USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	//konfiguracja linii Tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	//GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//konfiguracja linii Rx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,GPIO_AF_USART2);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;

	// predkosc transmisji (mozliwe standardowe opcje: 9600, 19200, 38400,
	// 57600,115200,...)
	USART_InitStructure.USART_BaudRate=115200;
	//dugo sowa(USART_WordLength_8b lub USART_WordLength_9b)
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	// liczba bitów stopu (USART_StopBits_1, USART_StopBits_0_5,
	//USART_StopBits_2,USART_StopBits_1_5)
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	// sprawdzanie parzystoci (USART_Parity_No, USART_Parity_Even,
	// USART_Parity_Odd)
	USART_InitStructure.USART_Parity=USART_Parity_No;
	// sprztowa kontrola przepywu (USART_HardwareFlowControl_None,
	// USART_HardwareFlowControl_RTS, USART_HardwareFlowControl_CTS,
	// USART_HardwareFlowControl_RTS_CTS)
	USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	//trybnadawania/odbierania(USART_Mode_Rx,USART_Mode_Rx)
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	//konfiguracja
	USART_Init(USART2,&USART_InitStructure);

	USART_Cmd(USART2,ENABLE);

	// wlaczenie przerwania zwizanego z odebraniem danych (pozostale zrodla
	//przerwanzdefiniowanesawplikustm32f4xx_usart.h)
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//konfiguracjakontroleraprzerwan
	NVIC_Init(&NVIC_InitStructure);
	//wlaczenieprzerwanodukladuUSART
	NVIC_EnableIRQ(USART2_IRQn);

	USART_put("AT\r\n");
		for(int i=0;i<2000; i++);
	USART_put("AT+RST\r\n");
		for(int i=0;i<2000; i++);
	USART_put("AT+CWMODE=1\r\n");
		for(int i=0;i<2000; i++);
	USART_put("AT+CIPSTAMAC?\r\n");
		for(int i=0;i<2000; i++);
	USART_put("AT+CWQAP\r\n");
		for(int i=0;i<2000; i++);
	USART_put("AT+CWLAP\r\n");
		for(int i=0;i<2000; i++);
	USART_put("AT+CWJAP=\"Domcia-iPhone\",\"brzuszek\"\r\n");
		for(int i=0;i<2000; i++);
	USART_put("AT+CWJAP\r\n");
		for(int i=0;i<2000; i++);
	USART_put("AT+CIFSR\r\n");
		for(int i=0;i<2000; i++);

////	while(1){
//	//czekajnaoprónieniebuforawyjciowego
//	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
////	wyslaniedanych
//			USART_put(USART3,"AT\r\n");
////			czekajazdanezostanawyslane
//			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
////	}
}
