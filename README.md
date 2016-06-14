# Projekt zabezpieczenia domu przed włamaniem

## Czujka wyczuwa ruch, nastepnie po 3-krotkny błędnym wpisaniu hasła lub po 20 s bezczynności wysyła e-mail do właściciela o włamaniu.Jest możliwość zmiany hasła.

### Projekt składa się z STM32 oraz aplikacji serwera.

### użyte materiały:
- płytka STM32 
- czujnik wykywający ruch
- wyświetlacz HD44780
- klawiatura 12-przyciskowa typu grid
- dodatkowy moduł wifi ESP8266
- serwer http (obsługuj±cy html , css, php i python oraz połaczenie z baz±) gdzie przechowywani bedą użytkownicy ze swoimi pinami
- weryfikacja hasła przez internet
- o potencjalnym włamaniu właściciel systemu będzie informowany drogą mailową


### Instrukcja:
- podłączyć wyświetlacz, klawiaturę, czujkę i moduł wifi do pinów opisanych w plikach odpowiednio "lcd.c", "keypad.c", "md.c" i "wifi.c"
- W pliku "lcd.c" należy w inicjalizacji w linijce AT+CWJAP_CUR=\"SSID\",\"PASSWORD\" zamienić SSID i PASSWORD na dane używanej sieci. W funcji wifi_connectToServer() nalezy zamienić adres ip na adres komputera, na którym jest uruchomiona aplikacja serwera.
- Jeżeli poprzednie kroki zostały wykonane prawidłowo, to po włączeniu urządzenia powinno połączyć się z siecią i po chwili wyświetlić logo.
