#include <MazeLib.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

MOTORS motors = MOTORS();

Adafruit_MPU6050 mpu;

struct Str {
  int state;
  float u;
};

float staticErr;
int32_t Oldmillis;
int stateOld;
int encForward = 3000;
int encTurn = 2000;
int encBack = 1000;
float v = 300;
int state = 0;
int u = 0;
float angleOld = 0;
float angle = 0;
float _err_measure = 0.8;  // примерный шум измерений
float _q = 0.1;   // скорость изменения значений 0.001-1, варьировать самому

float simpleKalman(float newVal) {
  float _kalman_gain, _current_estimate;
  static float _err_estimate = _err_measure;
  static float _last_estimate;
  _kalman_gain = (float)_err_estimate / (_err_estimate + _err_measure);
  _current_estimate = _last_estimate + (float)_kalman_gain * (newVal - _last_estimate);
  _err_estimate =  (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
  _last_estimate = _current_estimate;
  return _current_estimate;
}

void clearAllEnc() {
  motors.clear_enc(1);
  motors.clear_enc(2);
  motors.clear_enc(3);
  motors.clear_enc(4);
}

void setupGyro()
{
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }
  Serial.println("");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
  motors.softwarePWM(0);
  delay(2000);
  setupGyro();
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  staticErr = g.gyro.z;
}

void loop() {
  Str buf;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  angle = float(angle) + float(millis() - Oldmillis) * simpleKalman(g.gyro.z - staticErr) * 57.296 / 1000.0;
  Serial.println(angle);
  if (Serial2.readBytes((byte*)&buf, sizeof(buf))) {
    u = buf.u;
    state = buf.state;
  }
  Serial.println(motors.encoder(1));
  if (state != stateOld) {
    stateOld = state;
    clearAllEnc();
    angleOld = g.gyro.z;
  }

  switch (state)
  {
    case 1:
      //forward
      motors.follow(v + u, v - u);
      Serial.println("forward");
      if (abs(motors.encoder(1)) >= encForward) {
        state = 0;
        motors.follow(0, 0);
      }
      break;
    case 2:
      //right
      motors.follow(v, -v);
      Serial.println("right");
      if (abs(angle) >=  abs(angleOld) + 90) {
        state = 0;
        motors.follow(0, 0);
      }
      break;
    case 3:
      //left
      motors.follow(-v, v);
      Serial.println("left");
      if (abs(angle) >=  abs(angleOld) + 90) {
        state = 0;
        motors.follow(0, 0);
      }
      break;
    case 4:
      //back
      motors.follow(-v, -v);
      //      delay(2000);
      //      motors.follow(v, v);
      Serial.println("back");
      if (abs(motors.encoder(1)) >= encBack) {
        state = 0;
        motors.follow(0, 0);
      }
      break;
    default:
      //stay
      motors.follow(0, 0);
      Serial.println("stay");
      break;
  }

  buf.state = state;
  Serial2.write((byte*)&buf, sizeof(buf));
  Oldmillis = millis();
  delay(10);
}
