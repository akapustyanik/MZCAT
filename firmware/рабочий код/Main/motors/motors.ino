#include "MotorsLib.h"

#define kMotor 2
#define kiMotor 0.01
#define newFuncMotor 0
#define tpsMaxMotor 1000

uint8_t motorPins[] = { 8, 9, 5, 4, 10, 11, 7, 6 };

MOTORS motor[4];

int32_t *motorEncs[4];

const uint8_t interrupts[] = { 1, 5, 0, 4 };

const uint8_t reverses[] = { 1, 0, 1, 0 };

const uint8_t directionPins[] = { A1, A3, A0, A2 };

void interr0() {
  *motorEncs[0] = *motorEncs[0] - (reverses[0] * 2 - 1) * (digitalRead(directionPins[0]) * 2 - 1);
}

void interr1() {
  *motorEncs[1] = *motorEncs[1] - (reverses[1] * 2 - 1) * (digitalRead(directionPins[1]) * 2 - 1);
}

void interr2() {
  *motorEncs[2] = *motorEncs[2] - (reverses[2] * 2 - 1) * (digitalRead(directionPins[2]) * 2 - 1);
}

void interr3() {
  *motorEncs[3] = *motorEncs[3] - (reverses[3] * 2 - 1) * (digitalRead(directionPins[3]) * 2 - 1);
}

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <EEPROM.h>

MPU6050 mpu;

struct DataToEEPROM {
  int XAccelOffset;
  int YAccelOffset;
  int ZAccelOffset;
  int XGyroOffset;
  int YGyroOffset;
  int ZGyroOffset;
};

struct DataToSerial {
  float ypr[3];
  int32_t enc[4];
  byte crc;
};

struct DataFromSerial {
  bool onlyPwm[4];
  int16_t vMotor[4];
};

struct Buffer {
  bool onlyPwm[4];
  int16_t vMotor[4];
  byte crc;
};

Buffer buf;

bool errSerial = 0;
uint8_t fifoBuffer[64];  // буфер
float ypr[3], myYpr[3], oldYpr[3];
uint32_t myTimer;
const int16_t sleep = 4;
int32_t corr = 0;

Quaternion q;
VectorFloat gravity;

DataToEEPROM dataEEPROM;
DataToSerial toSerial;
DataFromSerial fromSerial;

void setup() {
  motor[0].attach(motorPins[0], motorPins[1], tpsMaxMotor, kMotor, kiMotor, newFuncMotor);
  motor[1].attach(motorPins[2], motorPins[3], tpsMaxMotor, kMotor, kiMotor, newFuncMotor);
  motor[2].attach(motorPins[4], motorPins[5], tpsMaxMotor, kMotor, kiMotor, newFuncMotor);
  motor[3].attach(motorPins[6], motorPins[7], tpsMaxMotor, kMotor, kiMotor, newFuncMotor);
  pinMode(directionPins[0], 0);
  pinMode(directionPins[0], 0);
  pinMode(directionPins[0], 0);
  pinMode(directionPins[0], 0);
  Serial.begin(115200);
  Serial2.begin(115200 * 3);
  motorEncs[0] = motor[0].interruptEnc();
  motorEncs[1] = motor[1].interruptEnc();
  motorEncs[2] = motor[2].interruptEnc();
  motorEncs[3] = motor[3].interruptEnc();
  attachInterrupt(interrupts[0], interr0, RISING);
  attachInterrupt(interrupts[1], interr1, RISING);
  attachInterrupt(interrupts[2], interr2, RISING);
  attachInterrupt(interrupts[3], interr3, RISING);

  Wire.begin();
  Wire.setClock(800000UL);   // разгоняем шину на максимум

  // инициализация DMP
  mpu.initialize();

  mpu.dmpInitialize();

  EEPROM.get(0, dataEEPROM);

  mpu.setXAccelOffset(dataEEPROM.XAccelOffset);
  mpu.setYAccelOffset(dataEEPROM.YAccelOffset);
  mpu.setZAccelOffset(dataEEPROM.ZAccelOffset);
  mpu.setXGyroOffset(dataEEPROM.XGyroOffset);
  mpu.setYGyroOffset(dataEEPROM.YGyroOffset);
  mpu.setZGyroOffset(dataEEPROM.ZGyroOffset);

  mpu.setDMPEnabled(true);

  myTimer = millis() + 2000;
}

void loop() {
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    // расчёты
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    for (int i = 0; i < 3; i++) {
      if (abs(degrees(ypr[i] - oldYpr[i])) > 180 and i == 0) {
        corr += 360 * sgn(oldYpr[i] - ypr[i]);
      }
      toSerial.ypr[i] = degrees(ypr[i]) + float(corr) * (i == 0);
      oldYpr[i] = ypr[i];
      Serial.print(toSerial.ypr[i]);
      Serial.print(" ");
    }
    Serial.println();
    // для градусов можно использовать degrees()
  }
  for (int i = 0; i < 4; i++) {
    toSerial.enc[i] = motor[i].returnEnc();
  }
  if (millis() > sleep + myTimer) {
    writeSerial();
    myTimer = millis();
  }

  if (Serial2.available()) {
    if (!readSerial()) Serial.println("err with Serial2");
    for (int i = 0; i < 4; i++) {
      if (fromSerial.onlyPwm[i]) {
        motor[i].setPwm(fromSerial.vMotor[i]);
      } else {
        motor[i].setTps(fromSerial.vMotor[i]);
      }
      //Serial.print(fromSerial.vMotor[i]);
      //Serial.print(" ");
    }
    //Serial.println();
  }
  for (int i = 0; i < 4; i++) {
    if (!fromSerial.onlyPwm[i]) {
      motor[i].regTps();
    }
  }
}

bool readSerial() {
  static byte crc;
  if (errSerial) {
    Serial2.read();
    errSerial = 0;
  }
  Serial2.readBytes((byte *)&buf, sizeof(buf));
  crc = crc8_bytes((byte *)&buf, sizeof(buf));
  if (crc == 0) {
    for (int i = 0; i < 4; i++) {
      fromSerial.onlyPwm[i] = buf.onlyPwm[i];
      fromSerial.vMotor[i] = buf.vMotor[i];
    }
    return 1;
  }
  errSerial = 1;
  return 0;
}

void writeSerial() {
  toSerial.crc = crc8_bytes((byte *)&toSerial, sizeof(toSerial) - 1);
  Serial2.write((byte *)&toSerial, sizeof(toSerial));
}

byte crc8_bytes(byte *buffer, byte size) {
  byte crc = 0;
  for (byte i = 0; i < size; i++) {
    byte data = buffer[i];
    for (int j = 8; j > 0; j--) {
      crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
      data >>= 1;
    }
  }
  return crc;
}