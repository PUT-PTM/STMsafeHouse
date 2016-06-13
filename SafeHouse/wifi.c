#include "wifi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_conf.h"
#include "misc.h"
#include "lcd.h"

//5c:cf:7f:87:7e:b1
volatile int wifi_i;
volatile char wifi_data[200];
volatile int wifi_state;

enum {
	wifi_disconnected=0, wifi_connected, wifi_got_ip, wifi_connected_to_serv, wifi_sent
};

void USART_put(char* s){
	int i=0;
	while(s[i]){
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
		USART_SendData(USART2, s[i++]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
	for(int i=0;i<10000000; i++);
}

void USART_put_char(char c){
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
	USART_SendData(USART2, c);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{

		char temp=USART_ReceiveData(USART2);

		if (temp == '\n') {
			wifi_data[wifi_i]='\0';
			wifi_readReply(wifi_data);
			wifi_i=0;
		}
		else if(temp != '\r'){
			wifi_data[wifi_i++]=temp;
			if(wifi_i>=200) wifi_i=0;
		}

	}
}


void wifi_readReply(char* string){
	if(strstr(string, "WIFI ")) {
		if (strstr(string, "DISCONNECTED")) wifi_state = wifi_disconnected;
		else if(strstr(string, "CONNECTED")) wifi_state = wifi_connected;
		else if(strstr(string, "GOT IP")) wifi_state = wifi_got_ip;
	}
	else if(strstr(string, "ALREADY CONNECTED") || strstr(wifi_data, "CONNECT")) wifi_state = wifi_connected_to_serv;
	else if(strstr(string, "CLOSED")) wifi_state = wifi_got_ip;
	else if(strstr(string, "SEND OK")) wifi_state = wifi_sent;
}

void wifi_init(){
	wifi_i=0;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	//konfiguracja linii Tx
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//konfiguracja linii Rx
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);

	USART_Cmd(USART2,ENABLE);

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(USART2_IRQn);

	lcd_changeScreen(lcd_scr_info_connecting);


	USART_put("ATE0\r\n");
	USART_put("AT+CWMODE_CUR=1\r\n");
	USART_put("AT+CIPMUX=0\r\n");

	USART_put("AT+CWJAP_CUR=\"SSID\",\"PASSWORD\"\r\n");

	while(wifi_state != wifi_got_ip);

	lcd_changeScreen(lcd_scr_logo);
}

void wifi_connectToServer() {
	while(wifi_state != wifi_connected_to_serv) {
			USART_put("AT+CIPSTART=\"TCP\",\"192.168.1.3\",8081\r\n");
			for(int i=0; i<100000000; i++);
	}
}

void sendmail() {
	wifi_connectToServer();

	while (wifi_state != wifi_sent) {
		USART_put("AT+CIPSENDEX=8\r\n");
		USART_put("sendmail");
		USART_SendData(USART2, 0);
		wifi_readReply(wifi_data);
	}

	wifi_state = wifi_connected_to_serv;
}

int verifyPassword(char* pass) {
	wifi_connectToServer();

	while (wifi_state != wifi_sent) {
		USART_put("AT+CIPSENDEX=11\r\n");

		USART_put("verify:");
		for(int i=0; i<4; i++)
			USART_put_char(pass[i]);

		USART_put_char('\0');
	}
	wifi_state = wifi_connected_to_serv;

	while(1) {
		if(!strncmp(wifi_data, "+IPD,19:verification:passed", 27)) return 1;
		else if(!strncmp(wifi_data, "+IPD,19:verification:failed", 27)) return 0;
		for(int i=0; i < 500000; i++);
	}
}
