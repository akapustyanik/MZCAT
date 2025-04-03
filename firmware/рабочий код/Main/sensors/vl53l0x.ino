void setID() {
  for (int i = 0; i < 6; i++) {
    pinMode(lox_mosfets[i], OUTPUT);
    digitalWrite(lox_mosfets[i], LOW);
  }

  for (int i = 0; i < 6; i++) {
    pinMode(lox_shts[i], OUTPUT);
    // all reset
    digitalWrite(lox_shts[i], LOW);
    delay(10);
  }

  for (int i = 6; i >= 0; i--) {
    // initing LOXs
    delay(10);
    digitalWrite(lox_shts[i], HIGH);
    delay(10);
    lox[i].setTimeout(500);
    lox[i].setAddress(lox_adresses[i]);
    if (!lox[i].init() and i != 6) {
      Serial.println("Failed to boot " + String(i) + " VL53L0X");
      while (1)
        ;
    }
    lox[i].startContinuous();
    delay(10);
  }
}

void read_lasers() {
  for (int i = 0; i < 6; i++) {
    lasers[i] = lox[i].readRangeContinuousMillimeters();
    if (lox[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  }
}

void first_read() {
  static uint32_t myTimer1;
  myTimer1 = millis();
  while (millis() - myTimer1 < 200) {
    read_lasers();
    delay(1);
  }
}