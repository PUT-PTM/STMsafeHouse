# Projekt zabezpieczenia domu przed w³amaniem

### u¿yte materia³y:
- p³ytka STM32 
- czujnik wykywaj¹cy ruch
- wyœwietlacz HD44780
- klawiatura 12-przyciskowa typu grid
- dodatkowy modu³ wifi ESP8266
- serwer http (obs³uguj±cy html , css, php i python oraz po³aczenie z baz±) gdzie przechowywani bed¹ u¿ytkownicy ze swoimi pinami
- weryfikacja has³a przez internet
- o potencjalnym w³amaniu w³aœciciel systemu bêdzie informowany drog¹ mailow¹


### Instrukcja:
- pod³¹czyæ wyœwietlacz, klawiaturê, czujkê i modu³ wifi do pinów opisanych w plikach odpowiednio "lcd.c", "keypad.c", "md.c" i "wifi.c"
- W pliku "lcd.c" nale¿y w inicjalizacji w linijce AT+CWJAP_CUR=\"SSID\",\"PASSWORD\" zamieniæ SSID i PASSWORD na dane u¿ywanej sieci. W funcji wifi_connectToServer() nalezy zamieniæ adres ip na adres komputera, na którym jest uruchomiona aplikacja serwera.
- Je¿eli poprzednie kroki zosta³y wykonane prawid³owo, to po w³¹czeniu urz¹dzenia powinno po³¹czyæ siê z sieci¹ i po chwili wyœwietliæ logo.