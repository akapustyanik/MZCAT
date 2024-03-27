#include <config.h>

#include <iarduino_I2C_connect.h>                     // подключаем библиотеку для соединения arduino по шине I2C

#include "LSM6DS3.h"
#include "Adafruit_VL53L0X.h"
#include <Wire.h>

LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

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
struct Str {
  int left;
  int front;
  int right;
  int button1;
  float x;
  float y;
  float z;
};
float startX, startY, startZ;
void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
  pinMode(47, INPUT);
  setup_sensors();
  if (myIMU.begin() != 0) {
    Serial.println("Device error");
  } else {
    Serial.println("Device OK!");
  }
  delay(3000);
  startX = myIMU.readFloatGyroX();
  startY = myIMU.readFloatGyroY();
  startZ = myIMU.readFloatGyroZ();
}
float ax, ay, az;
float courseX;
float courseY;
float courseZ;
int left_wall;
int front_wall;
int right_wall;

void loop() {
  Str buf;
  read_gyro();
  read_five_sensors();
  buf.left = left_wall;
  buf.front = front_wall;
  buf.right = right_wall;
  buf.button1 = digitalRead(47);
  buf.x = courseX;
  buf.y = courseY;
  buf.z = courseZ;
  Serial3.write((byte*)&buf, sizeof(buf));
  Serial.println(myIMU.readFloatGyroZ());
  //  Serial.print(digitalRead(16));
  //  Serial.print(' ');
  //  Serial.print(digitalRead(17));
  //  Serial.print(' ');
  //  Serial.print(digitalRead(18));
  //  Serial.print(' ');
  //  Serial.println(digitalRead(19));
  //  Serial.println(courseZ);
  delay(1);
}
