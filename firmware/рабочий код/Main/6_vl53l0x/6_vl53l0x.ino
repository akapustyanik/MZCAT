#include <Wire.h>
#include "VL53L0X.h"

VL53L0X lox[7];

int16_t lox_mosfets[7] = { 22, 23, 24, A13, A14, A15 , -1};

int16_t lox_adresses[7] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 , -1};

int16_t lox_shts[7] = { 31, 32, 33, 30, 35, 34 , -1};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  setID();
}

void loop() {
  // put your main code here, to run repeatedly:
  read_lasers();
}

void setID() {
  uint32_t myTimer = millis();
  for (int i = 0; i < 6; i++) {
    pinMode(lox_mosfets[i], OUTPUT);
    digitalWrite(lox_mosfets[i], LOW);
    delay(10);
  }

  for (int i = 0; i < 6; i++) {
    pinMode(lox_shts[i], OUTPUT);
    // all reset
    digitalWrite(lox_shts[i], LOW);
    delay(10);
  }


  for (int i = 0; i < 6; i++) {
    // initing LOXs
    digitalWrite(lox_shts[i], HIGH);
    delay(10);
    lox[i].setAddress(lox_adresses[i]);
    if (!lox[i].init()) {
      Serial.println("Failed to boot " + String(i) + " VL53L0X");
      while (1)
        ;
    }
    lox[i].startContinuous();
    delay(10);
    //digitalWrite(lox_shts[i], LOW);
    //delay(10);
  }
  Serial.println(millis() - myTimer);
}

uint32_t debugTimer;

float lasers[] = {0, 0, 0, 0, 0, 0};

void read_lasers() {
  debugTimer = millis();
  for (int i = 0; i < 6; i++) {

    lasers[i] = lasers[i] * 2 / 3 + lox[i].readRangeContinuousMillimeters() * 1 / 3;
    Serial.print(lasers[i]);
    Serial.print("\t\t");
    if (lox[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  }
  Serial.println();
  //Serial.println(millis() - debugTimer);
}
