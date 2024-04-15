#include "MazeLib.h"

MOTORS motors();

void setup() {
  Serial.begin(115200);
  //определение номеров прерываний и пины энкодеров (interrupt A, interrupt B, interrupt C, interrupt D, pin A, pin B, pin C, pin D)
  motors.attachEnc(5, 4, 0, 1, 57, 56, 54, 55);
  //определение переменных для регулятора (об/мин мотора, упр. коэф., кол-во энкодеров за оборот)
  motors.setConst(170, 1, 360);
  //изменение хардварного таймера для управления ШИМ-ом
  motors.comparePinPWM(10);
  //изменение пинов моторов (m1a, m1b, m2a, m2b, m3a, m3b, m4a, m4b)
  motors.attachMotors(4, 5, 6, 7, 9, 8, 11, 10);
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
  //управление четырмя моторами с регулятором - градусы/сек
  motors.follow(50, 50);
  delay(10);
}
