// ESP32 C3 SERIAL1 (second UART)
HardwareSerial mySerial1(1);

#define SLEEP    1      // Define if LoRa packet are sent to network

int rxPin = 20;
int txPin = 21;

int period = 30; // period to send LoRaWAN packet
int rx_delay = 0;
int8_t DC=0;

String devAddr = "260B8F3A";
String nwkkey = "784E918B1A403743889FB445E8261895";
String appskey = "BF2E0A2E28B5E5378408C3864C138A57";


void setup()
{
  Serial.begin(115200);
  delay(1000);
 Serial.println("ABP tracker");
  
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);

  pinMode(10, OUTPUT); //Rak enable
  pinMode(4, OUTPUT); // LED


  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  

    digitalWrite(10, HIGH); // Switch on RAK
    delay(1000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);

//  mySerial1.println("AT+BAUD=921600"); // Set Baud rate
//  delay(100);
//   mySerial1.begin(921600, SERIAL_8N1, rxPin, txPin);

 

  Serial.println("Setup at command");
  //mySerial1.println("ATR"); // Set ABP
  mySerial1.println("AT+NJM=0"); // Set ABP
 // delay(300);
  mySerial1.println("AT+BAND=4");// Set EU868 frequency band
//  delay(300);
  mySerial1.println("AT+DR=5");// Set EU868 frequency band
 // delay(300);
 
  mySerial1.print("AT+DEVADDR=");
  mySerial1.println(devAddr);
  //delay(300);
    mySerial1.print("AT+NWKSKEY=");
  mySerial1.println(nwkkey);
 // delay(300);  
  mySerial1.print("AT+APPSKEY=");
  mySerial1.println(appskey);
 // delay(300);
  while (mySerial1.available() == 0)
{
Serial.println("Waiting");
delay(200);
  }

flush_serial_AT(false); 
 mySerial1.println("ATC+GPSON=1"); // Activate GNSS
delay(500); 
 mySerial1.println("ATC+GPSON=1"); // Activate GNSS
delay(500); 
flush_serial_AT(false);   
}

void loop()
{
int16_t t=(int16_t) 10*measure_temp(); // return temperature in tens of degree
uint8_t h=(uint8_t)2*measure_hum(); // return humidity in percent
int16_t x = 1000*measure_acc(1);
int16_t y = 1000*measure_acc(2);
int16_t z = 1000*measure_acc(3);
int16_t l = measure_lum();
int16_t b = measure_bat()/10;
int32_t LatitudeBinary= 10000* measure_gnss(1); //Latitude : 0.0001 ° Signed MSB
int32_t LongitudeBinary= 10000*measure_gnss(2); //Longitude : 0.0001 ° Signed MSB
int32_t AltitudeBinary= 100* measure_gnss(3); // Altitude : 0.01 meter Signed MSB
uint16_t s= 100*measure_gnss(4); // nb of satellite in view with GNSS



//int blocks=7;
int i=0;
unsigned char mydata[64];
mydata[i++] = 0x1; // CH1
mydata[i++] = 0x67; // Temp
mydata[i++] = t >> 8;
mydata[i++] = t & 0xFF;
mydata[i++] = 0x2; // CH1
mydata[i++] = 0x68; // Hum
mydata[i++] = h;
mydata[i++] = 0x3; // CH1
mydata[i++] = 0x71; // Acc
mydata[i++] = x >> 8;
mydata[i++] = x & 0xFF;
mydata[i++] = y >> 8;
mydata[i++] = y & 0xFF;
mydata[i++] = z >> 8;
mydata[i++] = z & 0xFF;
mydata[i++] = 0x4; // CH4
mydata[i++] = 0x65; // Luminosity
mydata[i++] = l >> 8;
mydata[i++] = l & 0xFF;
mydata[i++] = 0x5; // CH5
mydata[i++] = 0x2; // Bat analog value
mydata[i++] = b >> 8;
mydata[i++] = b & 0xFF;
mydata[i++] = 0x1; // CH1
mydata[i++] = 0x2; // Hum
mydata[i++] = s >> 8;
mydata[i++] = s & 0xFF;
mydata[i++] = 0x1; // CH1
mydata[i++] = 0x1; // Hum
mydata[i++] = DC;
mydata[i++] = 0x6; // CH6
mydata[i++] = 0x88; // GNSS value
mydata[i++] = ( LatitudeBinary >> 16 ) & 0xFF;
mydata[i++] = ( LatitudeBinary >> 8 ) & 0xFF;
mydata[i++] = LatitudeBinary & 0xFF;
mydata[i++] = ( LongitudeBinary >> 16 ) & 0xFF;
mydata[i++] = ( LongitudeBinary >> 8 ) & 0xFF;
mydata[i++] = LongitudeBinary & 0xFF;
mydata[i++] = ( AltitudeBinary >> 16 ) & 0xFF;
mydata[i++] = ( AltitudeBinary >> 8 ) & 0xFF;
mydata[i++] = AltitudeBinary & 0xFF;

char str[i] = "";
array_to_string(mydata, i, str);

flush_serial_AT(false);

blink();
mySerial1.printf("AT+SEND=3:");
mySerial1.println(str);

blink();

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
  Serial.print("AT set complete with downlink : ");
  Serial.println(str);
  int sleep_time=period*1000 - rx_delay; // Sleep duration in ms
  rx_delay=0;
  sleep (sleep_time); 
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
  while (mySerial1.available() == 0)
{
delay(100);
  }


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
  while (mySerial1.available() == 0)
{
delay(300);
  }
 

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
flush_serial_AT(false);// flush AT Serial reading buffer
  
mySerial1.println("ATC+LUM=?"); // Request bat value
 String lum;
  while (mySerial1.available() == 0)
{
delay(100);
  }
 delay(100);

 if(mySerial1.available()){
        lum = mySerial1.readStringUntil('\n');
        Serial.print("Luminosity:");
        Serial.println(lum);
 }
 
return lum.toFloat();
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

  // Return Acceleration level in G
 float measure_acc(int axis){

//Serial.flush();
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
  while (mySerial1.available() == 0)
{
delay(100);
  }
 delay(100);
 if(mySerial1.available()){
        a = mySerial1.readStringUntil('\n');
        Serial.print("Acc:");
        Serial.println(a);
 }
 
return a.toFloat();
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


void blink(){
digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(70);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(70);                       // wait for a second
  digitalWrite(4, HIGH);    // turn the LED off by making the voltage LOW
  delay(70);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
} 

// Sleep function over a period in ms
void sleep(int period) {

  #if SLEEP

  Serial.print("Move to sleep : ");
  Serial.println(period/1000);

  mySerial1.print("AT+SLEEP="); // Set Sleep for RAK3172
  mySerial1.println(period); // Set Sleep
  

   //gpio_hold_en((gpio_num_t)10);
   //esp_sleep_enable_ext0_wakeup(WAKEPIN,0); //1 = Low to High, 0 = High to Low. Pin pulled HIGH
 
  //gpio_light_sleep_hold_en();
  // wake up 1 second later and then go into deep sleep
  esp_sleep_enable_timer_wakeup(period*1000); // in us
  delay(100);

  esp_light_sleep_start();

  #else

  Serial.print("Stop during : ");
  Serial.println(period/1000);
  // Sleep during period
  delay(period);

  #endif

}

  // Return 1:lat 2:long 3:alt 4:sat from GNSS
 float measure_gnss(int axis){

//Serial.flush();
flush_serial_AT(false);// flush AT Serial reading buffer

if(axis==1){  
mySerial1.println("ATC+GPSLAT=?"); // Request lat value
Serial.print("Lat:");
}
else if(axis==2){  
mySerial1.println("ATC+GPSLON=?"); // Request lon value
Serial.print("Long:");
}
else if(axis==3){  
mySerial1.println("ATC+GPSALT=?"); // Request alt value
Serial.print("Altitude:");
}
else if(axis==4){  
mySerial1.println("ATC+GPSSAT=?"); // Request sat value
Serial.print("Sat:");
}
else if(axis==5){  
mySerial1.println("ATC+GPSPWR=?"); // Request sat value
Serial.print("On:");
}

 String a;
 while (mySerial1.available() == 0)
{
delay(100);
  }


if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
  a = mySerial1.readStringUntil('\n');    
  Serial.println(a);
  delay(100);
  mySerial1.readStringUntil('\n');
  }
  

  if(a.toFloat() > 5  && DC==0 && axis==4)
  {
 mySerial1.println("ATC+GPSDC=1"); // Activate GNSS
DC=1;
  }
  if(a.toFloat() < 5  && DC==1 && axis==4)
  {
 mySerial1.println("ATC+GPSDC=0"); // Activate GNSS
DC=0;
  }

 
return a.toFloat();
 }
