# AT-COMMAND FOR P2P Communication
Space Université Côte d'Azur Board for Education

Version 0.4, Feb, 2024

Author: Fabien Ferrero, Manh Thao Nguyen, Trinh Le Huy, Xuan Minh

In these examples, the ATC (custom AT-COMMAND) and macro-command are demonstrated for P2P communication

This lab is intended to be run with a live Spectogram using a SDR receiver.
We recommand to use RTL2832 USB dongle with CubicSDR software.

Two different sections are proposed in this tutorial :

 1. [Continous Wave](https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand_LoRaP2P.md#continuous-wave)
 2. [P2P FSK](https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand_LoRaP2P.md#p2p-fsk-modulation)
 3. [P2P LoRa Modulation](https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand_LoRaP2P.md#p2p-lora-modulation-ccs-)
 4. [P2P LoRa Communication between two SUNICA boards] https://github.com/FabienFerrero/SUniCA/blob/main/Examples/atcommand_LoRaP2P.md#p2p-lora-communication-between-two-sunica-boards



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

1. Configure FSK mode

In order to set-up the FSK P2P configuration, a simple meta-command is available. It will configure a FSK @600bps at 868MHz
   
```	
$startFSK
```

2. Configure your frequency

```	
AT+PFREQ=XX
```
XX : RF frequency in Hz (ie 865000000)


3. Test different FSK baudrate

 In order to evaluate the effect of FSK data rate on the modulation, 3 different configurations with H index = 1 are available :  
Send 192 bytes with 600 bps & 600Hz BW :

```            
$$startFSK600
```       

Send 192 bytes with 6000 bps & 6kHz BW :

```            
$$startFSK6000
```     

Send 192 bytes with 60000 bps & 60kHz BW :

```            
$$startFSK60000
```          


<h1>P2P LoRa Modulation (CCS) </h1>

1. Configure LoRa P2P

In order to set-up the LoRa P2P configuration, a simple meta-command is available. It will configure a LoRa with SF12 at 868MHz
   
```	
$startLoRa
```

2. Configure your frequency

```	
AT+PFREQ=XX
```
XX : RF frequency in Hz (ie 865000000)


3. Test different LoRa Bandwidth

 In order to evaluate the effect of Bandwitdth on the LoRa modulation, this command will sequentially transmit 5 lora packets with bandwidth 31.25KHz, 62.5KHz, 125KHz, 250KHz and 500KHz.
 The 5 packets have the same Spreading Factor (SF10) and payload (helloWorld)).

```            
$testloraBW
```    

4. Test different LoRa Spreading Factor

In order to evaluate the effect of Spreading Factor on the LoRa modulation, this command will sequentially transmit 8 lora packets with Spreading Factor 5, 6, 7, 8, 9, 10, 11 and 12.
 The 8 packets have the same Bandwidth (125KHz) and payload (helloWorld)).

```            
$testloraSF
```      

<h1>P2P LoRa Communication between two SUNICA boards</h1>

This part of the lab requires two SUNICA terminals. One terminal will be the Transmitter and the second will be the receiver.

0. Before starting the communication, you must agree on the configuration to be used. Especially, you have to define the frequency, bandwidth and Spreading Factor that will be used.

1. Initialize LoRa P2P

Use the meta-command to reset all modulation parameters. It will configure a LoRa with SF12 at 868MHz
   
```	
$startLoRa
```

2. Configure your frequency, SF and BW

```	
AT+PFREQ=XX
```
XX : RF frequency in Hz (ie 865000000)

```	
AT+PBW=YY
```
YY : Freq. Bandwidth [0 = 125, 1 = 250, 2 = 500, 3 = 7.8, 4 = 10.4, 5 = 15.63, 6 = 20.83, 7 = 31.25, 8 = 41.67, 9 = 62.5]

```	
AT+PSF==ZZ
```
ZZ : Spreading Factor [5;12]

3.Activate Receive mode on the first terminal

The transciever can be set in receiving mode. Any other operation will be impossible while this mode is activated. 
```	
$startRx
```
If you need to change any parameters, you have to switch off the Rx mode using :
```	
$stopRx
```
4.Send a LoRa packet from the second terminal using
```	
$sendTXT
```

The serial will propose to type your text message. Use any ASCII symbol. The code will convert your ASCII text into a HEX code and send it through a LoRa packet.

5. The message is received on the first terminal. The first line prodive some information about the receiption signal. Your decoded text is on the second line.

```	
+EVT:RXP2P:XX:YY:ZZ
```	

RSSI = XX    SNR = YY    Payload = ZZ

<h1>P2P Encrypted LoRa Communication between two SUNICA boards</h1>

As you understand, the actual communication can be received by any terminal which is configured with the right RF parameters. 
In order to keep privacy, the lora packet can be encrypted using a 128bit AES key. 

1. Define the encryption key

The encryption key is composed with 16 bytes. The Rx must be stop before setting the key.
   
```	
AT+ENCKEY=0123456789ABCDEF0123456789ABCDEF
```

The Encryption key must be only known by the tranmitter and receiver. 

2. Activate the encryption on LoRa communication on both terminal

```	
AT+ENCRY=1
```	

3. You can send message using 
```	
$sendTXT
```


# License

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

<img src="../Document/pic/UniCA_logo.png">

Maintained by Fabien Ferrero and Trinh Le Huy
