#include <Servo.h>

#include <config.h>

#include <iarduino_I2C_connect.h>                     // подключаем библиотеку для соединения arduino по шине I2C

#include "LSM6DS3.h"
#include "Adafruit_VL53L0X.h"
#include <Wire.h>

LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A
Servo dropper;
#define LOX1_ADDRESS 1
#define LOX2_ADDRESS 2
#define LOX3_ADDRESS 3
#define LOX4_ADDRESS 4
#define LOX5_ADDRESS 5

// set the pins to shutdown
#define SHT_LOX1 31 //front
#define SHT_LOX2 30 //left
#define SHT_LOX3 35 //right
#define SHT_LOX4 32 //left_45
#define SHT_LOX5 33 //right_45

#define Rx 15
#define Tx 14

float ax, ay, az, courseX, courseY, courseZ;
int left, front, right, turnLeft, turnRight, timer=0;
byte cameraLeft, cameraRight;
// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox5 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;
VL53L0X_RangingMeasurementData_t measure5;

iarduino_I2C_connect I2C2;

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  digitalWrite(SHT_LOX5, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }

  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX2
  if (!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot 3 VL53L0X"));
    while (1);
  }

  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  //initing LOX4
  if (!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot 4 VL53L0X"));
    while (1);
  }

  digitalWrite(SHT_LOX5, HIGH);
  delay(10);

  //initing LOX5
  if (!lox5.begin(LOX5_ADDRESS)) {
    Serial.println(F("Failed to boot 5 VL53L0X"));
    while (1);
  }
}

void read_five_sensors() {

  lox1.rangingTest(&measure1, false);
  lox2.rangingTest(&measure2, false);
  lox3.rangingTest(&measure3, false);
  lox4.rangingTest(&measure4, false);
  lox5.rangingTest(&measure5, false);
  left = measure2.RangeMilliMeter;
  turnLeft = measure4.RangeMilliMeter;
  front = measure1.RangeMilliMeter;
  turnRight = measure5.RangeMilliMeter;
  right = measure3.RangeMilliMeter;
  //    Serial.print(measure1.RangeMilliMeter);
  //    Serial.print(' ');
  //    Serial.print(measure2.RangeMilliMeter);
  //    Serial.print(' ');
  //    Serial.print(measure3.RangeMilliMeter);
  //    Serial.print(' ');
  //    Serial.print(measure4.RangeMilliMeter);
  //    Serial.print(' ');
  //    Serial.println(measure5.RangeMilliMeter);
}

void read_gyro()
{
  //
  ax = ax / 10 * 7 + ((-8.32813 - myIMU.readFloatGyroX()) * 0.022) / 10 * 3;
  ay = ay / 10 * 7 + ((-8.32813 - myIMU.readFloatGyroY()) * 0.022) / 10 * 3;
  az = az / 10 * 7 + ((-8.32813 - myIMU.readFloatGyroZ()) * 0.022) / 10 * 3;
  courseX = courseX + ax;
  courseY = courseY + ay;
  courseZ = courseZ + az;
}
void setup_sensors() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(A13, OUTPUT);
  pinMode(A14, OUTPUT);
  pinMode(A15, OUTPUT);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(A13, LOW);
  digitalWrite(A14, LOW);
  digitalWrite(A15, LOW);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);
  pinMode(SHT_LOX5, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));


  Serial.println(F("Starting..."));
  setID();
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
float startX, startY, startZ;
void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
  dropper.attach(5);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(47, INPUT);
  pinMode(48, INPUT);
  setup_sensors();
  if (myIMU.begin() != 0) {
    Serial.println("Device error");
  } else {
    Serial.println("Device OK!");
  }
  //  delay(3000);
  //  startX = myIMU.readFloatGyroX();
  //  startY = myIMU.readFloatGyroY();
  //  startZ = myIMU.readFloatGyroZ();
  dropper.write(96);
}

void loop() {
  Str buf;
  //read_gyro();
  timer++;
  read_five_sensors();
  buf.left = left;
  buf.front = front;
  buf.right = right;
  buf.turnLeft = turnLeft;
  buf.turnRight = turnRight;
  if (digitalRead(47))buf.button1 = true;
  else buf.button1 = false;
  if (digitalRead(2)) buf.button_left = 1;
  else buf.button_left = 0;
  if (digitalRead(3)) buf.button_right = 1;
  else buf.button_right = 0;
  buf.cameraLeft = (digitalRead(19) + 10 * digitalRead(18));
  buf.cameraRight = (digitalRead(17) + 10 * digitalRead(16));
  //buf.x = courseX;
  //buf.y = courseY;
  //buf.z = courseZ;
  Serial3.write((byte*)&buf, sizeof(buf));
  if (timer > 22)
  {
    if (buf.cameraLeft == 1)
    {
      dropper.write(130);
      delay(1000);
      dropper.write(98);
      delay(1000);
      dropper.write(130);
      delay(1000);
      dropper.write(98);
      delay(1000);
      timer = 0;
    }
    else  if(buf.cameraRight == 1)
    {
      dropper.write(50);
      delay(1000);
      dropper.write(110);
      delay(1000);
      dropper.write(50);
      delay(1000);
      dropper.write(110);
      delay(1000);
      timer = 0;
    }
    else if (buf.cameraLeft == 10 or buf.cameraRight == 10)
    {
      dropper.write(128);
      delay(1000);
      dropper.write(96);
      delay(1000);
      timer = 0;
    }
  }
  Serial.print(buf.cameraLeft);
  Serial.print(' ');
  Serial.println(buf.cameraRight);
  //  Serial.print(' ');
  //  Serial.print(digitalRead(17));
  //  Serial.print(' ');
  //  Serial.print(digitalRead(18));
  //  Serial.print(' ');
  //  Serial.println(digitalRead(19));
  //  S0erial.println(courseZ);
  delay(5);
}
