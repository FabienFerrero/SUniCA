# SUniCA
Space Université Côte d'Azur Board for Education

Version 0.1, September, 2023

Author: Fabien Ferrero, Manh Thao Nguyen, Trinh Le Huy, Xuan Minh

# ATC-COMMAND
version 0.1.2

The LORA-WAN AT-COMMAND are based on the RUI3 platform provided by RAK : [RUI3 AT-COMMAND](https://docs.rakwireless.com/RUI3/Serial-Operating-Modes/AT-Command-Manual/#content)

Additional custom AT-COMMAND are provided to access to the specific feature of SUCA board.

- ```ATC+VER=?``` : return version of the firmware

## Temperature-Humidity (SHT-C3)
- ```ATC+SHT``` : Init SHT sensor, need to be activitated to measure temperature and humidity
- ```ATC+TEMP=?``` : return the value of temperature with 0.01° resolution, return 0 if not available
- ```ATC+HUM=?``` return the value of humidity with 1% resolution, return 0 if not available

## Accelerometer (KX023-1025-FR)
- ```ATC+KX023``` : Init KX023 sensor, need to be activated to measure acceleration
- ```ATC+AX=?``` : return the value of X acceleration with 0.01G resolution, return 0 if not available
- ```ATC+AY=?``` : return the value of Y acceleration with 0.01G resolution, return 0 if not available
- ```ATC+AZ?``` : return the value of Z acceleration with 0.01G resolution, return 0 if not available
- ```ATC+TAP``` : activate Tap mode to detect Single or Double Tap, return 0 if not available
- ```ATC+ACC_READ``` : Read continuously accelerometer X,Y and Z axis, return 0 if not available

## Luminosity sensor (LTR303)
- ```ATC+LTR=?``` : return 1 if sensor is available, return 0 of not
- ```ATC+LUMCH0=?``` : return the value of CH0, return 0 if not available
- ```ATC+LUMCH1=?``` : return the value of CH1, return 0 if not available
- ```ATC+LUM=?``` : return the value of CH1, return 0 if not available

## GPS (LC76F)
- ```ATC+GPSPWR=?``` : 1:0 Turn ON GPS LDO/ Turn OFF GPS LDO, return OK
- ```ATC+GPS``` : Update GPS results, return 1 if LC76F is available, return 0 of not
- ```ATC+GPSFIX=?``` : return 1 if GNSS get a fix, return 0 of not
- ```ATC+GPSSAT=?``` : return number of satellite available
- ```ATC+GPSTIME=?``` : return GPS time in EPOCH format, 0 if not available
- ```ATC+GPSLAT=?``` : return GPS Latitude, 0 if not available
- ```ATC+GPSLON=?``` : return GPS Longitude, 0 if not available
- ```ATC+GPSALT=?``` : return GPS Altitude, 0 if not available
- ```ATC+GPSNMEA=<1/0>``` : Activate NMEA log from GNSS module
- ```ATC+GPSLOG=<1/0>``` : Activate GNSS Log from GNSS module
- ```ATC+GPSDC=<1/0>``` : Set GNSS module in duty cycle mode with 30sec sleep
## Battery voltage
- ```ATC+BAT=?``` : return battery voltage in mv, 0 if not available
- ```ATC+LDO=?``` : return LDO voltage on Rak3172 in mv, 0 if not available

## LoRaWAN
- ```ATC+SENDSENSORS``` : Send a LoRaWAN packet with Temperature, humidity, Luminosity, Accelerometer and GNSS data in Cayenne format



# License


This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

<img src="https://github.com/FabienFerrero/UCA21/blob/main/Doc/Pictures/UCA_logo.png">

Maintained by Fabien Ferrero and Trinh Le Huy
