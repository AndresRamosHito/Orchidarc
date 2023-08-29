
#include "Arduino.h"
#include "SHT85.h"
#include "stdio.h"
#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "GravityTDS.h"
GravityTDS gravityTds;
int status = WL_IDLE_STATUS;
char ssid[] = "PINEMEDIA-008275";
char pass[] = "severo36";
#define NTP_SERVER "time.google.com"
int keyIndex = 0;
unsigned int localPort = 2390;
IPAddress timeServer(129, 6, 15, 28);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
WiFiUDP Udp;
#define SHT85_ADDRESS 0x44
#define DS3231_ADDRESS 0x57
uint32_t start;
uint32_t stop;
SHT85 sht;

// variables de tiempo
#define min10 600000
#define hour 3600000
#define min5 300000
#define min6 360000
#define s30 30000
// para sensor de TDS()
#define TdsSensorPin A0
float temperature = 25,tdsValue = 0;
int waterstart;
int waterfreq;
int wtrtdy = 0;
int trigWP = 5, trigFan = 9, trigHeat = 6, trigAP = 2, trigLED = 3, trigHum = 12;
//float shp; //soil humidity percentage
int t; float ahp; //temperature and air humidity percentage //water level percentage

//void tempHum(){
  //int i=0;
  //sht.read();
  //t = sht.getTemperature();
  //for(i=0;i<50;i++) //for loop added to read humidity 30 times before proceeding
  //{
  //ahp = sht.getHumidity();
  //if(isnan(ahp)){Serial.println("issue with humididty");} // comment down this if you are getting constant trouble
  //delay(1000);
  //} 
//}
void ledLow(){
  analogWrite(trigLED, 100);
}
void PumpOff(){
  analogWrite(trigWP, 0);
}
void PumpSustain(){
  analogWrite(trigWP, 140);
}
void PumpFill(){
  analogWrite(trigWP, 255);
}
void PumpDrain(){
  analogWrite(trigWP, 50);
}
void HeatMax(){
  analogWrite(trigHeat, 255);
}
void Heatlow(){
  analogWrite(trigHeat, 100);
}
// velocidades venitlador
void fanHigh(){
  analogWrite(trigFan, 255);
}

void fanStandby(){
  analogWrite(trigFan, 200);
}
void fanLow(){
  analogWrite(trigFan, 150);
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
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

while (!Serial) {
    ; 
  }

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");

  }
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);
  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();
  //triggers transistores sistema
  pinMode(trigWP, OUTPUT);
  pinMode(trigFan, OUTPUT);
  pinMode(trigHeat, OUTPUT);
  pinMode(trigHum, OUTPUT);
  pinMode(trigAP, OUTPUT);
  pinMode(trigLED, OUTPUT);
  //inciar con todo apagado menos el ventilador
  digitalWrite(trigWP, LOW);
  digitalWrite(trigHeat, LOW);
  digitalWrite(trigHum, LOW);
  digitalWrite(trigAP, LOW);
  digitalWrite(trigLED, LOW);
  digitalWrite(trigFan, LOW);
  //inicializar ventilador a velocidad media
  digitalWrite(trigFan, 150);
  int Hour = 0;
  int Minute = 0;
}
void loop() {
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(10000);
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    Udp.read(packetBuffer, NTP_PACKET_SIZE); 
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    int Hour = ((epoch % 86400L)/ 3600)+1;
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
     gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    delay(1000);
    delay(1000);
    //int t = sht.getTemperature();
    //int ahp = sht.getHumidity();
    if (Hour == 6 || Hour == 12 || Hour == 16){
      wtrtdy=0;
    }
    //tempHum();
    if (Hour>7 && Hour<22){
      digitalWrite(trigLED, HIGH);
      Serial.print("the temperature is ");
      Serial.print(t,1);
      Serial.println();
      Serial.print("The air humidity is ");
      Serial.println(ahp,1);
      Serial.println();
      digitalWrite(trigAP, HIGH);
      delay(min5);
      digitalWrite(trigAP, LOW);
      if((Hour == 13) && wtrtdy == 0){
        digitalWrite(trigHeat, LOW);
        Serial.println();
        Serial.print("watering");
        Serial.println();
        PumpFill();
        digitalWrite(trigAP, HIGH);
        delay(min6);
        PumpSustain();
        digitalWrite(trigHum, HIGH);
        delay(min5);
        digitalWrite (trigHum, LOW);
        PumpDrain();
        delay(min5);
        digitalWrite(trigAP, LOW);
        PumpOff();
        fanStandby();
        wtrtdy = 1;
      }
      //tempHum();
      t = 24;
      if(t>=25){
        delay(min5);
        fanStandby();
      } else if(t<22){
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
      //tempHum();
      ahp = 75;
      if(ahp<70){
        delay(2000);
        digitalWrite(trigAP, HIGH);
        delay(min5);
        digitalWrite(trigWP, HIGH);
        delay(120000);
        PumpSustain();
        digitalWrite(trigHum, HIGH);
        delay(120000);
        digitalWrite(trigAP, LOW);
        digitalWrite(trigWP, LOW);
        digitalWrite (trigHum, LOW);
        Serial.println();
        Serial.print("The new air humidity is");
        int ahp = sht.getHumidity();
        Serial.print(ahp,1);
        delay(min5);
      }
     delay(min5);
    }
    if (Hour>21 || Hour<8){
      t = 20;
      delay(1000);
      Serial.print("the temperature is");
      Serial.print(t,1);
      Serial.println();
      Serial.print("The air humidity is");
      Serial.print(ahp,1);
      digitalWrite(trigLED, LOW);
      digitalWrite(trigAP, HIGH);
      delay(min5);
      digitalWrite(trigAP, LOW);
      if (t>16){
      delay (min5);
      }
      if (t<15){
        digitalWrite(trigHeat, HIGH);
        delay(300000);
        digitalWrite(trigHeat, LOW);
        fanStandby();
        delay(min5);
      }
    }

  }
  delay(1000);
}
unsigned long sendNTPpacket(IPAddress& address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
