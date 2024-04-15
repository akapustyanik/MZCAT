#include "mazeLib.h"

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

#define Rx 17
#define Tx 16

#define rpm 170 //скорость в об/мин
#define d 77 //диаметр в мм

MOTOR motorA(mA_IN1, mA_IN2, A, rpm, d, 3);
MOTOR motorB(mB_IN1, mB_IN2, B, rpm, d, 3);
MOTOR motorC(mC_IN1, mC_IN2, C, rpm, d, 3);
MOTOR motorD(mD_IN1, mD_IN2, D, rpm, d, 3);

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
unsigned long mill_A = 0, mill_B = 0, mill_C = 0, mill_D = 0;
int EncMA = 0, EncMB = 0, EncMC = 0, EncMD = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
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
struct Str {
  int left;
  int front;
  int right;
  int turnLeft;
  int turnRight;
  bool button1;
  bool button_left;
  bool button_right;
  byte cameraLeft;
  byte cameraRight;
//  float x;
//  float y;
//  float z;
};
int left_wall, front_wall, right_wall, left2, right2, button_left, button_right;
float x;
float y;
float z;
bool a = true, activate = false, button1 = false;
float errP, errI;
float alpha = 0 , distance = 30.5;
byte cameraLeft, cameraRight;
int state = 0, angle = 90, help = 0, cam_state = 1;
long timer = millis(), old_timer = 0; //11 - определение жертвы 10 - 1 набор 01 - 2 набора
//while ((abs(motorA.encoder()) + abs(motorB.encoder()) + abs(motorC.encoder()) + abs(motorD.encoder())) / 4 < 360 / (diam * 3.14) * distance)
void loop() {
  Str buf;
  timer = millis();
  Serial.println(button_right);
  if (Serial2.readBytes((byte*)&buf, sizeof(buf))) {
    left_wall = buf.left;
    front_wall = buf.front;
    right_wall = buf.right;
    left2 = buf.turnLeft;
    right2 = buf.turnRight;
    button1 = buf.button1;
    button_left = buf.button_left;
    button_right = buf.button_right;
    cameraLeft = buf.cameraLeft;
    cameraRight = buf.cameraRight;
    //x = buf.x;
    //y = buf.y;
    //z = buf.z;
  }
  if (button1 == true && old_timer + 200 < timer)
  {
    if (activate)
    {
      activate = false;
    }
    else
    {
      activate = true;
    }
    old_timer = millis();
  }
  if (activate)
  {
    switch (state)
    {
      case 0 : brain(); break;
      case 1: turn_left(); break;
      case 2: turn_right(); break;
      case 3: move_forward(); break;
      case 4: back_move();
    }
  }
  delay(5);
}
