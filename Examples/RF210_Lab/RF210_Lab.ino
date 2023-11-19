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

String devAddr = "00000000";
String nwkkey = "00000000000000000000000000000000";
String appskey = "00000000000000000000000000000000";

String appEUI = "0000000000000000";
String devEUI = "0000000000000000";
String appkey = "00000000000000000000000000000000";

// ESP32 C3 SERIAL1 (second UART)
HardwareSerial mySerial1(1);

// BLE scan
int scanTime = 5; //In seconds
int BLE_near = 0;
int BLE_tot = 0;
BLEScan* pBLEScan;

int period = 30000; // period to send LoRaWAN packet in ms
unsigned long currentMillis = 0, getSensorDataPrevMillis = 0;
boolean lora_sending=false;

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
  blink(500);

  //  digitalWrite(1, LOW);   // switch off GPS
    digitalWrite(10, HIGH); // Switch on RAK
    delay(1000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);
  delay(1000);  
  mySerial1.println("ATR");  // Reset RAK3172
  delay(200);
  
    while (mySerial1.available()){
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
    }
 
}

void loop()
{

 
  if (button > 0){

  SetLoRaABP();
  delay(2000);
  lora_sending=!lora_sending;
  delay(300);
  blink(200);
  button=0;    
  }

  if (lora_sending==1){
      currentMillis = millis();  
      
      if (currentMillis - getSensorDataPrevMillis > period){
        getSensorDataPrevMillis=currentMillis;
        SendLoRa(1);        
      }
  }
  
  if (Serial.available()) {      // If anything comes in Serial (USB),
  c=Serial.read();
  if(c=='$'){     
    blink(50);
    
    String CMD=Serial.readStringUntil('\r');
    Serial.println(CMD);           
    if(CMD.equalsIgnoreCase("BLEscan")){
      BLEscan(true);
      } 
    else if(CMD.equalsIgnoreCase("SetABP")){
      SetLoRaABP();
      }  
    else if(CMD.equalsIgnoreCase("SetOTAA")){
      SetLoRaOTAA();
      } 
    else if(CMD.equalsIgnoreCase("SendLoRa")){
      SendLoRa(1);
      }
    else if(CMD.equalsIgnoreCase("Periodic")){
      lora_sending=!lora_sending;
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
      Serial.print("Devices found: ");      
      Serial.println(scan_num);
      if (log){
      checklist(foundDevices,scan_num);
      }
      foundDevices.dump();
      return scan_num;
}


void SetLoRaABP(){
   mySerial1.println("AT+NJM=0"); // Set ABP
   Serial.println("AT+NJM=0 // Set ABP"); // Set ABP
  delay(200);
  flush_serial_AT(true); 
  mySerial1.println("AT+BAND=4");// Set EU868 frequency band
  Serial.println("AT+BAND=4 // Set EU868 frequency band");// Set EU868 frequency band
  delay(200);
  flush_serial_AT(true);

  mySerial1.println("AT+DR=5");// Set EU868 frequency band
  Serial.println("AT+DR=5 // Set EU868 frequency band");// Set EU868 frequency band  
  delay(200); 
  flush_serial_AT(true);
  mySerial1.printf("AT+DEVADDR=");
  mySerial1.println(devAddr);
  Serial.printf("AT+DEVADDR=");
  Serial.println(devAddr);
  delay(200);
  flush_serial_AT(true);
  mySerial1.printf("AT+NWKSKEY=");
  mySerial1.println(nwkkey);
  Serial.printf("AT+NWKSKEY=");
  Serial.println(nwkkey);
  delay(200);
  flush_serial_AT(true);
  mySerial1.printf("AT+APPSKEY=");
  mySerial1.println(appskey);
  Serial.printf("AT+APPSKEY=");
  Serial.println(appskey);
  delay(200);
  flush_serial_AT(true);
}

void SetLoRaOTAA(){
   mySerial1.println("AT+NJM=1"); // Set OTAA
  Serial.println("AT+NJM=1 // Set OTAA mode"); // Set OTAA
  delay(200);
  flush_serial_AT(true); 
  mySerial1.println("AT+BAND=4");// Set EU868 frequency band
  Serial.println("AT+BAND=4 // Set EU868 frequency band");// Set EU868 frequency band
  delay(200);
  flush_serial_AT(true);

  mySerial1.println("AT+DR=5");// Set SF7
  Serial.println("AT+DR=5 // Set SF7 (default is SF12)");// Set SF7  
  delay(200); 
  flush_serial_AT(true);
  mySerial1.printf("AT+DEVEUI=");
  mySerial1.println(devEUI);
  Serial.printf("AT+DEVEUI=");
  Serial.println(devEUI);
  delay(200);
  flush_serial_AT(true);
  mySerial1.printf("AT+APPEUI=");
  mySerial1.println(appEUI);
  Serial.printf("AT+APPEUI=");
  Serial.println(appEUI);
  delay(200);
  flush_serial_AT(true);
  mySerial1.printf("AT+APPKEY=");
  mySerial1.println(appkey);
  Serial.printf("AT+APPKEY=");
  Serial.println(appkey);
  delay(200);
  flush_serial_AT(true);
}


bool SendLoRa(uint8_t mode){

int16_t rx_delay=0;
int16_t t=(int16_t) 100*measure_temp(); // return temperature in cents of degree
uint8_t h=(uint8_t)2*measure_hum(); // return humidity in percent
int8_t x = 50*measure_acc(1);
int8_t y = 50*measure_acc(2);
int8_t z = 50*measure_acc(3);
int16_t l = 10*measure_lum();
uint16_t b = BLEscan(false);
uint16_t bat = measure_bat();


//int blocks=7;
int i=0;
unsigned char mydata[64];
mydata[i++] = t >> 8;
mydata[i++] = t & 0xFF;
mydata[i++] = h;
mydata[i++] = x;
mydata[i++] = y;
mydata[i++] = z;
mydata[i++] = l >> 8;
mydata[i++] = l & 0xFF;
mydata[i++] = b >> 8;
mydata[i++] = b & 0xFF;
mydata[i++] = bat >> 8;
mydata[i++] = bat & 0xFF;

char str[32];
array_to_string(mydata, i, str);

flush_serial_AT(false); 
blink(100);
mySerial1.printf("AT+SEND=4:");
mySerial1.println(str);

while (mySerial1.available() == 0)
{
delay(100);
}
mySerial1.readStringUntil('\n');
while (mySerial1.available() == 0)
{
rx_delay=rx_delay+100;
delay(100);
if(rx_delay>8000){
  Serial.println("Error during uplink process (do you perform the JOIN ?)");
  return false;}
}
Serial.print("Rx delay : ");
Serial.println(rx_delay);


   if (mySerial1.available())
  { // If anything comes in Serial1
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  Serial.println("AT set complete with downlink");
  return true;
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
flush_serial_AT(false);// flush AT Serial reading buffer
  
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
flush_serial_AT(false);// flush AT Serial reading buffer
  
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


flush_serial_AT(false);// flush AT Serial reading buffer
  
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


flush_serial_AT(false);// flush AT Serial reading buffer

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
 }
 
return a.toFloat();
 }

// Return bat level in mv
 float measure_bat(){

//Serial.flush();
flush_serial_AT(false);// flush AT Serial reading buffer
  
mySerial1.println("ATC+BAT=?"); // Request bat value
 String bat;
  while (mySerial1.available() == 0)
{
delay(100);
  }
 delay(100);

 if(mySerial1.available()){
        bat = mySerial1.readStringUntil('\n');
        Serial.print("Bat:");
        Serial.println(bat);
 }
 
return bat.toFloat();
 } 

void flush_serial_AT(bool print){

   if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      if(print) {Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
      }
      else {
      mySerial1.read();}
  }
  delay(100);
}



void blink(int blinktime){
digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(blinktime);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
} 

// Function check list
// Compare the new BLE scan with detected BLE list
// Increment counter and append BLE list if new device detected 
void checklist(BLEScanResults foundDevices, int scan_num) {
  
  BLE_tot = scan_num;
  BLE_near = 0;
  int RSSI;

for (int i=0;i<scan_num;i++){
    Serial.printf("Device: %x : ", foundDevices.getDevice(i).getAddress());
    Serial.print("  RSSI :");
    RSSI = foundDevices.getDevice(i).getRSSI();
    Serial.println(RSSI);
    if(RSSI > -75) {BLE_near++;}                   
   } // end for 
 Serial.println(BLE_near);
}
