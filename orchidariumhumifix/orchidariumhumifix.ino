// libreria sensor tempHum()
#include "SHT85.h"
#include "stdio.h"
#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
int status = WL_IDLE_STATUS;
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "PINEMEDIA-008275";        // your network SSID (name)
char pass[] = "severo36";
int keyIndex = 0;
unsigned int localPort = 2390;
const char* ntpServer = "uk.pool.ntp.org";
//IPAddress timeServer(129, 6, 15, 28);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
WiFiUDP Udp;
// incluir address para comunicacion I2C
#define SHT85_ADDRESS 0x44
#define DS3231_ADDRESS 0x57
uint32_t start;
uint32_t stop;
SHT85 sht;

// variables de tiempo
#define min10 600000
#define hour 3600000
#define min5 300000
#define s30 30000
// para sensor de waterLevel()
int wL = A2;
int wlp = 0;
// para sensor de soilHum()
int sH = A1;
int shp = 0;
int wtrtdy;
int trigWP = 5, trigFan = 3, trigHeat = 6, trigSV = 9, trigRED = 12, trigBLUE =10, trigHum = 7, clockon = 8;
//float shp; //soil humidity percentage
int t; float ahp; //temperature and air humidity percentage //water level percentage

//reading soil humidity and turning it into percentage
void soilHum(){
  shp = (float)((analogRead(sH)/(float)1023));
}

void tempHum(){
  int i=0;
  sht.read();
  t = sht.getTemperature();
  for(i=0;i<50;i++) //for loop added to read humidity 30 times before proceeding
  {
  ahp = sht.getHumidity();
  if(isnan(ahp)){Serial.println("issue with humididty");} // comment down this if you are getting constant trouble
  delay(100);
  } 
}

void waterLevel(){
  int wlp = analogRead(wL);
}

// velocidades venitlador
void fanHigh(){
  analogWrite(trigFan, 160);
}

void fanStandby(){
  analogWrite(trigFan, 140);
}
void fanLow(){
  analogWrite(trigFan, 50);
}

void fanOff(){
  analogWrite(trigFan, 0);
}

void setup() {
  Serial.begin(9600);
  //sensor tempHum()
  Wire.begin();
  sht.begin(SHT85_ADDRESS);
  Wire.setClock(100000);

while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }

  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true);

  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");

  }

  // attempt to connect to Wifi network:

  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();

  //triggers transistores sistema
  pinMode(trigWP, OUTPUT);
  pinMode(trigFan, OUTPUT);
  pinMode(trigHeat, OUTPUT);
  pinMode(trigHum, OUTPUT);
  pinMode(trigSV, OUTPUT);
  pinMode(trigRED, OUTPUT);
  pinMode(trigBLUE, OUTPUT);
  pinMode(clockon, OUTPUT);
  //inciar con todo apagado menos el ventilador
  digitalWrite(trigWP, LOW);
  digitalWrite(trigHeat, LOW);
  digitalWrite(trigHum, LOW);
  digitalWrite(trigSV, LOW);
  digitalWrite(trigRED, LOW);
  digitalWrite(trigBLUE, LOW);
  digitalWrite(clockon, LOW);
  //inicializar ventilador a velocidad media
  analogWrite(trigFan, 150);
  int Hour = 0;
  int Minute = 0;
}
void loop() {
  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");

    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:

    delay(10000);

  }
  Serial.println("Connected to wifi");
  printWifiStatus();
  IPAddress timeServerIP;
  WiFi.hostByName(ntpServer, timeServerIP);
  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);
  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    int Hour = ((epoch % 86400L)/ 3600);
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    int Minute = (epoch  % 3600) / 60;
    Serial.print(':');
    if ((epoch % 60) < 10) {
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
    shp = analogRead(A0);
    delay(1000);
    //sht.read();
    if (Hour == 7){
      wtrtdy = 0;
    }
    delay(1000);
    //int t = sht.getTemperature();
    //int ahp = sht.getHumidity();
    tempHum();
    Serial.println(wlp);
    if (Hour>7 && Hour<22){
      int ahp = sht.getHumidity();
      digitalWrite(trigRED, HIGH);
      digitalWrite(trigBLUE, HIGH);
      Serial.print("the temperature is ");
      Serial.print(t,1);
      Serial.println();
      Serial.print("The air humidity is ");
      Serial.println(ahp,1);
      //Serial.print("water level is ");
      //Serial.print(wlp);
      Serial.println();
      //Serial.print("moisture of media is");
      //Serial.print(shp);
      delay(1000);
      if(Hour==13 && wtrtdy == 0){
        fanOff();
        Serial.println();
        Serial.print("watering");
        Serial.println();
        digitalWrite(trigWP, HIGH);
        delay(7000);
        digitalWrite(trigWP, LOW);
        fanStandby();
        wtrtdy = 1;
      }
      //control de temperatura
      tempHum();
      if(t>=28){
        //inicializar ventilador a velocidad alta 
        fanHigh();
        delay(min5);
        fanStandby();
      } else if(t<21){
        fanHigh();
        Serial.print("heating");
        digitalWrite(trigHeat, HIGH);
        delay(300000);
        digitalWrite(trigHeat, LOW);
        Serial.println();
        Serial.print("the new temperature is");
        Serial.print(t,1);
        fanStandby();
      }
      //control de humedad del aire
      tempHum();
      if(ahp<97){
        fanHigh();
        digitalWrite(trigSV, LOW);
        delay(2000);
        digitalWrite(trigWP, HIGH);
        delay(4000);
        digitalWrite(trigWP, LOW);
        delay(3000);
        digitalWrite(trigSV, LOW);
        digitalWrite(trigHum, LOW);
        //delay(1000000);
        digitalWrite(trigHum, LOW);
        fanStandby();
        Serial.println();
        Serial.print("The new air humidity is");
        Serial.print(ahp,1);
        delay(min5);
      }
      //conrtol de nivel de agua
      waterLevel();
      if(wlp<550 && wlp>450){
        fanStandby();
        digitalWrite(trigWP, LOW);
        digitalWrite(trigSV, LOW);
        delay(2000);
        digitalWrite(trigWP, LOW);
        delay(3000);
        digitalWrite(trigSV, LOW);
        delay(2000);
        fanStandby();
      }
      delay(6000);
    }
    //Once the code begins this part it stops.
    if (Hour>21 || Hour<9){
      delay(1000);
      Serial.print("the temperature is");
      Serial.print(t,1);
      Serial.println();
      Serial.print("The air humidity is");
      Serial.print(ahp,1);
      Serial.println();
      Serial.print("moisture of media is");
      Serial.print(shp,3);
      digitalWrite(trigRED, LOW);
      digitalWrite(trigBLUE, LOW);
      if (t>16){
        delay (min5);
     }
      if (t<17){
        fanHigh();
        digitalWrite(trigHeat, HIGH);
        delay(300000);
        digitalWrite(trigHeat, LOW);
        fanStandby();
        delay(120000);
     }
      tempHum();
      if(ahp<93){
        fanHigh();
        digitalWrite(trigSV, LOW);
        delay(2000);
        digitalWrite(trigWP, HIGH);
        delay(3500);
        digitalWrite(trigWP, LOW);
        delay(3000);
        digitalWrite(trigSV, LOW);
        digitalWrite(trigHum, LOW);
        //delay(1000000);
        digitalWrite(trigHum, LOW);
        fanStandby();
        Serial.println();
        Serial.print("The new air humidity is");
        int ahp = sht.getHumidity();
        Serial.print(ahp,1);
        delay(min5);
      }
    }

  }
  // wait ten seconds before asking for the time again
  delay(10000);
}

// CLOCK SYNCRONISATION
// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address) {

  //Serial.println("1");

  // set all bytes in the buffer to 0

  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  // Initialize values needed to form NTP request

  // (see URL above for details on the packets)

  //Serial.println("2");

  packetBuffer[0] = 0b11100011;   // LI, Version, Mode

  packetBuffer[1] = 0;     // Stratum, or type of clock

  packetBuffer[2] = 6;     // Polling Interval

  packetBuffer[3] = 0xEC;  // Peer Clock Precision

  // 8 bytes of zero for Root Delay & Root Dispersion

  packetBuffer[12]  = 49;

  packetBuffer[13]  = 0x4E;

  packetBuffer[14]  = 49;

  packetBuffer[15]  = 52;

  //Serial.println("3");

  // all NTP fields have been given values, now

  // you can send a packet requesting a timestamp:

  Udp.beginPacket(address, 123); //NTP requests are to port 123

  //Serial.println("4");

  Udp.write(packetBuffer, NTP_PACKET_SIZE);

  //Serial.println("5");

  Udp.endPacket();

  //Serial.println("6");


}
         //WIFI STATUS
void printWifiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your board's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}
