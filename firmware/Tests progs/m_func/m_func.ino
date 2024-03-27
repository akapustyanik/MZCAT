#include "mazeLib.h"

unsigned long mill_A = 0, mill_B = 0, mill_C = 0, mill_D = 0;
int EncMA = 0, EncMB = 0, EncMC = 0, EncMD = 0;

#define mA_IN1 4
#define mA_IN2 5
#define mB_IN1 6
#define mB_IN2 7
#define mC_IN1 8
#define mC_IN2 9
#define mD_IN1 11
#define mD_IN2 10

#define EncPin_A 18
#define EncPin_B 19
#define EncPin_C 2
#define EncPin_D 3

#define rpm 170 //скорость в об/мин
#define d 77 //диаметр в мм

MOTOR motorA(mA_IN1, mA_IN2, A, rpm, d, 3);
MOTOR motorB(mB_IN1, mB_IN2, B, rpm, d, 3);
MOTOR motorC(mC_IN1, mC_IN2, C, rpm, d, 3);
MOTOR motorD(mD_IN1, mD_IN2, D, rpm, d, 3);

//изменение энкодеров
void enc_A() {
  motorA.writeEnc();
}
void enc_B() {
  motorB.writeEnc();
}
void enc_C() {
  motorC.writeEnc();
}
void enc_D() {
  motorD.writeEnc();
}

void clearAllEnc() {
  motorA.clear_enc();
  motorB.clear_enc();
  motorC.clear_enc();
  motorD.clear_enc();
}

void setup() {
  Serial.begin(115200);
  attachInterrupt(5, enc_A, RISING);
  attachInterrupt(4, enc_B, RISING);
  attachInterrupt(0, enc_C, RISING);
  attachInterrupt(1, enc_D, RISING);
  pinMode(EncPin_A, INPUT); // Инициализируем пин Encoder_output_A на вход
  pinMode(EncPin_B, INPUT); // Инициализируем пин Encoder_output_B на вход
  pinMode(EncPin_C, INPUT); // Инициализируем пин Encoder_output_C на вход
  pinMode(EncPin_D, INPUT); // Инициализируем пин Encoder_output_D на вход
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //motorA.justRun(255);
  //motorA.justSpeed(40);
  motorA.followSpeed(50);
  motorB.followSpeed(50);
  motorC.followSpeed(50);
  motorD.followSpeed(50);
  Serial.println(motorA.encoder());
  //Serial.println();
  delay(10);
}
