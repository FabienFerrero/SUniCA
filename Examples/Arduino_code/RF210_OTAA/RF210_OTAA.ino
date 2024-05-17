// ESP32 C3 SERIAL1 (second UART)
HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

String appEUI = "0000000000000000";
String devEUI = "0000000000000000";
String appkey = "000000000000000000000000000000000";


void setup()
{
  Serial.begin(115200);
  delay(1000);
 Serial.println("OTAA test");
  
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
  delay(1000);   

   Serial.println("Setup at command");
  mySerial1.println("AT+NJM=1"); // Set OTAA
  delay(300);
  
  mySerial1.println("AT+NWM=1"); // Set LORAWAN
  Serial.println("AT+NWM=1 // Set LORAWAN"); // Set OTAA
  delay(200);
  flush_serial_AT(true);
  delay(1000);
  flush_serial_AT(true);
  mySerial1.println("AT+NJM=1"); // Set OTAA
  Serial.println("AT+NJM=1 // Set OTAA mode"); // Set OTAA
  delay(200);
  flush_serial_AT(true); 
  mySerial1.println("AT+BAND=4");// Set EU868 frequency band
  Serial.println("AT+BAND=4 // Set EU868 frequency band");// Set EU868 frequency band
  delay(200);
  flush_serial_AT(true);
  mySerial1.println("AT+DR=2");// Set SF10
  Serial.println("AT+DR=2 // Set SF10 (default is SF12)");// Set SF10  
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
  delay(1000);
  flush_serial_AT(true);

   if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  delay(1000);

  mySerial1.println("AT+JOIN");

     if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  delay(1000);

}

void loop()
{
//mySerial1.println("AT+JOIN");

int humidity = analogRead(0);
Serial.print("Read humidity ground sensor :");
Serial.println(humidity);


int t=(int) 10*measure_temp(); // return temperature in tens of degree
unsigned char mydata[8];
mydata[0] = 0x1; // CH1
mydata[1] = 0x67; // Temp
mydata[2] = t >> 8;
mydata[3] = t & 0xFF;
mydata[4] = 0x1; // CH1
mydata[5] = 0x3; // Temp
mydata[6] = humidity >> 8;
mydata[7] = humidity & 0xFF;


char str[32] = "";
array_to_string(mydata, 8, str);  // from mydata to string of N characters
Serial.println(str);

mySerial1.printf("AT+SEND=3:");
mySerial1.println(str);
   delay(5300);
  if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  Serial.println("AT set complete with downlink");
  delay(30000);
  Serial.println("Uplink");
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

flush_serial_AT();// flush AT Serial reading buffer
  
mySerial1.println("ATC+TEMP=?"); // Request bat value
 String temperature;
 delay(300);

 if(mySerial1.available()){
        temperature = mySerial1.readStringUntil('\n');
        Serial.print("Temperature:");
        Serial.println(temperature);
 }
 
return temperature.toFloat();
 }

void flush_serial_AT(){

   if (mySerial1.available())
  { // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      Serial.write(mySerial1.read()); // read it and send it out Serial (USB)
  }
  delay(100);
}

void flush_serial_AT(bool print){

  //while(mySerial1.available() == 0) {}

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
