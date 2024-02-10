# AT-COMMAND EXAMPLES
Space Université Côte d'Azur Board for Education

Version 0.4, Feb, 2024

Author: Fabien Ferrero, Manh Thao Nguyen, Trinh Le Huy, Xuan Minh

In these examples, the ATC (custom AT-COMMAND) and macro-command are demonstrated.

Two different sections are proposed in this tutorial :


 1. [LoRaWAN ABP](https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand_LoRaWAN.md#lorawan-abp-activation-by-personalization)
 2. [LoRaWAN OTAA](https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand_LoRaWAN.md#lorawan-otaa)



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


<h1>LoRaWAN ABP (Activation by Personalization)</h1>

# Configure ABP parameters 
You are going to send a LoRaWAN packet to a network server.
If you are using The Things Network, you can follow this tutorial to get your LoRaWAN parameters (devaddress, nwkskey and appskey).
[TTN tuto](abp_ttn.md)

0. It is recommended to reset the Rak3172 module before a new configuration, type :
   
```	
ATZ
```

  In order to speed up the configuration, and meta-command is available  
1. Set ABP parameters, type :
```            
$setABP
```            
It returns the different AT-Command executed. ABP credentials are by default.
They can be personalized using :
```	
AT+DEVADDR=00000000
AT+NWKSKEY=00000000000000000000000000000000
AT+APPSKEY=00000000000000000000000000000000
```	
2. Send ABP LoRaWan packet on Port 3 with "BABA" payload, type :
```	
AT+SEND=3:BABA
```

You can also schedule a downlink from the Network server. After sending the loraWan packet, the downlink will be received on the Rx window and show in your log. 

3. Send ABP LoRaWan packet with RF210 sensor data, type :
```	
$sendLoRa
```

The Javascript decoder to use in the network server is : [decoder](RF210_Lab/decoder.js)

4. To send LoRaWan packet periodically (every 30 sec), type :
   
```	
$periodic
```

<h1>LoRaWan OTAA</h1>

0. It is recommended to reset the Rak3172 module before a new configuration, type :
   
```	
ATZ
```

  In order to speed up the configuration, and meta-command is available  
1. Set OTAA parameters, type :
```            
$setOTAA
```            
It returns the different AT-Command executed. ABP credentials are by default.
They can be personalized using :
```	
AT+DEVEUI=00000000
AT+APPEUI=0000000000000000
AT+APPKEY=0000000000000000
```	
2. Join the LoRaWan network, type :
```	
AT+JOIN
```

If it successfully join the network, it returns  : 

* +EVT:JOINED

3. Send LoRaWan packet on Port 3 with "BABA" payload, type :
```	
AT+SEND=3:BABA
```

4. Send LoRaWan packet with RF210 sensor data, type :
```	
$sendLoRa
```

The Javascript decoder to use in the network server is : [decoder](RF210_Lab/decoder.js)

5. To send LoRaWan packet periodically (every 30 sec), type :
   
```	
$periodic
```

<h1>Automatic LoRaWan ABP mode</h1>

By pushing the B_ESP button on the RF210 terminal, the periodic ABP LoRaWAN sending with sensors value is automatically activated.
It can be used to connect the device on any USB plug and to start a measurement


# License

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

<img src="../Document/pic/UniCA_logo.png">

Maintained by Fabien Ferrero and Trinh Le Huy
