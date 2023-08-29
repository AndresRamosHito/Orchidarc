#include "SHT85.h"
#include <Wire.h>

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
int t;
int ahp;
int dayclock;
int trigWP = 5, trigHeat = 6, trigAP = 2, trigLED = 3;

void tempHum() {
  int i = 0;
  start = millis();  // Get the current time
  while (millis() - start < 10000) {  // Run for 10 seconds
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
}

void controlHeater(int pwmValue) {
  start = millis();  // Get the current time
  while (millis() - start < 10000) {  // Run for 10 seconds
    analogWrite(trigHeat, pwmValue);
  }
  analogWrite(trigHeat, 0);  // Turn off the heater after 10 seconds
}

void controlLED(int pwmValue) {
  start = millis();  // Get the current time
  while (millis() - start < 10000) {  // Run for 10 seconds
    analogWrite(trigLED, pwmValue);
  }
  analogWrite(trigLED, 0);  // Turn off the LED after 10 seconds
}

void controlFan(int pwmValue) {
  start = millis();  // Get the current time
  while (millis() - start < 10000) {  // Run for 10 seconds
    analogWrite(trigAP, pwmValue);
  }
  analogWrite(trigAP, 0);  // Turn off the fan after 10 seconds
}

void controlWaterPump(int pwmValue) {
  start = millis();  // Get the current time
  while (millis() - start < 10000) {  // Run for 10 seconds
    analogWrite(trigWP, pwmValue);
  }
  analogWrite(trigWP, 0);  // Turn off the water pump after 10 seconds
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  sht.begin(SHT85_ADDRESS);
  Wire.setClock(100000);

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
  Serial.println("Waiting for serial input...");
  while (!Serial.available()) {
    // Wait for serial input to trigger actions
  }

  int input = Serial.parseInt();

  int pwmValue;
  if (Serial.available()) {
    pwmValue = Serial.parseInt();  // Read the PWM value from serial input
  }

  switch (input) {
    case 1:  // Trigger temperature and humidity reading
      tempHum();
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" °C");
      Serial.print("Humidity: ");
      Serial.print(ahp);
      Serial.println(" %");
      break;
    case 2:  // Trigger pump sustain
      controlWaterPump(pwmValue);
      Serial.println("Pump sustain triggered");
      break;
    case 3:  // Trigger pump fill
      controlWaterPump(pwmValue);
      Serial.println("Pump fill triggered");
      break;
    case 4:  // Trigger pump drain
      controlWaterPump(pwmValue);
      Serial.println("Pump drain triggered");
      break;
    case 5:  // Trigger heater control
      controlHeater(pwmValue);
      Serial.println("Heater control triggered");
      break;
    case 6:  // Trigger LED control
      controlLED(pwmValue);
      Serial.println("LED control triggered");
      break;
    case 7:  // Trigger fan control
      controlFan(pwmValue);
      Serial.println("Fan control triggered");
      break;
    default:
      Serial.println("Invalid input");
      break;
  }
}

