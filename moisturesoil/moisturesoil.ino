#include "SHT85.h"
#include "stdio.h"
#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <ArduinoHttpClient.h>
#define SHT85_ADDRESS 0x44
#define min10 600000
#define hour 3600000
#define min5 240000
#define min6 360000
#define s30 30000

uint32_t start;
uint32_t stop;
int Hour = 0;
int Minute = 0;
SHT85 sht;
int status = WL_IDLE_STATUS;
char ssid[] = "PINEMEDIA-008275"; // your network SSID (name)
char pass[] = "severo36";
int keyIndex = 0;
unsigned int localPort = 2390;
const char* ntpServer = "uk.pool.ntp.org";
WiFiUDP Udp;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
int t;
int ahp;
int wtrtdy;
int awa;
int Horas;
int dayclock;
int trigWP = 11, trigHeat = 6, trigAP = 2, trigLED = 3, trigDown = 9;

void tempHum() {
  int i = 0;
  sht.read();
  t = sht.getTemperature();
  for (i = 0; i < 50; i++) {
    ahp = sht.getHumidity();
    if (isnan(ahp)) {
      Serial.println("issue with humidity");
    }
    delay(100);
  }
}

void PumpSustain() {
  analogWrite(trigWP, 150);
}

void PumpFill() {
  analogWrite(trigWP, 255);
}

void PumpDrain() {
  analogWrite(trigWP, 50);
}
void PumpOff()  {
  analogWrite(trigWP, 0);
}

void sendNTPpacket(IPAddress& address);
void printWifiStatus();

void setup() {
  Serial.begin(9600);
  Wire.begin();
  sht.begin(SHT85_ADDRESS);
  Wire.setClock(100000);
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();

  pinMode(trigWP, OUTPUT);
  pinMode(trigHeat, OUTPUT);
  pinMode(trigAP, OUTPUT);
  pinMode(trigLED, OUTPUT);
  pinMode(trigDown, OUTPUT);

  digitalWrite(trigWP, LOW);
  digitalWrite(trigHeat, LOW);
  digitalWrite(trigAP, LOW);
  digitalWrite(trigLED, LOW);
  digitalWrite(trigDown, LOW);
  int Hour = 0;
  int Minute = 0;
  Horas = 0;
  wtrtdy = 0;
  awa = 0;
}

void loop() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");
  IPAddress timeServerIP;
  WiFi.hostByName(ntpServer, timeServerIP);
  Udp.begin(localPort);
  Serial.println();
  sendNTPpacket(timeServerIP);
  delay(5000);
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    Udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;

    Serial.print("The UTC time is ");
    Serial.print((epoch  % 86400L) / 3600);
    Hour = ((epoch % 86400L) / 3600);
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
       Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60);
    Minute = (epoch  % 3600) / 60;
    Serial.print(':');
    if ((epoch % 60) < 10) {
      Serial.print('0');
    Serial.println(epoch % 60);
    }
  }
  if (Hour == 6) {
    dayclock = 0;
    awa = 0;
  }
  if (Hour == 7 && dayclock == 0) {
    wtrtdy = 0;
  }

  tempHum();

  if (Hour > 7 && Hour < 22) {
    digitalWrite(trigLED, HIGH);
    Serial.print("the temperature is ");
    Serial.print(t, 1);
    Serial.println();
    Serial.print("The air humidity is ");
    Serial.println(ahp, 1);


    if ((Hour == 9 || Hour == 13 || Hour == 17 || Hour == 21) && (Hour != Horas)) {
      Serial.println();
      Serial.print("watering");
      Serial.println();
      analogWrite(trigWP, 75);
      delay(5000);
      analogWrite(trigWP, 0);
      Horas = Hour;
    }

    tempHum();

    if (t < 22) {
      Serial.print("heating");
      digitalWrite(trigHeat, HIGH);
      delay(300000);
      digitalWrite(trigHeat, LOW);
      Serial.println();
      Serial.print("the new temperature is");
      Serial.print(t, 1);
    }
  }

  if (Hour > 21 || Hour < 7) {
    delay(1000);
    Serial.print("the temperature is");
    Serial.print(t, 1);
    Serial.println();
    Serial.print("The air humidity is");
    Serial.print(ahp, 1);
    Serial.println();
    digitalWrite(trigAP, HIGH);
    delay(min5);
    digitalWrite(trigAP, LOW);
    digitalWrite(trigLED, LOW);
    if (t > 16) {
      delay(min5);
    }
    if (t < 17) {
      digitalWrite(trigHeat, HIGH);
      delay(min5);
      digitalWrite(trigHeat, LOW);
      delay(120000);
    }
  }
  printWifiStatus();
  WiFi.disconnect();
  status = !WL_CONNECTED;
  delay(min5);
}


void sendNTPpacket(IPAddress& address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

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
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}