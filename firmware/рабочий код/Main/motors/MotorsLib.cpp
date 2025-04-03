#include "MotorsLib.h"

void _offPWM(uint8_t timer) {
  switch (timer) {
#if defined(TCCR1A) && defined(COM1A1)
    case TIMER1A:
      bitClear(TCCR1A, COM1A1);
      break;
#endif
#if defined(TCCR1A) && defined(COM1B1)
    case TIMER1B:
      bitClear(TCCR1A, COM1B1);
      break;
#endif
#if defined(TCCR1A) && defined(COM1C1)
    case TIMER1C:
      bitClear(TCCR1A, COM1C1);
      break;
#endif

#if defined(TCCR2) && defined(COM21)
    case TIMER2:
      bitClear(TCCR2, COM21);
      break;
#endif

#if defined(TCCR0A) && defined(COM0A1)
    case TIMER0A:
      bitClear(TCCR0A, COM0A1);
      break;
#endif

#if defined(TCCR0A) && defined(COM0B1)
    case TIMER0B:
      bitClear(TCCR0A, COM0B1);
      break;
#endif
#if defined(TCCR2A) && defined(COM2A1)
    case TIMER2A:
      bitClear(TCCR2A, COM2A1);
      break;
#endif
#if defined(TCCR2A) && defined(COM2B1)
    case TIMER2B:
      bitClear(TCCR2A, COM2B1);
      break;
#endif

#if defined(TCCR3A) && defined(COM3A1)
    case TIMER3A:
      bitClear(TCCR3A, COM3A1);
      break;
#endif
#if defined(TCCR3A) && defined(COM3B1)
    case TIMER3B:
      bitClear(TCCR3A, COM3B1);
      break;
#endif
#if defined(TCCR3A) && defined(COM3C1)
    case TIMER3C:
      bitClear(TCCR3A, COM3C1);
      break;
#endif

#if defined(TCCR4A) && defined(COM4A1)
    case TIMER4A:
      bitClear(TCCR4A, COM4A1);
      break;
#endif
#if defined(TCCR4A) && defined(COM4B1)
    case TIMER4B:
      bitClear(TCCR4A, COM4B1);
      break;
#endif
#if defined(TCCR4A) && defined(COM4C1)
    case TIMER4C:
      bitClear(TCCR4A, COM4C1);
      break;
#endif
#if defined(TCCR4C) && defined(COM4D1)
    case TIMER4D:
      bitClear(TCCR4C, COM4D1);
      break;
#endif

#if defined(TCCR5A)
    case TIMER5A:
      bitClear(TCCR5A, COM5A1);
      break;
    case TIMER5B:
      bitClear(TCCR5A, COM5B1);
      break;
    case TIMER5C:
      bitClear(TCCR5A, COM5C1);
      break;
#endif
  }
}

//On PWM function
void _onPWM(uint8_t timer, int val) {
  switch (timer) {
// XXX fix needed for atmega8
#if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
    case TIMER0A:
      // connect pwm to pin on timer 0
      bitSet(TCCR0, COM00);
      OCR0 = val;  // set pwm duty
      break;
#endif

#if defined(TCCR0A) && defined(COM0A1)
    case TIMER0A:
      // connect pwm to pin on timer 0, channel A
      bitSet(TCCR0A, COM0A1);
      OCR0A = val;  // set pwm duty
      break;
#endif

#if defined(TCCR0A) && defined(COM0B1)
    case TIMER0B:
      // connect pwm to pin on timer 0, channel B
      bitSet(TCCR0A, COM0B1);
      OCR0B = val;  // set pwm duty
      break;
#endif

#if defined(TCCR1A) && defined(COM1A1)
    case TIMER1A:
      // connect pwm to pin on timer 1, channel A
      bitSet(TCCR1A, COM1A1);
      OCR1A = val;  // set pwm duty
      break;
#endif

#if defined(TCCR1A) && defined(COM1B1)
    case TIMER1B:
      // connect pwm to pin on timer 1, channel B
      bitSet(TCCR1A, COM1B1);
      OCR1B = val;  // set pwm duty
      break;
#endif

#if defined(TCCR1A) && defined(COM1C1)
    case TIMER1C:
      // connect pwm to pin on timer 1, channel C
      bitSet(TCCR1A, COM1C1);
      OCR1C = val;  // set pwm duty
      break;
#endif

#if defined(TCCR2) && defined(COM21)
    case TIMER2:
      // connect pwm to pin on timer 2
      bitSet(TCCR2, COM21);
      OCR2 = val;  // set pwm duty
      break;
#endif

#if defined(TCCR2A) && defined(COM2A1)
    case TIMER2A:
      // connect pwm to pin on timer 2, channel A
      bitSet(TCCR2A, COM2A1);
      OCR2A = val;  // set pwm duty
      break;
#endif

#if defined(TCCR2A) && defined(COM2B1)
    case TIMER2B:
      // connect pwm to pin on timer 2, channel B
      bitSet(TCCR2A, COM2B1);
      OCR2B = val;  // set pwm duty
      break;
#endif

#if defined(TCCR3A) && defined(COM3A1)
    case TIMER3A:
      // connect pwm to pin on timer 3, channel A
      bitSet(TCCR3A, COM3A1);
      OCR3A = val;  // set pwm duty
      break;
#endif

#if defined(TCCR3A) && defined(COM3B1)
    case TIMER3B:
      // connect pwm to pin on timer 3, channel B
      bitSet(TCCR3A, COM3B1);
      OCR3B = val;  // set pwm duty
      break;
#endif

#if defined(TCCR3A) && defined(COM3C1)
    case TIMER3C:
      // connect pwm to pin on timer 3, channel C
      bitSet(TCCR3A, COM3C1);
      OCR3C = val;  // set pwm duty
      break;
#endif

#if defined(TCCR4A)
    case TIMER4A:
      //connect pwm to pin on timer 4, channel A
      bitSet(TCCR4A, COM4A1);
#if defined(COM4A0)  // only used on 32U4
      bitClear(TCCR4A, COM4A0);
#endif
      OCR4A = val;  // set pwm duty
      break;
#endif

#if defined(TCCR4A) && defined(COM4B1)
    case TIMER4B:
      // connect pwm to pin on timer 4, channel B
      bitSet(TCCR4A, COM4B1);
      OCR4B = val;  // set pwm duty
      break;
#endif

#if defined(TCCR4A) && defined(COM4C1)
    case TIMER4C:
      // connect pwm to pin on timer 4, channel C
      bitSet(TCCR4A, COM4C1);
      OCR4C = val;  // set pwm duty
      break;
#endif

#if defined(TCCR4C) && defined(COM4D1)
    case TIMER4D:
      // connect pwm to pin on timer 4, channel D
      bitSet(TCCR4C, COM4D1);
#if defined(COM4D0)  // only used on 32U4
      bitClear(TCCR4C, COM4D0);
#endif
      OCR4D = val;  // set pwm duty
      break;
#endif


#if defined(TCCR5A) && defined(COM5A1)
    case TIMER5A:
      // connect pwm to pin on timer 5, channel A
      bitSet(TCCR5A, COM5A1);
      OCR5A = val;  // set pwm duty
      break;
#endif

#if defined(TCCR5A) && defined(COM5B1)
    case TIMER5B:
      // connect pwm to pin on timer 5, channel B
      bitSet(TCCR5A, COM5B1);
      OCR5B = val;  // set pwm duty
      break;
#endif

#if defined(TCCR5A) && defined(COM5C1)
    case TIMER5C:
      // connect pwm to pin on timer 5, channel C
      bitSet(TCCR5A, COM5C1);
      OCR5C = val;  // set pwm duty
      break;
#endif

    default:
      break;
  }
}

void MOTORS::attach(uint8_t ma, uint8_t mb, float tpsMax, float k, float ki, bool newFunc) {
  _newFunc = newFunc;
  _tpsMax = tpsMax;
  _k = k;
  _ki = ki;

  _maPin = ma;
  _ma = digitalPinToPort(ma);
  _outMa = portOutputRegister(_ma);
  _bitMaskMa = digitalPinToBitMask(ma);
  _maTimerPWM = digitalPinToTimer(ma);

  _mbPin = mb;
  _mb = digitalPinToPort(mb);
  _outMb = portOutputRegister(_mb);
  _bitMaskMb = digitalPinToBitMask(mb);
  _mbTimerPWM = digitalPinToTimer(mb);

  pinMode(ma, OUTPUT);
  pinMode(mb, OUTPUT);
}

int32_t MOTORS::returnEnc() {
  return _motorEnc - _zeroEnc;
}

void MOTORS::set(int16_t pwm) {
  pwm = constrain(pwm, -255, 255);
  if (_newFunc) {
    if (pwm <= -255) {
      _offPWM(_maTimerPWM);
      _offPWM(_mbTimerPWM);
      *_outMb |= _bitMaskMb;   //PWM B HIGH
      *_outMa &= ~_bitMaskMa;  //PWM A LOW
    } else if (pwm >= 255) {
      _offPWM(_maTimerPWM);
      _offPWM(_mbTimerPWM);
      *_outMa |= _bitMaskMa;   //PWM A HIGH
      *_outMb &= ~_bitMaskMb;  //PWM B LOW
    } else if (pwm < 0) {
      _offPWM(_maTimerPWM);
      _onPWM(_mbTimerPWM, pwm *= -1);
      *_outMa &= ~_bitMaskMa;  //PWM A LOW
    } else if (pwm > 0) {
      _offPWM(_mbTimerPWM);
      _onPWM(_maTimerPWM, pwm);
      *_outMb &= ~_bitMaskMb;  //PWM B LOW
    } else {
      _offPWM(_maTimerPWM);
      _offPWM(_mbTimerPWM);
      *_outMa &= ~_bitMaskMa;  //PWM A LOW
      *_outMb &= ~_bitMaskMb;  //PWM B LOW
    }
  } else {
    analogWrite(_maPin, max(pwm, 0));
    analogWrite(_mbPin, sgn(pwm) * min(pwm, 0));
  }
}

void MOTORS::stop() {
  if (_newFunc) {
    _offPWM(_maTimerPWM);
    _offPWM(_mbTimerPWM);
  } else {
    digitalWrite(_maPin, 1);
    digitalWrite(_mbPin, 1);
  }
}

int32_t* MOTORS::interruptEnc() {
  return &_motorEnc;
}

void MOTORS::resetEnc() {
  _zeroEnc = _motorEnc;
}

void MOTORS::resetInEnc() {
  _zeroInEnc = _motorEnc;
}

void MOTORS::resetStaticErr() {
  _staticErr = 0;
}

void MOTORS::motorStop() {
  _mode = 0;
  stop();
}

void MOTORS::setTps(float tps) {
  if (_mode != 2) {
    resetStaticErr();
  } else {
    _staticErr += _err;
  }
  resetInEnc();
  _mode = 2;
  _I = 0;
  _tps = constrain(tps, -_tpsMax, _tpsMax);
  _myTimer = millis();
}

void MOTORS::setPwm(int16_t pwm) {
  _mode = 1;
  if (pwm != 0) {
    set(pwm);
  } else {
    stop();
  }
}

void MOTORS::regTps(int32_t myTime) {
  if (_mode == 2) {
    _err = _tps * (millis() - _myTimer) / 1000.0 - (_motorEnc - _zeroInEnc);
    _motorPwm = float(_err + _staticErr) * _k + float(_I) * _ki;
    _I += (_err + _staticErr) * (abs(_motorPwm) < 256) * myTime;
    set(_motorPwm);
  } else {
    stop();
  }
}
