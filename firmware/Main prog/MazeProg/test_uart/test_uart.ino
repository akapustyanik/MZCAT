#include <Wire.h>

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(2000);
}

void loop() {
  Str buf;
  if (Serial2.readBytes((byte*)&buf, sizeof(buf))) {
    state = buf.state - 1;
  }
  Serial.println(state);
  buf.state = state;
  Serial.println(state);
  Serial2.write((byte*)&buf, sizeof(buf));
  delay(10);
}
