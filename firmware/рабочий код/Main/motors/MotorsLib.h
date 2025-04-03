#pragma once

#include <Arduino.h>

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

void _onPWM(uint8_t timer, int val);
void _offPWM(uint8_t timer);

class MOTORS {
public:
  void attach(uint8_t ma, uint8_t mb, float tpsMax, float k, float ki, bool newFunc);
  void motorStop();
  int32_t returnEnc();
  void resetEnc();
  void setPwm(int16_t pwm);
  void setTps(float tps);
  void regTps(int32_t myTime = 1);
  int32_t* interruptEnc();
private:
  void resetInEnc();
  void set(int16_t pwm);
  void stop();
  void resetStaticErr();
  uint8_t _ma, _mb, _maPin, _mbPin, _interrupt, _directionPin, _bitMaskMa, _bitMaskMb, _maTimerPWM, _mbTimerPWM, *_outMa, *_outMb;
  float _tpsMax, _tps, _k, _ki;
  int32_t _motorEnc = 0, _zeroInEnc = 0, _zeroEnc = 0, _staticErr = 0, _err;
  uint32_t _myTimer;
  uint8_t _mode = 0;
  bool _newFunc;
  int16_t _motorPwm;
  double _I = 0;
};
