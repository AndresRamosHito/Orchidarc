// libreria sensor tempHum()
#include "SHT85.h"
#include "stdio.h"
#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
int status = WL_IDLE_STATUS;
char ssid[] = "PINEMEDIA-008275";        // your network SSID (name)
char pass[] = "severo36";
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
// para sensor de soilHum()
int phragwa;
int trigWP = 5, trigHeat = 6, trigAP = 2, trigLED = 3;
//float shp; //soil humidity percentage
int t; float ahp; //temperature and air humidity percentage //water level percentage

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

void PumpPhrag(){
  analogWrite(trigWP, 100);
}
void Pumpoff(){
  analogWrite(trigWP, 0);
}

void setup() {
  Serial.begin(9600);
  //sensor tempHum()
  Wire.begin();
  sht.begin(SHT85_ADDRESS);
  Wire.setClock(100000);

while (!Serial) {

    ;
  }

  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");
    while (true);

  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");

  }
  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();
  pinMode(trigWP, OUTPUT);
  pinMode(trigHeat, OUTPUT);
  pinMode(trigAP, OUTPUT);
  pinMode(trigLED, OUTPUT);
  digitalWrite(trigWP, LOW);
  digitalWrite(trigHeat, LOW);
  digitalWrite(trigAP, LOW);
  digitalWrite(trigLED, LOW);
  int Hour = 0;
  int Minute = 0;
}
void loop() {
   while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Udp.begin(localPort);
  Serial.println();
  sendNTPpacket(timeServer);
  delay(1000);
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
    int Hour = ((epoch % 86400L)/ 3600+1);
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
    delay(1000);
    if (Hour==6 || Hour==10){
      phragwa = 0;
    } 
    tempHum();
    if (Hour>7 && Hour<22){
      digitalWrite(trigLED, HIGH);
      Serial.print("the temperature is ");
      Serial.print(t,1);
      Serial.println();
      Serial.print("The air humidity is ");
      Serial.println(ahp,1);
      if ((Hour == 9 || Hour == 19) && phragwa == 0 ){
        phragwa = 1;
        Serial.println();
        PumpPhrag();
        delay(50000);
        Pumpoff();
        Serial.print("Watering at");
        Serial.print(Hour,1);
      }
      tempHum();
      if(t>=27){
        delay(min5);
      } else if(t<22){
        Serial.print("heating");
        digitalWrite(trigHeat, HIGH);
        delay(300000);
        digitalWrite(trigHeat, LOW);
        Serial.println();
        Serial.print("the new temperature is");
        Serial.print(t,1);
      }
      delay(min5);
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
      digitalWrite(trigAP, HIGH);
      delay(min5);
      digitalWrite(trigAP,LOW);
      digitalWrite(trigLED, LOW);
      if (t>16){
       delay (min5);
     }
      if (t<17){
        digitalWrite(trigHeat, HIGH);
        delay(min5);
        digitalWrite(trigHeat, LOW);
        delay(120000);
     }
    }
  }
  delay(120000);
  printWifiStatus();
  WiFi.disconnect();
  status = !WL_CONNECTED;
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
  Udp.beginPacket(address, 123);
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