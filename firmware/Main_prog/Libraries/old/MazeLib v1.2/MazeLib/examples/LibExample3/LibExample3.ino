#include "MazeLib.h"

//m1a, m1b, m2a, m2b, m3a, m3b, m4a, m4b, номера прерываний A, B, C, D, пин энкодера A, B, C, D, пин копирования шима, кол-во об/мин мотора, коэф упр. воздействия, кол-во энкодеров за оборот
MOTORS motors(4, 5, 6, 7, 9, 8, 11, 10, 5, 4, 0, 1, 57, 56, 54, 55, 10, 170, 1, 360);

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
  //управление четырмя моторами с регулятором - градусы/сек
  motors.follow(50, 50);
  delay(10);
}
