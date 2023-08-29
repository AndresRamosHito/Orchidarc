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
unsigned int localPort = 2389;
IPAddress timeServer(129, 6, 15, 28);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
WiFiUDP Udp;

// variables de tiempo
#define min10 600000
#define hour 3600000
#define min5 300000
#define min6 360000
#define s30 30000
// para sensor de soilHum()
int wtrtdy;
int trigWP = 5, trigHeat = 6, trigAP = 2, trigLED = 3;
//float shp; //soil humidity percentage
int t;
float ahp; //temperature and air humidity percentage //water level percentage

void setup() {
  Serial.begin(9600);
  //sensor tempHum()
  Wire.begin();
  Wire.setClock(100000);

  while (!Serial) {
    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
      Serial.println("Please upgrade the firmware");
    }
    int Hour = 0;
    int Minute = 0;
  }
} // <- Missing closing brace for setup() function

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
    printWifiStatus();
    WiFi.disconnect();
    status = !WL_CONNECTED;
  }
} // <- Missing closing brace for loop() function

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
} // <- Missing closing brace for sendNTPpacket() function

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
} // <- Missing closing brace for printWifiStatus() function

