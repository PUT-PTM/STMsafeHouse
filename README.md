# Projekt zabezpieczenia domu przed w�amaniem

### u�yte materia�y:
- p�ytka STM32 
- czujnik wykywaj�cy ruch
- wy�wietlacz HD44780
- klawiatura 12-przyciskowa typu grid
- dodatkowy modu� wifi ESP8266
- serwer http (obs�uguj�cy html , css, php i python oraz po�aczenie z baz�) gdzie przechowywani bed� u�ytkownicy ze swoimi pinami
- weryfikacja has�a przez internet
- o potencjalnym w�amaniu w�a�ciciel systemu b�dzie informowany drog� mailow�


### Instrukcja:
- pod��czy� wy�wietlacz, klawiatur�, czujk� i modu� wifi do pin�w opisanych w plikach odpowiednio "lcd.c", "keypad.c", "md.c" i "wifi.c"
- W pliku "lcd.c" nale�y w inicjalizacji w linijce AT+CWJAP_CUR=\"SSID\",\"PASSWORD\" zamieni� SSID i PASSWORD na dane u�ywanej sieci. W funcji wifi_connectToServer() nalezy zamieni� adres ip na adres komputera, na kt�rym jest uruchomiona aplikacja serwera.
- Je�eli poprzednie kroki zosta�y wykonane prawid�owo, to po w��czeniu urz�dzenia powinno po��czy� si� z sieci� i po chwili wy�wietli� logo.