// libreria sensor tempHum()
#include "SHT85.h"
#include "stdio.h"
#include <DS3231.h>
#include <Wire.h>
#include <time.h>
// incluir address para comunicacion I2C
#define SHT85_ADDRESS 0x44
#define DS3231_ADDRESS 0x57
uint32_t start;
uint32_t stop;
SHT85 sht;
int Hour = 20;
int Minute = 20;
// variables de tiempo
#define min10 600000
#define min5 300000
#define s30 30000
// para sensor de waterLevel()
#define wL A0
// para sensor de soilHum()
#define sH A1
//variables soilHum()
/*const int Moistureair = 607;
const int Moisturewater = 344;
const int range = Moistureair - Moisturewater;*/

int trigWP = 5, trigFan = 3, trigHeat = 2, trigSV = 4, trigRED = 12, trigBLUE =13, trigHum = 7;
float shp; //soil humidity percentage
int t; float ahp; //temperature and air humidity percentage
float wlp; //water level percentage

//reading soil humidity and turning it into percentage
void soilHum(){
  shp = (float)((analogRead(sH)/(float)1023));
}

void tempHum(){
  t = sht.getTemperature();
  ahp = sht.getHumidity();
}

void waterLevel(){
  wlp = (float)analogRead(wL)/(float)1023;
}

// velocidades venitlador
void fanHigh(){
  analogWrite(trigFan, 200);
}

void fanStandby(){
  analogWrite(trigFan, 80);
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
  //inciar con todo apagado menos el ventilador
  digitalWrite(trigWP, LOW);
  digitalWrite(trigHeat, LOW);
  digitalWrite(trigHum, LOW);
  digitalWrite(trigSV, LOW);
  digitalWrite(trigRED, LOW);
  digitalWrite(trigBLUE, LOW);
  //inicializar ventilador a velocidad media
  analogWrite(trigFan, 127);
}

void loop() {
shp = (float)((analogRead(sH)/(float)1023));
delay(5000);
sht.read();
int t = sht.getTemperature();
int ahp = sht.getHumidity();
if (Hour>7 && Hour<22){
  digitalWrite(trigRED, HIGH);
  digitalWrite(trigBLUE, HIGH);
  Serial.print("the temperature is");
  Serial.print(t,1);
  Serial.println();
  Serial.print("The air humidity is");
  Serial.print(ahp,1);
  Serial.println();
  Serial.print("moisture of media is");
  Serial.print(shp,3);
  delay(10000);
  if(shp>0.200){
    fanOff();
    Serial.println();
    Serial.print("watering");
    Serial.println();
    digitalWrite(trigWP, HIGH);
    digitalWrite(trigSV, HIGH);
    delay(1000);
    digitalWrite(trigWP, LOW);
    delay(2000);
    digitalWrite(trigSV, LOW);
    fanStandby();}
  //control de temperatura
  tempHum();
  if(t>=28){
    //inicializar ventilador a velocidad alta 
    fanHigh();
    delay(min5);
    fanStandby();
  } else if(t<24){
    fanHigh();
    Serial.print("heating");
    digitalWrite(trigHeat, HIGH);
    delay(60000);
    digitalWrite(trigHeat, LOW);
    Serial.println();
    Serial.print("the new temperature is");
    Serial.print(t,1);
    fanStandby();
  }
  //control de humedad del aire
  tempHum();
  if(ahp<95){
    fanOff();
    digitalWrite(trigHum, HIGH);
    delay(60000);
    digitalWrite(trigHum, LOW);
    fanStandby();
    Serial.println();
    Serial.print("The new air humidity is");
    int ahp = sht.getHumidity();
    Serial.print(ahp,1);
    Minute = Minute + 1;
  }
  //conrtol de nivel de agua
  waterLevel();
  if(wlp<400 and wlp>100){
    fanOff();
    digitalWrite(trigWP, HIGH);
    digitalWrite(trigSV, HIGH);
    delay(2000);
    digitalWrite(trigWP, LOW);
    delay(3000);
    digitalWrite(trigSV, LOW);
    delay(2000);
    fanStandby();
  }
  delay(300000);
  Minute = (Minute+5);
}
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
  if (t>17){
  delay (min5);
  Minute = Minute + 5;
  }
  if (t<18){
    fanHigh();
    digitalWrite(trigHeat, HIGH);
    delay(60000);
    digitalWrite(trigHeat, LOW);
    fanStandby();
    delay(180000);
    fanOff();
    delay(120000);
    Minute = Minute + 5;
  }
}
}