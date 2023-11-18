/*

   ___    _   _             _      ___                      ___      ___    ___      _       __   
  / __|  | | | |  _ _      (_)    / __|   __ _      o O O  | _ \    | __|  |_  )    / |     /  \  
  \__ \  | |_| | | ' \     | |   | (__   / _` |    o       |   /    | _|    / /     | |    | () | 
  |___/   \___/  |_||_|   _|_|_   \___|  \__,_|   TS__[O]  |_|_\   _|_|_   /___|   _|_|_   _\__/  
_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""| {======|_|"""""|_| """ |_|"""""|_|"""""|_|"""""| 
"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'./o--000'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-' 

   Author: fferrero, m1nhle, mtnguyen
   Code : RF210_RAK3172_Lab : Provide on ESP32 Serial port a direct access in write and read to Rak3172 module for AT Command mode

 */
 //Library

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Define Default LoRaWan ABP credential

String devAddr = "260BBD3A";
String nwkkey = "41E878B4C85999BECA298C6205A202BF";
String appskey = "250F552CB5C43F1EB13282F6422A6E2D";

// ESP32 C3 SERIAL1 (second UART)
HardwareSerial mySerial1(1);

// BLE scan
int scanTime = 5; //In seconds
BLEScan* pBLEScan;

int period = 30; // period to send LoRaWAN packet
int rx_delay = 0;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
     // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

int rxPin = 20;
int txPin = 21;
uint8_t button = 0;
char c;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("RF210 Lab");

  Serial.println("Setup BLE");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(200);
  pBLEScan->setWindow(200);  // less or equal setInterval value

  
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(10, OUTPUT); //Rak enable
  pinMode(4, OUTPUT); // LED
   pinMode(9, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(9), i_button_isr, RISING);
 // pinMode(1, OUTPUT); // GNSS enable
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  

  //  digitalWrite(1, LOW);   // switch off GPS
    digitalWrite(10, HIGH); // Switch on RAK
    delay(1000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);
  delay(1000);  
  mySerial1.println("ATE");
  delay(100);
  
    while (mySerial1.available()){
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
    }
 
}

void loop()
{

 
  if (button > 0){
  
 mySerial1.println("ATC+GPSON=1");
  delay(300);
   mySerial1.println("ATC+GPSCONST");
  delay(300);
  mySerial1.println("ATC+GPSNMEA=1");
  delay(300);
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(200);  
  button=0;    
  }
  
  if (Serial.available()) {      // If anything comes in Serial (USB),
  c=Serial.read();
  if(c=='$'){     
    digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);                       // wait for a second
    digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
    
    String CMD=Serial.readStringUntil('\r');
    Serial.println(CMD);           
    if(CMD.equalsIgnoreCase("BLEscan")){
      BLEscan(true);
      } 
    else if(CMD.equalsIgnoreCase("SetABP")){
      SetLoRaABP();
      }  
    else if(CMD.equalsIgnoreCase("SendABP")){
      SendLoRaABP("BABA");
      }
    else{
     Serial.println("Wrong Command");
        }  
  }
    else{
    mySerial1.write(c);   // read it and send it out Serial1  
    }    
  }

  if (mySerial1.available()) {     // If anything comes in Serial1 
    Serial.write(mySerial1.read());   // read it and send it out Serial (USB)
  }
}

void i_button_isr(void) {
button++; 
}

int BLEscan(boolean log){

      Serial.println("Start Scanning BLE during 5s");
      BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
      int scan_num = foundDevices.getCount();
      if (log){
      Serial.print("Devices found: ");      
      Serial.println(scan_num);
      checklist(foundDevices,scan_num);
      }
      foundDevices.dump();
      return scan_num;
}


void SetLoRaABP(){
   mySerial1.println("AT+NJM=0"); // Set ABP
   Serial.println("AT+NJM=0"); // Set ABP
  delay(200);
  flush_serial_AT(); 
  mySerial1.println("AT+BAND=4");// Set EU868 frequency band
  Serial.println("AT+BAND=4");// Set EU868 frequency band
  delay(200);
  flush_serial_AT();

  mySerial1.println("AT+DR=5");// Set EU868 frequency band
  Serial.println("AT+DR=5");// Set EU868 frequency band  
  delay(200); 
  flush_serial_AT();
  mySerial1.printf("AT+DEVADDR=");
  mySerial1.println(devAddr);
  Serial.printf("AT+DEVADDR=");
  Serial.println(devAddr);
  delay(200);
  flush_serial_AT();
  mySerial1.printf("AT+NWKSKEY=");
  mySerial1.println(nwkkey);
  Serial.printf("AT+NWKSKEY=");
  Serial.println(nwkkey);
  delay(200);
  flush_serial_AT();
  mySerial1.printf("AT+APPSKEY=");
  mySerial1.println(appskey);
  delay(200);
}


void SendLoRaABP(String str){
flush_serial_AT(); 
blink();

mySerial1.printf("AT+SEND=3:");
mySerial1.println(str);
Serial.printf("AT+SEND=3:");
Serial.println(str);

delay(200);
mySerial1.readStringUntil('\n');
while (mySerial1.available() == 0)
{
rx_delay=rx_delay+100;
delay(100);
}
Serial.print("Rx delay : ");
Serial.println(rx_delay);

   if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  Serial.println("AT set complete with downlink");
  int wait_time=period*1000 - rx_delay;
  rx_delay=0;
  delay(wait_time);
 
}
  
 void array_to_string(byte array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

 // Return temperature level in degree
 float measure_temp(){

//Serial.flush();
flush_serial_AT();// flush AT Serial reading buffer
  
mySerial1.println("ATC+TEMP=?"); // Request bat value
 String temperature;
 delay(100);

 if(mySerial1.available()){
        temperature = mySerial1.readStringUntil('\n');
        Serial.print("Temperature:");
        Serial.println(temperature);
 }
 
return temperature.toFloat();
 }

 // Return humidity level in percent
 float measure_hum(){

//Serial.flush();
flush_serial_AT();// flush AT Serial reading buffer
  
mySerial1.println("ATC+HUM=?"); // Request bat value
 String hum;
 delay(100);

 if(mySerial1.available()){
        hum = mySerial1.readStringUntil('\n');
        Serial.print("Humidity:");
        Serial.println(hum);
 }
 
return hum.toFloat();
 }

// Return humidity level in percent
 float measure_lum(){

//Serial.flush();
flush_serial_AT();// flush AT Serial reading buffer
  
mySerial1.println("ATC+LUM=?"); // Request bat value
 String lum;
 delay(100);

 if(mySerial1.available()){
        lum = mySerial1.readStringUntil('\n');
        Serial.print("Luminosity:");
        Serial.println(lum);
 }
 
return lum.toFloat();
 } 

  // Return Acceleration level in G
 float measure_acc(int axis){

//Serial.flush();
flush_serial_AT();// flush AT Serial reading buffer

if(axis==1){  
mySerial1.println("ATC+AX=?"); // Request bat value
}
else if(axis==2){  
mySerial1.println("ATC+AY=?"); // Request bat value
}
else if(axis==3){  
mySerial1.println("ATC+AZ=?"); // Request bat value
}
 String a;
 delay(100);
 if(mySerial1.available()){
        a = mySerial1.readStringUntil('\n');
        Serial.print("Acc:");
        Serial.println(a);
 }
 
return a.toFloat();
 }

void flush_serial_AT(){

   if (mySerial1.available())
  { // If anything comes in mySerial1
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
      mySerial1.read();
  }
  delay(100);
}


void blink(){
digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
} 

// Function check list
// Compare the new BLE scan with detected BLE list
// Increment counter and append BLE list if new device detected 
void checklist(BLEScanResults foundDevices, int scan_num) {
  
for (int i=0;i<scan_num;i++){
    Serial.printf("Device: %x : ", foundDevices.getDevice(i).getAddress());
    Serial.print("  RSSI :");
    Serial.println(foundDevices.getDevice(i).getRSSI());
    delay(50);
                   
   } // end for 
}
