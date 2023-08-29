 // libreria sensor tempHum()
#include "SHT85.h"
#include "stdio.h"
// incluir address para comunicacion I2C
#define SHT85_ADDRESS 0x44
uint32_t start;
uint32_t stop;
SHT85 sht;

// variables de tiempo
#define min60 3600000
#define min10 600000
#define min5 300000
#define s30 30000
int Hour = 10;
int Minute = 40;
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
  shp = (float)((analogRead(sH)/(float)1023));
  sht.read();
  int t = sht.getTemperature();
  int ahp = sht.getHumidity();

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
sht.read();
sht.readStatus();
int t = sht.getTemperature();
int ahp = sht.getHumidity();
Serial.print("The time is:");
Serial.print(Hour);
Serial.print(":");
Serial.print(Minute);
Serial.print("\t");
Serial.print("the temperature is");
Serial.print(t,1);
Serial.print("\t");
Serial.print("The air humidity is");
Serial.print(ahp,1);
Serial.print("\t");
Serial.print("moisture of media is");
Serial.print(shp,3);
if (Minute >= 60){
  Hour = Hour + 1;
  Minute = Minute-60;
}
if (Hour>= 24){
Hour = 0;
}
if (Hour>7 && Hour<22){
  digitalWrite(trigRED, HIGH);
  digitalWrite(trigBLUE, HIGH);
  //control de humedad del suelo
  shp = (float)((analogRead(sH)/(float)1023));
  sht.read();
  int t = sht.getTemperature();
  int ahp = sht.getHumidity();
  Serial.print("the temperature is");
  Serial.print(t,1);
  Serial.print("\t");
  Serial.print("The air humidity is");
  Serial.print(ahp,1);
  Serial.print("\t");
  Serial.print("moisture of media is");
  Serial.print(shp,3);
  delay(10000);
  if(Hour==9){
    fanOff();
    Serial.print("\t");
    Serial.print("watering");
    digitalWrite(trigWP, HIGH);
    delay(3000);
    digitalWrite(trigWP, LOW);
    fanStandby();}
  if(Hour==12){
    fanOff();
    Serial.print("\t");
    Serial.print("watering");
    digitalWrite(trigWP, HIGH);
    delay(3000);
    digitalWrite(trigWP, LOW);
    fanStandby();}
  if(Hour==15){
    fanOff();
    Serial.print("\t");
    Serial.print("watering");
    digitalWrite(trigWP, HIGH);
    delay(3000);
    digitalWrite(trigWP, LOW);
    fanStandby();}
  if(Hour==18){
    fanOff();
    Serial.print("\t");
    Serial.print("watering");
    digitalWrite(trigWP, HIGH);
    delay(3000);
    digitalWrite(trigWP, LOW);
    fanStandby();}
  //control de temperatura
  tempHum();
  if(t>=28){
    //inicializar ventilador a velocidad alta 
    fanHigh();
    delay(min5);
    fanStandby();
    Minute = Minute +5;
  } else if(t<24){
    fanHigh();
    Serial.print("heating");
    digitalWrite(trigHeat, HIGH);
    delay(60000);
    digitalWrite(trigHeat, LOW);
    fanStandby();
    Minute = Minute +1;
  }
  //control de humedad del aire
  tempHum();
  if(ahp<90){
    fanOff();
    digitalWrite(trigHum, HIGH);
    delay(60000);
    digitalWrite(trigHum, LOW);
    fanStandby();
    Serial.print("\t");
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
  delay(min60);
  Hour = Hour+1;
}
if (Hour>22 || Hour<9){
  delay(1000);
  //Serial.print("the temperature is");
  //Serial.print(t,2);
  //Serial.print("\t");
  //Serial.print("The air humidity is");
  //Serial.print(ahp,3);
  //Serial.print("\t");
  //Serial.print("moisture of media is");
  //Serial.print(shp,3);
  digitalWrite(trigRED, LOW);
  digitalWrite(trigBLUE, LOW);
  fanOff();
   tempHum();
  if(ahp<80){
    fanOff();
    digitalWrite(trigHum, HIGH);
    delay(60000);
    digitalWrite(trigHum, LOW);
    fanStandby();
    Serial.print("\t");
    Serial.print("The new air humidity is");
    int ahp = sht.getHumidity();
    Serial.print(ahp,1);
    Minute = Minute + 1;
  }
  if (t<18){
    fanHigh();
    digitalWrite(trigHeat, HIGH);
    delay(60000);
    digitalWrite(trigHeat, LOW);
    fanStandby();
    delay(180000);
    fanOff();
    delay(540000);
    Minute = Minute + 13;
  } else{
    delay(min60);
    Hour = Hour+1;;
  }
}}