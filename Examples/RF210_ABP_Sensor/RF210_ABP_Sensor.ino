// ESP32 C3 SERIAL1 (second UART)
HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

int period = 30; // period to send LoRaWAN packet
int rx_delay = 0;

String devAddr = "00000000";
String nwkkey = "00000000000000000000000000000000";
String appskey = "00000000000000000000000000000000";


void setup()
{
  Serial.begin(115200);
  delay(1000);
 Serial.println("ABP test");
  
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

 mySerial1.println("AT+BAUD=921600"); // Set Baud rate
 delay(100);
  mySerial1.begin(921600, SERIAL_8N1, rxPin, txPin);

 

  Serial.println("Setup at command");
  mySerial1.println("AT+NJM=0"); // Set ABP
 // delay(300);
  mySerial1.println("AT+BAND=4");// Set EU868 frequency band
//  delay(300);
  mySerial1.println("AT+DR=5");// Set EU868 frequency band
 // delay(300);
 
  mySerial1.printf("AT+DEVADDR=");
  mySerial1.println(devAddr);
  //delay(300);
    mySerial1.printf("AT+NWKSKEY=");
  mySerial1.println(nwkkey);
 // delay(300);  
  mySerial1.printf("AT+APPSKEY=");
  mySerial1.println(appskey);
 // delay(300);
  mySerial1.printf("ATC+TEMP=?");
 

 while (mySerial1.available() == 0)
{
Serial.println("Waiting");
delay(500);
  }

flush_serial_AT(); 
delay(500); 
  
}

void loop()
{


int16_t t=(int16_t) 10*measure_temp(); // return temperature in tens of degree
uint8_t h=(uint8_t)2*measure_hum(); // return humidity in percent
int16_t x = 1000*measure_acc(1);
int16_t y = 1000*measure_acc(2);
int16_t z = 1000*measure_acc(3);
int16_t l = measure_lum();


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
mydata[i++] = 0x71; // Hum
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



char str[32] = "";
array_to_string(mydata, i, str);


flush_serial_AT();
//Serial.flush();
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
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      //Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
      mySerial1.read();
  }
  delay(100);
}


void blink(){
digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
} 
