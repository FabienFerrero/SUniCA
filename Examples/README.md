# SUniCA EXAMPLES
Space Université Côte d'Azur Board for Education
Version 0.3, October, 2023
Author: Fabien Ferrero, Manh Thao Nguyen, Trinh Le Huy, Xuan Minh

In these examples, the main terminal features are showcased

Before starting Examples, the Arduino IDE and board manager should be installed as described in main Readme document :
[Main](main/readme.md)

# 1/ AT-COMMAND example

*  Dip Switch 2 has to be in ESP32-C3 mode to connect UART between ESP32-C3 and RAK3172

*  Open in Arduini IDE [Examples/RF210_RAK3172_Lab/RF210_Lab.ino](/Examples/RF210_Lab/RF210_Lab.ino)

*  Click on the upload button in Arduino IDE (for LoRaWan Example, you can update the credentials)

* Open Serial Monitor with 115200 baud

* Type AT-Command in "Message" text box.

* A step-by-step tutorial is available here : [atcommand](atcommand.md)

# 2/ LoRaWAN Activation by Personalization example

*  If you are not familiar with LoRaWAN, useful information is available on:   [https://www.univ-smb.fr/lorawan/en/free-book/](https://www.univ-smb.fr/lorawan/en/free-book/)

*  Open in Arduini IDE [Examples/RF210_ABP/RF210_ABP.ino](/Examples/RF210_ABP/RF210_ABP.ino)  , the code is simply providing the different AT-COMMAND to set a LoRaWan ABP transmission.

*  Update the DevAdr, NWKkey and AppsKey credential and click on upload button in Arduino IDE

*  For a step by step tutorial with TTN LoRaWAN network, follow this training : [abp_ttn](abp_ttn.md)

# License

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

<img src="../Document/pic/UniCA_logo.png">

Maintained by Fabien Ferrero and Trinh Le Huy
