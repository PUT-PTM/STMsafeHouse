void wifi_init();
void USART_put(char *s);
void USART_put_char(char c);
void USART3_IRQHandler();
void wifi_sendData(char* data);
void wifi_connectToServer();
void sendmail();
int verifyPassword(char* pass);
