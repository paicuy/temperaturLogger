# temperaturLogger
Projekt Status: https://suoy.monday.com/boards/1578955167/pulses/1578955196


Schaltbild:

![image](https://user-images.githubusercontent.com/86537226/129707013-428c15ba-4fd0-421d-99a0-32ec8f4c69ab.png)



ESP 8266 Programmierung mit Arduino:

1. Download Arduino
https://downloads.arduino.cc/arduino-1.8.15-windows.exe
https://downloads.arduino.cc/arduino-1.8.15-linux64.tar.xz





2. Arduino starten

![image](https://user-images.githubusercontent.com/86537226/129705830-07318ac6-7e52-4e57-8612-ad9d7fe4927f.png)

3.1 Installieren zusätzlicher Boards 

In Datei, Einstellungen unter
Zusätzliche Board Manager URLs:
https://dl.espressif.com/dl/package_esp32_index.json
http://arduino.esp8266.com/stable/package_esp8266com_index.json

Tools > Boards > Boards Manager
ESP8266 suchen und installieren

3.2 Installieren zusätzliche Bibliotheken:
OneWire, DallasTemperature, u8g2,


Datei > Einstellungen
Zusätzliche Board Manager URLs: https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
Tools > Boards > Boards Manager
ESP8266 suchen und installieren


4. Test-Scripts unter Development/Scripts laden,
um die Hardware auf Funktion zu überprüfen.

> Port unter Werkzeuge auf Microcontroller einstellen
Ausgabe des seriellen Monitors anzeigen:
> Serieller Monitor anzeigen [baud rate = 115200] (Strg+Umsch+M)






Recherchekram:

ESP8266 GPIO:
https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/

How To start Adroino with ESP8266 (Youtube): 
https://www.youtube.com/watch?v=OC9wYhv6juM

How To Adroino with ESP8266:
https://arduino-esp8266.readthedocs.io/en/latest/installing.html

How to ESP8266 with DS18B20:
https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/

How to ESP8266 to Raspberry Pi:
https://www.youtube.com/watch?v=OFaow_TPv6s

Raspberry Pi img:
https://www.raspberrypi.org/software/operating-systems/#raspberry-pi-os-32-bit

Adruino Software:
https://www.arduino.cc/en/software


