# AT-COMMAND EXAMPLES
Space Université Côte d'Azur Board for Education

Version 0.4, Feb, 2024

Author: Fabien Ferrero, Manh Thao Nguyen, Trinh Le Huy, Xuan Minh

In these examples, the ATC (custom AT-COMMAND) and macro-command are demonstrated for P2P communication

This lab is intended to be run with a live Spectogram using a SDR receiver.
We recommand to use RTL2832 USB dongle with CubicSDR software.

Two different sections are proposed in this tutorial :

 1. [Continous Wave](https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand.md#lorawan-abp-activation-by-personalization)
 2. [P2P FSK](https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand.md#lorawan-abp-activation-by-personalization)
 3. [P2P LoRa](https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand.md#lorawan-otaa)



<h1>Before starting this tutorial</h1>

*  Dip Switch 2 has to be in ESP32-C3 mode to connect UART between ESP32-C3 and RAK3172

*  Open in Arduini IDE [Examples/RF210_RAK3172_Lab/RF210_Lab.ino](/Examples/RF210_Lab/RF210_Lab.ino)

*  Click on the upload button in Arduino IDE (for LoRaWan Example, you can update the credentials)

* Open Serial Monitor with 115200 baud and select "Both NL & CR"

* ==Warning== : Echo mode has to be disabled for this tutorial, you can use ATE command to toggle the echo mode
  
* check Rak3172 Firmware version, type :
```            
ATC+VER
```            
It should return version 0.1.2 or higher: 
* ATC+VER=xx.yy.zz    


<h1>Continuous Wave</h1>

# Configure CW parameters 

0. To start a Continous Wave @868MHz with 10dBm output Power and during 1 second, enter :
   
```	
$startCW
```
   
1. To start you own CW, you can now use the AT command :  
```            
AT+CW=XX:YY:ZZ
```            
XX : RF frequency in Hz (ie 865000000)
YY : Tx power in dBm (from 5 to 22)
ZZ : Duration in second



<h1>P2P FSK Modulation</h1>

0. In order to set-up the FSK configuration, a simple meta-command is available. It will configure a FSK @2400bps at 868MHz
   
```	
$startFSK
```

1. Configure your frequency

```	
AT+PFREQ=XX
```
XX : RF frequency in Hz (ie 865000000)


2. Test different FSK baudrate, type :
 In order to evaluate the effect of FSK data rate on the modulation, 3 different configurations are available :  
2400 bps :

```            
$$startFSK2400
```       

2400 bps :

```            
$$startFSK2400
```     

2400 bps :

```            
$$startFSK2400
```          



<h1>P2P LoRa Modulation</h1>



<h1>P2P LoRa Communication between two SUNICA boards</h1>


# License

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

<img src="../Document/pic/UniCA_logo.png">

Maintained by Fabien Ferrero and Trinh Le Huy
