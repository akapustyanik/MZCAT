#include "MazeLib.h"

int myTimer = 0;
bool Flag = 0;
//m1a, m1b, m2a, m2b, m3a, m3b, m4a, m4b, номера прерываний A, B, C, D, пин энкодера A, B, C, D, пин копирования шима, кол-во об/мин мотора, коэф упр. воздействия, кол-во энкодеров за оборот
MOTORS motors();

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (millis() - myTimer > 5000 && Flag == 0) {
    motors.clear_enc(1);
    motors.clear_enc(2);
    motors.clear_enc(3);
    motors.clear_enc(4);
    Serial.println("Encoders cleared by function");
    myTimer = millis();
  }
  if (millis() - myTimer > 5000 && Flag == 1) {
    Encoder[1] = 0;
    Encoder[2] = 0;
    Encoder[3] = 0;
    Encoder[4] = 0;
    Serial.println("Encoders cleared by massive");
    myTimer = millis();
  }
  
  Serial.print("Encoders by function A:");
  Serial.print(motors.encoder(1));
  Serial.print(" B:");
  Serial.print(motors.encoder(2));
  Serial.print(" C:");
  Serial.print(motors.encoder(3));
  Serial.print(" D:");
  Serial.print(motors.encoder(4));
  Serial.print(" ");
  Serial.print("Encoders by massive A:");
  Serial.print(Encoder[1]);
  Serial.print(" B:");
  Serial.print(Encoder[2]);
  Serial.print(" C:");
  Serial.print(Encoder[3]);
  Serial.print(" D:");
  Serial.println(Encoder[4]);

  if (Flag == 0) {
    //управление четырмя моторами с регулятором - градусы/сек
    motors.follow(50, 50);
  }
  else {
    //управление каждым мотором в отдельности - шим от -255 до 255
    motors.setA(100);
    motors.setB(150);
    motors.setC(200);
    motors.setD(250);
  }
  delay(10);
}
