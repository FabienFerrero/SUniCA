# AT-COMMAND EXAMPLES
Space Université Côte d'Azur Board for Education

Version 0.2, October, 2023

Author: Fabien Ferrero, Manh Thao Nguyen, Trinh Le Huy, Xuan Minh

In these examples, the ATC (custom AT-COMMAND) and macro-command are demonstrated.

Three different section are proposed :
1. [Sensors Evaluation](Sensors evaluation)
2. LoRaWAN ABP
3. LoRaWAN OTAA

<h1>Sensors evaluation</h1>


# SHT temperature and humidity sensor 
    
1. Measure temperature, type :
```            
ATC+TEMP
```            
It returns a result in Celsius degree: 
* ATC+TEMP=xx.yy      

2. Measure Humidity, type :
```            
ATC+HUM
```             
It returns a result in percent of humidity :
* ATC+HUM=xx
    

# KX023 Accelerometer sensor

1. Check the accelerometer value on x-axis, type :
```	
ATC+AX
```	 
It returns the result in G unit : 

* ATC+AX=-x.yy
    

2. Check the accelerometer value on y-axis, type :
```	 
ATC+AY
```	
It returns the result in G unit : 

* ATC+AY=x.yy


3. Check the accelerometer value on z-axis, type :
```	
ATC+AZ
```	
It returns the result in G unit : 

* ATC+AZ=x.yy
  
# LTR303 Luminosity sensor

1. Check the luminisoty value , type :
```	
ATC+LUM
```	 
It returns the result in Lux unit : 

* ATC+AX=-x.yy

# L76F GNSS sensor

1. GNSS experiment needs to be run with a clear view on the sky to capture signal from GPS, Galileo or Beidu Satellite.
   

2. Activate the GNSS LDO, it power-on the L76F module, type :
```	
    ATC+GPSPWR=1
```	
It returns : 

* OK

3. The GNSS module will need up to 1 minute to obtain a position. If LED D1 (close to L76F) is blinking, the module get a fix.
   
4. Plot log from the GPS , type
```	
ATC+LOG=1
```	
It returns : 

* GNSS log from Module
    
5. Read the number of satellites in view, type :

```	
ATC+GPSSAT
```	
It returns the number of satellites available : 

* ATC+GPSSAT=x
	
6. Read the latitude, type : 
```	
ATC+GPSLAT
```	
It returns the latitude in degreees : 
	
* ATC+GPSLAT=xx.yyyy
	

7. Read the Longitude, type :
```	
ATC+GPSLON
```	
It returns the longitude in degrees : 

* ATC+GPSLON=xx.yyyy
    

8. Read the Altitude, type : 
```	
ATC+GPSALT
```	
It returns the altitude in meter : 

* ATC+GPSALT=xx.yyy
    

9. Read the Time, type : 
```	
ATC+GPSTIME
```	
It returns the time in second in Epoch format : 

* ATC+GPSTIME=xxxxxxxxxx
    

# Battery Voltage

1. Request battery voltage level, type :
```	
ATC+BAT
```	
It returns the battery voltage level in millivolt : 

* ATC+BAT=xxxx   




# License

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

<img src="https://github.com/FabienFerrero/UCA21/blob/main/Doc/Pictures/UCA_logo.png">

Maintained by Fabien Ferrero and Trinh Le Huy
