#include <Servo.h>
Servo myServo;
#define ASCII_CONVERT '0'

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  myServo.attach(5);
  myServo.write(45);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial2.available()) {
    int in = Serial2.read() - int(ASCII_CONVERT);
    Serial.println(in);
  }
}
