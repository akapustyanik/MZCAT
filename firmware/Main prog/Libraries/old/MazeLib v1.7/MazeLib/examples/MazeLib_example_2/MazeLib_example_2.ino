#include "MazeLib.h"

MOTORS motors = MOTORS();

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("A:");
  Serial.print(motors.encoder(1));
  Serial.print(" B:");
  Serial.print(motors.encoder(2));
  Serial.print(" C:");
  Serial.print(motors.encoder(3));
  Serial.print(" D:");
  Serial.println(motors.encoder(4));

  if(millis() % 10000 == 0) {
    motors.clear_enc(1);
    motors.clear_enc(2);
    motors.clear_enc(3);
    motors.clear_enc(4);
  }
  
  //управление четырмя моторами без регулятора - проценты ШИМа
  motors.set(150, 150, 150, 150);
  delay(10);
}