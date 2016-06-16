# STMsafeHouse

## Overview
 STMsafeHouse is a simple security system based on STM32F4 microcontroller. It's main function is to notify the user about a break-in via email.

## Description
 Project consists of 3 modules: a password authentification server, web-based http user service, and an STM32F4-Discovery board with a Wi-Fi adapter, connected by UART, numeric keyboard, an HD44780 lcd display and one or more motion detectors. After detecting a break-in the device sends an e-mail to the user. It also allows users to arm and disarm the alarm. Passwords are stored on the server and can be changed via a website. The system will also notify about multiple incorrect password entries.

## Tools
- CooCox CoIDE, Version 1.7.8
- STM Studio
- Visual Studio 2015
- Anaconda 2.7.11 Windows https://www.continuum.io/downloads
- Fedora 23 Workstation + Python 2.7
- Geany 1.27 https://www.geany.org/
- Pycharm Community 2016.1.4
- Apache 2.4.20 http://httpd.apache.org/
- Xammp For Windows 64bit
- Build in php server http://php.net/manual/en/features.commandline.webserver.php
-Python Liblaries:
	- os
	- mySQL
	- cgi
	- socket	
	-SimpleHTTPServer


## How to compile
- The code for STM32 must be compiled for each user individually, since WLAN data is hard-coded.
- To do that, simply open the project in CooCox, follow the steps described below and compile.
- You do not need to perform additional actions to compile other modules.
-To use http server you must have administator priviledge
-Files .html, .php, .py and .cgi must be in suitable folders - the paths dependents on Operating System, and firewalls should be down on WindowsOS


## How to run
- open CoIDE,connect the display, keyboard, motion sensor and Wi-Fi module to pins described in "lcd.c", "keypad.c", "md.c" and "wifi.c" files
- in "wifi.c" file, in line AT+CWJAP_CUR=\"SSID\",\"PASSWORD\" replace SSID and PASSWORD with your WLAN ssid and password. In function wifi_connectToServer() replace the ip address, so that the STM32 can connect to the server aplication. After that build the project and flash it to STM32F4-Discovery board
- If you did previous steps correctly, the lcd should display the logo after a short while. It means it is connected to your WLAN. 
- "#" arms the alarm. It is disarmed by default.
- pin is a four-digit code. To enter it just press any of the numeric keys, the password entering screen will appear.
- "*" is used to undo key presses.
- after entering the pin, the microcontroller should connect to your server and verify the pin.
- If a connected motion detector triggers an interrupt, and a correct pin is not entered within 20 seconds, an email is sent.

## Future improvements
- Allow users to set up WLAN data without recompiling

## Licence
- This project could be used under MIT license

## Credits
- Dominika Pawlaczyk
- Dominika Czarnecka
- Tomasz Fengier

#### The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
#### Supervisor: Tomasz Mańkowski
