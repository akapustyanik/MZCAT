#include "MazeLib.h"

MOTORS motors = MOTORS();

void setup() {
  Serial.begin(115200);
  //определение номеров прерываний и пины энкодеров (interrupt A, interrupt B, interrupt C, interrupt D, pin A, pin B, pin C, pin D)
  motors.attachEnc(5, 4, 0, 1, 57, 56, 54, 55);
  //определение переменных для регулятора (об/мин мотора, упр. коэф.)
  motors.setConst(110, 2);
  //изменение пинов моторов (m1a, m1b, m2a, m2b, m3a, m3b, m4a, m4b)
  motors.attachMotors(5, 4, 7, 6, 10, 11, 8, 9);
  //велючение или выключения ШИМ данной библиотеки
  motors.softwarePWM(1);
}

void loop() {
  Serial.print("A:");
  Serial.print(Encoder[0]);
  Serial.print(" B:");
  Serial.print(Encoder[1]);
  Serial.print(" C:");
  Serial.print(Encoder[2]);
  Serial.print(" D:");
  Serial.println(Encoder[3]);

  if(abs(motors.encoder(1)) == 10000) {
    motors.clear_enc(1);
    motors.clear_enc(2);
    motors.clear_enc(3);
    motors.clear_enc(4);
  }
  
  //управление четырмя моторами с регулятором - градусы/сек (110 * 6 = 660 максимальная скорость)
  motors.follow(300, 300);
  delay(10);
}