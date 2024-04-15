#include <MazeLib.h>

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

volatile uint8_t *_directionInRegA;
volatile uint8_t *_directionInRegB;
volatile uint8_t *_directionInRegC;
volatile uint8_t *_directionInRegD;

uint8_t _directionBitMaskA;
uint8_t _directionBitMaskB;
uint8_t _directionBitMaskC;
uint8_t _directionBitMaskD;

volatile int16_t Encoder[4];
volatile int16_t EncoderOld[4];

void _encoderA()
{
  if (*_directionInRegA & _directionBitMaskA)
    Encoder[0]--;
  else
    Encoder[0]++;
}
void _encoderB()
{
  if (*_directionInRegB & _directionBitMaskB)
    Encoder[1]--;
  else
    Encoder[1]++;
}
void _encoderC()
{
  if (*_directionInRegC & _directionBitMaskC)
    Encoder[2]--;
  else
    Encoder[2]++;
}
void _encoderD()
{
  if (*_directionInRegD & _directionBitMaskD)
    Encoder[3]--;
  else
    Encoder[3]++;
}

MOTORS::MOTORS() {
  _APS_max = 1020.0;
  _rotate_enc = 360.0;
  _k = 1.0;

  _portPWM = digitalPinToPort(10);
  _outRegPWM = portOutputRegister(_portPWM);
  _bitMaskPWM = digitalPinToBitMask(10);
  _timerPWM = digitalPinToTimer(10);

  _m1a = digitalPinToPort(4);
  _outM1a = portOutputRegister(_m1a);
  _bitMaskM1a = digitalPinToBitMask(4);

  _m1b = digitalPinToPort(5);
  _outM1b = portOutputRegister(_m1b);
  _bitMaskM1b = digitalPinToBitMask(5);

  _m2a = digitalPinToPort(6);
  _outM2a = portOutputRegister(_m2a);
  _bitMaskM2a = digitalPinToBitMask(6);

  _m2b = digitalPinToPort(7);
  _outM2b = portOutputRegister(_m2b);
  _bitMaskM2b = digitalPinToBitMask(7);

  _m3a = digitalPinToPort(9);
  _outM3a = portOutputRegister(_m3a);
  _bitMaskM3a = digitalPinToBitMask(9);

  _m3b = digitalPinToPort(8);
  _outM3b = portOutputRegister(_m3b);
  _bitMaskM3b = digitalPinToBitMask(8);

  _m4a = digitalPinToPort(11);
  _outM4a = portOutputRegister(_m4a);
  _bitMaskM4a = digitalPinToBitMask(11);

  _m4b = digitalPinToPort(10);
  _outM4b = portOutputRegister(_m4b);
  _bitMaskM4b = digitalPinToBitMask(10);

  pinMode(10, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(57, INPUT);
  pinMode(56, INPUT);
  pinMode(54, INPUT);
  pinMode(55, INPUT);

  attachInterrupt(5, _encoderA, FALLING);
  _directionInRegA = portInputRegister(digitalPinToPort(57));
  _directionBitMaskA = digitalPinToBitMask(57);

  attachInterrupt(4, _encoderB, FALLING);
  _directionInRegB = portInputRegister(digitalPinToPort(56));
  _directionBitMaskB = digitalPinToBitMask(56);

  attachInterrupt(0, _encoderC, FALLING);
  _directionInRegC = portInputRegister(digitalPinToPort(54));
  _directionBitMaskC = digitalPinToBitMask(54);

  attachInterrupt(1, _encoderD, FALLING);
  _directionInRegD = portInputRegister(digitalPinToPort(55));
  _directionBitMaskD = digitalPinToBitMask(55);
}

void MOTORS::attachMotors(uint8_t m1a, uint8_t m1b, uint8_t m2a, uint8_t m2b, uint8_t m3a, uint8_t m3b, uint8_t m4a, uint8_t m4b) {
  _m1a = digitalPinToPort(m1a);
  _outM1a = portOutputRegister(_m1a);
  _bitMaskM1a = digitalPinToBitMask(m1a);

  _m1b = digitalPinToPort(m1b);
  _outM1b = portOutputRegister(_m1b);
  _bitMaskM1b = digitalPinToBitMask(m1b);

  _m2a = digitalPinToPort(m2a);
  _outM2a = portOutputRegister(_m2a);
  _bitMaskM2a = digitalPinToBitMask(m2a);

  _m2b = digitalPinToPort(m2b);
  _outM2b = portOutputRegister(_m2b);
  _bitMaskM2b = digitalPinToBitMask(m2b);

  _m3a = digitalPinToPort(m3a);
  _outM3a = portOutputRegister(_m3a);
  _bitMaskM3a = digitalPinToBitMask(m3a);

  _m3b = digitalPinToPort(m3b);
  _outM3b = portOutputRegister(_m3b);
  _bitMaskM3b = digitalPinToBitMask(m3b);

  _m4a = digitalPinToPort(m4a);
  _outM4a = portOutputRegister(_m4a);
  _bitMaskM4a = digitalPinToBitMask(m4a);

  _m4b = digitalPinToPort(m4b);
  _outM4b = portOutputRegister(_m4b);
  _bitMaskM4b = digitalPinToBitMask(m4b);

  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);
  pinMode(m3a, OUTPUT);
  pinMode(m3b, OUTPUT);
  pinMode(m4a, OUTPUT);
  pinMode(m4b, OUTPUT);
}

void MOTORS::comparePinPWM(uint8_t pinPWM) {
  _portPWM = digitalPinToPort(pinPWM);
  _outRegPWM = portOutputRegister(_portPWM);
  _bitMaskPWM = digitalPinToBitMask(pinPWM);
  _timerPWM = digitalPinToTimer(pinPWM);

  pinMode(pinPWM, OUTPUT);
}

void MOTORS::setConst(float RPM, float k, int16_t rotate_enc) {
  _APS_max = RPM / 60.0 * 360.0;
  _rotate_enc = rotate_enc;
  _k = k;
}

void MOTORS::attachEnc(uint8_t interruptA, uint8_t interruptB, uint8_t interruptC, uint8_t interruptD, uint8_t directionPinA, uint8_t directionPinB, uint8_t directionPinC, uint8_t directionPinD) {
  pinMode(interruptA, INPUT);
  pinMode(interruptB, INPUT);
  pinMode(interruptC, INPUT);
  pinMode(interruptD, INPUT);
  pinMode(directionPinA, INPUT);
  pinMode(directionPinB, INPUT);
  pinMode(directionPinC, INPUT);
  pinMode(directionPinD, INPUT);
  
  attachInterrupt(interruptA, _encoderA, FALLING);
  _directionInRegA = portInputRegister(digitalPinToPort(directionPinA));
  _directionBitMaskA = digitalPinToBitMask(directionPinA);

  attachInterrupt(interruptB, _encoderB, FALLING);
  _directionInRegB = portInputRegister(digitalPinToPort(directionPinB));
  _directionBitMaskB = digitalPinToBitMask(directionPinB);

  attachInterrupt(interruptC, _encoderC, FALLING);
  _directionInRegC = portInputRegister(digitalPinToPort(directionPinC));
  _directionBitMaskC = digitalPinToBitMask(directionPinC);

  attachInterrupt(interruptD, _encoderD, FALLING);
  _directionInRegD = portInputRegister(digitalPinToPort(directionPinD));
  _directionBitMaskD = digitalPinToBitMask(directionPinD);
}

MOTORS::MOTORS(uint8_t m1a, uint8_t m1b, uint8_t m2a, uint8_t m2b, uint8_t m3a, uint8_t m3b, uint8_t m4a, uint8_t m4b, uint8_t interruptA, uint8_t interruptB, uint8_t interruptC, uint8_t interruptD, uint8_t directionPinA, uint8_t directionPinB, uint8_t directionPinC, uint8_t directionPinD, uint8_t pinPWM, float RPM, float k, int16_t rotate_enc) {
  _APS_max = RPM / 60.0 * 360.0;
  _rotate_enc = rotate_enc;
  _k = k;

  _portPWM = digitalPinToPort(pinPWM);
  _outRegPWM = portOutputRegister(_portPWM);
  _bitMaskPWM = digitalPinToBitMask(pinPWM);
  _timerPWM = digitalPinToTimer(pinPWM);

  _m1a = digitalPinToPort(m1a);
  _outM1a = portOutputRegister(_m1a);
  _bitMaskM1a = digitalPinToBitMask(m1a);

  _m1b = digitalPinToPort(m1b);
  _outM1b = portOutputRegister(_m1b);
  _bitMaskM1b = digitalPinToBitMask(m1b);

  _m2a = digitalPinToPort(m2a);
  _outM2a = portOutputRegister(_m2a);
  _bitMaskM2a = digitalPinToBitMask(m2a);

  _m2b = digitalPinToPort(m2b);
  _outM2b = portOutputRegister(_m2b);
  _bitMaskM2b = digitalPinToBitMask(m2b);

  _m3a = digitalPinToPort(m3a);
  _outM3a = portOutputRegister(_m3a);
  _bitMaskM3a = digitalPinToBitMask(m3a);

  _m3b = digitalPinToPort(m3b);
  _outM3b = portOutputRegister(_m3b);
  _bitMaskM3b = digitalPinToBitMask(m3b);

  _m4a = digitalPinToPort(m4a);
  _outM4a = portOutputRegister(_m4a);
  _bitMaskM4a = digitalPinToBitMask(m4a);

  _m4b = digitalPinToPort(m4b);
  _outM4b = portOutputRegister(_m4b);
  _bitMaskM4b = digitalPinToBitMask(m4b);

  pinMode(pinPWM, OUTPUT);
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);
  pinMode(m3a, OUTPUT);
  pinMode(m3b, OUTPUT);
  pinMode(m4a, OUTPUT);
  pinMode(m4b, OUTPUT);
  pinMode(interruptA, INPUT);
  pinMode(interruptB, INPUT);
  pinMode(interruptC, INPUT);
  pinMode(interruptD, INPUT);
  pinMode(directionPinA, INPUT);
  pinMode(directionPinB, INPUT);
  pinMode(directionPinC, INPUT);
  pinMode(directionPinD, INPUT);

  attachInterrupt(interruptA, _encoderA, FALLING);
  _directionInRegA = portInputRegister(digitalPinToPort(directionPinA));
  _directionBitMaskA = digitalPinToBitMask(directionPinA);

  attachInterrupt(interruptB, _encoderB, FALLING);
  _directionInRegB = portInputRegister(digitalPinToPort(directionPinB));
  _directionBitMaskB = digitalPinToBitMask(directionPinB);

  attachInterrupt(interruptC, _encoderC, FALLING);
  _directionInRegC = portInputRegister(digitalPinToPort(directionPinC));
  _directionBitMaskC = digitalPinToBitMask(directionPinC);

  attachInterrupt(interruptD, _encoderD, FALLING);
  _directionInRegD = portInputRegister(digitalPinToPort(directionPinD));
  _directionBitMaskD = digitalPinToBitMask(directionPinD);
}

void _offPWM(uint8_t timer)
{
  switch (timer)
  {
#if defined(TCCR1A) && defined(COM1A1)
    case TIMER1A:   bitClear(TCCR1A, COM1A1);    break;
#endif
#if defined(TCCR1A) && defined(COM1B1)
    case TIMER1B:   bitClear(TCCR1A, COM1B1);    break;
#endif
#if defined(TCCR1A) && defined(COM1C1)
    case TIMER1C:   bitClear(TCCR1A, COM1C1);    break;
#endif

#if defined(TCCR2) && defined(COM21)
    case  TIMER2:   bitClear(TCCR2, COM21);      break;
#endif

#if defined(TCCR0A) && defined(COM0A1)
    case  TIMER0A:  bitClear(TCCR0A, COM0A1);    break;
#endif

#if defined(TCCR0A) && defined(COM0B1)
    case  TIMER0B:  bitClear(TCCR0A, COM0B1);    break;
#endif
#if defined(TCCR2A) && defined(COM2A1)
    case  TIMER2A:  bitClear(TCCR2A, COM2A1);    break;
#endif
#if defined(TCCR2A) && defined(COM2B1)
    case  TIMER2B:  bitClear(TCCR2A, COM2B1);    break;
#endif

#if defined(TCCR3A) && defined(COM3A1)
    case  TIMER3A:  bitClear(TCCR3A, COM3A1);    break;
#endif
#if defined(TCCR3A) && defined(COM3B1)
    case  TIMER3B:  bitClear(TCCR3A, COM3B1);    break;
#endif
#if defined(TCCR3A) && defined(COM3C1)
    case  TIMER3C:  bitClear(TCCR3A, COM3C1);    break;
#endif

#if defined(TCCR4A) && defined(COM4A1)
    case  TIMER4A:  bitClear(TCCR4A, COM4A1);    break;
#endif
#if defined(TCCR4A) && defined(COM4B1)
    case  TIMER4B:  bitClear(TCCR4A, COM4B1);    break;
#endif
#if defined(TCCR4A) && defined(COM4C1)
    case  TIMER4C:  bitClear(TCCR4A, COM4C1);    break;
#endif
#if defined(TCCR4C) && defined(COM4D1)
    case TIMER4D: bitClear(TCCR4C, COM4D1); break;
#endif

#if defined(TCCR5A)
    case  TIMER5A:  bitClear(TCCR5A, COM5A1);    break;
    case  TIMER5B:  bitClear(TCCR5A, COM5B1);    break;
    case  TIMER5C:  bitClear(TCCR5A, COM5C1);    break;
#endif
  }
}

void _onPWM(uint8_t timer, int val)
{
  switch (timer)
  {
      // XXX fix needed for atmega8
#if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
    case TIMER0A:
      // connect pwm to pin on timer 0
      bitSet(TCCR0, COM00);
      OCR0 = val; // set pwm duty
      break;
#endif

#if defined(TCCR0A) && defined(COM0A1)
    case TIMER0A:
      // connect pwm to pin on timer 0, channel A
      bitSet(TCCR0A, COM0A1);
      OCR0A = val; // set pwm duty
      break;
#endif

#if defined(TCCR0A) && defined(COM0B1)
    case TIMER0B:
      // connect pwm to pin on timer 0, channel B
      bitSet(TCCR0A, COM0B1);
      OCR0B = val; // set pwm duty
      break;
#endif

#if defined(TCCR1A) && defined(COM1A1)
    case TIMER1A:
      // connect pwm to pin on timer 1, channel A
      bitSet(TCCR1A, COM1A1);
      OCR1A = val; // set pwm duty
      break;
#endif

#if defined(TCCR1A) && defined(COM1B1)
    case TIMER1B:
      // connect pwm to pin on timer 1, channel B
      bitSet(TCCR1A, COM1B1);
      OCR1B = val; // set pwm duty
      break;
#endif

#if defined(TCCR1A) && defined(COM1C1)
    case TIMER1C:
      // connect pwm to pin on timer 1, channel C
      bitSet(TCCR1A, COM1C1);
      OCR1C = val; // set pwm duty
      break;
#endif

#if defined(TCCR2) && defined(COM21)
    case TIMER2:
      // connect pwm to pin on timer 2
      bitSet(TCCR2, COM21);
      OCR2 = val; // set pwm duty
      break;
#endif

#if defined(TCCR2A) && defined(COM2A1)
    case TIMER2A:
      // connect pwm to pin on timer 2, channel A
      bitSet(TCCR2A, COM2A1);
      OCR2A = val; // set pwm duty
      break;
#endif

#if defined(TCCR2A) && defined(COM2B1)
    case TIMER2B:
      // connect pwm to pin on timer 2, channel B
      bitSet(TCCR2A, COM2B1);
      OCR2B = val; // set pwm duty
      break;
#endif

#if defined(TCCR3A) && defined(COM3A1)
    case TIMER3A:
      // connect pwm to pin on timer 3, channel A
      bitSet(TCCR3A, COM3A1);
      OCR3A = val; // set pwm duty
      break;
#endif

#if defined(TCCR3A) && defined(COM3B1)
    case TIMER3B:
      // connect pwm to pin on timer 3, channel B
      bitSet(TCCR3A, COM3B1);
      OCR3B = val; // set pwm duty
      break;
#endif

#if defined(TCCR3A) && defined(COM3C1)
    case TIMER3C:
      // connect pwm to pin on timer 3, channel C
      bitSet(TCCR3A, COM3C1);
      OCR3C = val; // set pwm duty
      break;
#endif

#if defined(TCCR4A)
    case TIMER4A:
      //connect pwm to pin on timer 4, channel A
      bitSet(TCCR4A, COM4A1);
#if defined(COM4A0)   // only used on 32U4
      bitClear(TCCR4A, COM4A0);
#endif
      OCR4A = val;  // set pwm duty
      break;
#endif

#if defined(TCCR4A) && defined(COM4B1)
    case TIMER4B:
      // connect pwm to pin on timer 4, channel B
      bitSet(TCCR4A, COM4B1);
      OCR4B = val; // set pwm duty
      break;
#endif

#if defined(TCCR4A) && defined(COM4C1)
    case TIMER4C:
      // connect pwm to pin on timer 4, channel C
      bitSet(TCCR4A, COM4C1);
      OCR4C = val; // set pwm duty
      break;
#endif

#if defined(TCCR4C) && defined(COM4D1)
    case TIMER4D:
      // connect pwm to pin on timer 4, channel D
      bitSet(TCCR4C, COM4D1);
#if defined(COM4D0)   // only used on 32U4
      bitClear(TCCR4C, COM4D0);
#endif
      OCR4D = val;  // set pwm duty
      break;
#endif


#if defined(TCCR5A) && defined(COM5A1)
    case TIMER5A:
      // connect pwm to pin on timer 5, channel A
      bitSet(TCCR5A, COM5A1);
      OCR5A = val; // set pwm duty
      break;
#endif

#if defined(TCCR5A) && defined(COM5B1)
    case TIMER5B:
      // connect pwm to pin on timer 5, channel B
      bitSet(TCCR5A, COM5B1);
      OCR5B = val; // set pwm duty
      break;
#endif

#if defined(TCCR5A) && defined(COM5C1)
    case TIMER5C:
      // connect pwm to pin on timer 5, channel C
      bitSet(TCCR5A, COM5C1);
      OCR5C = val; // set pwm duty
      break;
#endif

    default: break;
  }
}

void MOTORS::setA(int16_t pwm)
{	
	if(pwm == _oldPwm)	return;
		
	_oldPwm = pwm;

	if (pwm <= -255)
  {
    _offPWM(_timerPWM);
    *_outRegPWM |= _bitMaskPWM; //PWM HIGH
    *_outM1b |= _bitMaskM1b;  //B HIGH
    *_outM1a &= ~_bitMaskM1a; //A LOW
  }
  else if (pwm >= 255)
  {
    _offPWM(_timerPWM);
    *_outRegPWM |= _bitMaskPWM; //PWM HIGH
    *_outM1a |= _bitMaskM1a;  //A HIGH
    *_outM1b &= ~_bitMaskM1b; //B LOW
  }
  else if (pwm < 0)
  {
    _onPWM(_timerPWM, pwm *= -1);
    *_outM1b |= _bitMaskM1b;  //B HIGH
    *_outM1a &= ~_bitMaskM1a; //A LOW
  }
  else if (pwm > 0)
  {
    _onPWM(_timerPWM, pwm);
    *_outM1a |= _bitMaskM1a;  //A HIGH
    *_outM1b &= ~_bitMaskM1b; //B LOW
  }
  else
  {
    _offPWM(_timerPWM);
    *_outRegPWM &= ~_bitMaskPWM; //B LOW
    *_outM1a &= ~_bitMaskM1a; //A LOW
    *_outM1b &= ~_bitMaskM1b; //B LOW
  }
}

void MOTORS::setB(int16_t pwm)
{	
	if(pwm == _oldPwm)	return;
		
	_oldPwm = pwm;

	if (pwm <= -255)
  {
    _offPWM(_timerPWM);
    *_outRegPWM |= _bitMaskPWM; //PWM HIGH
    *_outM2b |= _bitMaskM2b;  //B HIGH
    *_outM2a &= ~_bitMaskM2a; //A LOW
  }
  else if (pwm >= 255)
  {
    _offPWM(_timerPWM);
    *_outRegPWM |= _bitMaskPWM; //PWM HIGH
    *_outM2a |= _bitMaskM2a;  //A HIGH
    *_outM2b &= ~_bitMaskM2b; //B LOW
  }
  else if (pwm < 0)
  {
    _onPWM(_timerPWM, pwm *= -1);
    *_outM2b |= _bitMaskM2b;  //B HIGH
    *_outM2a &= ~_bitMaskM2a; //A LOW
  }
  else if (pwm > 0)
  {
    _onPWM(_timerPWM, pwm);
    *_outM2a |= _bitMaskM2a;  //A HIGH
    *_outM2b &= ~_bitMaskM2b; //B LOW
  }
  else
  {
    _offPWM(_timerPWM);
    *_outRegPWM &= ~_bitMaskPWM; //B LOW
    *_outM2a &= ~_bitMaskM2a; //A LOW
    *_outM2b &= ~_bitMaskM2b; //B LOW
  }
}

void MOTORS::setC(int16_t pwm)
{	
	if(pwm == _oldPwm)	return;
		
	_oldPwm = pwm;

	if (pwm <= -255)
  {
    _offPWM(_timerPWM);
    *_outRegPWM |= _bitMaskPWM; //PWM HIGH
    *_outM3b |= _bitMaskM3b;  //B HIGH
    *_outM3a &= ~_bitMaskM3a; //A LOW
  }
  else if (pwm >= 255)
  {
    _offPWM(_timerPWM);
    *_outRegPWM |= _bitMaskPWM; //PWM HIGH
    *_outM3a |= _bitMaskM3a;  //A HIGH
    *_outM3b &= ~_bitMaskM3b; //B LOW
  }
  else if (pwm < 0)
  {
    _onPWM(_timerPWM, pwm *= -1);
    *_outM3b |= _bitMaskM3b;  //B HIGH
    *_outM3a &= ~_bitMaskM3a; //A LOW
  }
  else if (pwm > 0)
  {
    _onPWM(_timerPWM, pwm);
    *_outM3a |= _bitMaskM3a;  //A HIGH
    *_outM3b &= ~_bitMaskM3b; //B LOW
  }
  else
  {
    _offPWM(_timerPWM);
    *_outRegPWM &= ~_bitMaskPWM; //B LOW
    *_outM3a &= ~_bitMaskM3a; //A LOW
    *_outM3b &= ~_bitMaskM3b; //B LOW
  }
}

void MOTORS::setD(int16_t pwm)
{	
	if(pwm == _oldPwm)	return;
		
	_oldPwm = pwm;
	
	if (pwm <= -255)
  {
    _offPWM(_timerPWM);
    *_outRegPWM |= _bitMaskPWM; //PWM HIGH
    *_outM4b |= _bitMaskM4b;  //B HIGH
    *_outM4a &= ~_bitMaskM4a; //A LOW
  }
  else if (pwm >= 255)
  {
    _offPWM(_timerPWM);
    *_outRegPWM |= _bitMaskPWM; //PWM HIGH
    *_outM4a |= _bitMaskM4a;  //A HIGH
    *_outM4b &= ~_bitMaskM4b; //B LOW
  }
  else if (pwm < 0)
  {
    _onPWM(_timerPWM, pwm *= -1);
    *_outM4b |= _bitMaskM4b;  //B HIGH
    *_outM4a &= ~_bitMaskM4a; //A LOW
  }
  else if (pwm > 0)
  {
    _onPWM(_timerPWM, pwm);
    *_outM4a |= _bitMaskM4b;  //A HIGH
    *_outM4b &= ~_bitMaskM4a; //B LOW
  }
  else
  {
    _offPWM(_timerPWM);
    *_outRegPWM &= ~_bitMaskPWM; //B LOW
    *_outM4a &= ~_bitMaskM4a; //A LOW
    *_outM4b &= ~_bitMaskM4b; //B LOW
  }
}

int MOTORS::encoder(uint8_t enc) {
  _enc = enc;
  switch (_enc)
  {
    case 1:
      return Encoder[0];
      break;
    case 2:
      return Encoder[1];
      break;
    case 3:
      return Encoder[2];
      break;
    case 4:
      return Encoder[3];
      break;
  }
}

void MOTORS::clear_enc(uint8_t enc) {
  _enc = enc;
  switch (_enc)
  {
    case 1:
      Encoder[0] = 0;
      break;
    case 2:
      Encoder[1] = 0;
      break;
    case 3:
      Encoder[2] = 0;
      break;
    case 4:
      Encoder[3] = 0;
      break;
  }
}

void MOTORS::follow(int16_t spMA, int16_t spMB) {
  _delta_time = millis() - _millis_old;
  _millis_old = millis();
  _spMA = spMA;
  _spMB = spMB;
  _goal_speedMA = constrain(_spMA, -_APS_max, _APS_max);
  _goal_speedMB = constrain(_spMB, -_APS_max, _APS_max);
  _now_speed = float(_rotate_enc * _delta_time) * 1000.0 / 360.0;
  _averenge_speed = ((Encoder[0] - EncoderOld[0]) + (Encoder[1] - EncoderOld[1]) + (Encoder[2] - EncoderOld[2]) + (Encoder[3] - EncoderOld[3])) / float(_rotate_enc / _delta_time) * 1000.0 * 360.0 / 4.0;

  _fixSpeedA = constrain((_averenge_speed - (Encoder[0] - EncoderOld[0]) / _now_speed) * _k + _goal_speedMA, -255, 255);
  _fixSpeedB = constrain((_averenge_speed - (Encoder[1] - EncoderOld[1]) / _now_speed) * _k + _goal_speedMA, -255, 255);
  _fixSpeedC = constrain((_averenge_speed - (Encoder[2] - EncoderOld[2]) / _now_speed) * _k + _goal_speedMB, -255, 255);
  _fixSpeedD = constrain((_averenge_speed - (Encoder[3] - EncoderOld[3]) / _now_speed) * _k + _goal_speedMB, -255, 255);

  EncoderOld[0] = Encoder[0];
  EncoderOld[1] = Encoder[1];
  EncoderOld[2] = Encoder[2];
  EncoderOld[3] = Encoder[3];

  setA(_fixSpeedA);
  setB(_fixSpeedB);
  setC(_fixSpeedC);
  setD(_fixSpeedD);
}