#include "MazeLib.h"

//объявление пинов и констант (m1a, m1b, m2a, m2b, m3a, m3b, m4a, m4b, interrupt A, interrupt B, interrupt C, interrupt D, pin A, pin B, pin C, pin D, об/мин мотора, упр. коэф.)
MOTORS motors = MOTORS(5, 4, 7, 6, 10, 11, 8, 9, 5, 4, 0, 1, 57, 56, 54, 55, 110, 2);

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