# STMsafeHouse

## Overdrive
### STMsafeHouse is a simply security system basted on motion detector. It's sending e-mail message, when someone unauthorized step into your house.

## Description
### Project uses STM32F4-Disovery board with three modules: Wi-Fi adapter, which is connected by UART connection, numeric keyboard and simply display. You've got 20 seconds to write your pin on clipboard after detecting your presence or e-mail will be send. Also when you write wrong pin three times e-mail will be send. Theres webside and server aplication to add users or change passwords.

## Tools
- CooCox CoIDE, Version 1.7.8
- Visual Studio 2015

## How to compile
- Just compile it, nothing more.

## How to run
- connect display, keyboard, motion sensor and Wi-Fi module to write pins described in "lcd.c", "keypad.c", "md.c" and "wifi.c"
files
- open Visual Studio?
- open CoIDE, in "wifi.c" file, in line AT+CWJAP_CUR=\"SSID\",\"PASSWORD\" change SSID and PASSWORD for your own. Also in function wifi_connectToServer() change ip adress to komputer's ip where serwer aplication is running. After that build project and flash it to STM32F4-Discavery board
- After a while you'll be connected

## How to use
- "*" activates system
- pin is a four-digit code
- "#" cancels last number from pin

## Future improvements
- Adding more motion sensors.

## Licence
- This project can be used under MIT license

## Credits
- Dominika Pawlaczyk
- Dominika Czarnecka
- Tomasz Fengier

#### The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
#### Supervisor: Tomasz Mańkowski
