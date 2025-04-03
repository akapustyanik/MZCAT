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

uint8_t fifoBuffer[64];  // буфер
float ypr[3];
uint32_t myTimer;

DataToEEPROM data;

void setup() {

  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(800000UL);   // разгоняем шину на максимум
  // инициализация DMP

  delay(3000);
  mpu.initialize();

  mpu.dmpInitialize();

  delay(3000);

  for (int i = 1; i <= 3; i++) {
    mpu.CalibrateAccel(100);
    mpu.CalibrateGyro(100);

    mpu.PrintActiveOffsets();

    data.XAccelOffset = float(mpu.getXAccelOffset()) * 1.0 / float(i) + float(data.XAccelOffset) * (1.0 - 1.0 / float(i));
    data.YAccelOffset = float(mpu.getYAccelOffset()) * 1.0 / float(i) + float(data.YAccelOffset) * (1.0 - 1.0 / float(i));
    data.ZAccelOffset = float(mpu.getZAccelOffset()) * 1.0 / float(i) + float(data.ZAccelOffset) * (1.0 - 1.0 / float(i));
    data.XGyroOffset = float(mpu.getXGyroOffset()) * 1.0 / float(i) + float(data.XGyroOffset) * (1.0 - 1.0 / float(i));
    data.YGyroOffset = float(mpu.getYGyroOffset()) * 1.0 / float(i) + float(data.YGyroOffset) * (1.0 - 1.0 / float(i));
    data.ZGyroOffset = float(mpu.getZGyroOffset()) * 1.0 / float(i) + float(data.ZGyroOffset) * (1.0 - 1.0 / float(i));
  }



  EEPROM.put(0, data);

  //EEPROM.get(0, data);

  // mpu.setXAccelOffset(data.XAccelOffset);
  // mpu.setYAccelOffset(data.YAccelOffset);
  // mpu.setZAccelOffset(data.ZAccelOffset);
  // mpu.setXGyroOffset(data.XGyroOffset);
  // mpu.setYGyroOffset(data.YGyroOffset);
  // mpu.setZGyroOffset(data.ZGyroOffset);

  mpu.setDMPEnabled(true);
}
void loop() {
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    // переменные для расчёта (ypr можно вынести в глобал)
    Quaternion q;
    VectorFloat gravity;
    // расчёты
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    // выводим результат в радианах (-3.14, 3.14)
    Serial.print(degrees(ypr[0]));  // вокруг оси Z
    Serial.print(',');
    Serial.print(degrees(ypr[1]));  // вокруг оси Y
    Serial.print(',');
    Serial.print(degrees(ypr[2]));  // вокруг оси X
    Serial.println();
    // для градусов можно использовать degrees()
    myTimer = millis();
  }
}
