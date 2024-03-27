#include <MazeLib.h>

volatile uint8_t *_directionInRegA;
volatile uint8_t *_directionInRegB;
volatile uint8_t *_directionInRegC;
volatile uint8_t *_directionInRegD;

uint8_t _directionBitMaskA;
uint8_t _directionBitMaskB;
uint8_t _directionBitMaskC;
uint8_t _directionBitMaskD;

volatile int32_t Encoder[4];
volatile int32_t EncoderZero[4];

//Interrupt encoder functions
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
    Encoder[2]++;
  else
    Encoder[2]--;
}
void _encoderD()
{
  if (*_directionInRegD & _directionBitMaskD)
    Encoder[3]++;
  else
    Encoder[3]--;
}

void MOTORS::softwarePWM(bool softwarePWM) {
  _softwarePWM = softwarePWM;
}

//Attach motors pins
void MOTORS::attachMotors(uint8_t m1a, uint8_t m1b, uint8_t m2a, uint8_t m2b, uint8_t m3a, uint8_t m3b, uint8_t m4a, uint8_t m4b) {
  _m1aPin = _m1a;
  _m1a = digitalPinToPort(m1a);
  _outM1a = portOutputRegister(_m1a);
  _bitMaskM1a = digitalPinToBitMask(m1a);
  _m1aTimerPWM = digitalPinToTimer(m1a);

  _m1bPin = _m1b;
  _m1b = digitalPinToPort(m1b);
  _outM1b = portOutputRegister(_m1b);
  _bitMaskM1b = digitalPinToBitMask(m1b);
  _m1bTimerPWM = digitalPinToTimer(m1b);

  _m2aPin = _m2a;
  _m2a = digitalPinToPort(m2a);
  _outM2a = portOutputRegister(_m2a);
  _bitMaskM2a = digitalPinToBitMask(m2a);
  _m2aTimerPWM = digitalPinToTimer(m2a);

  _m2bPin = _m2b;
  _m2b = digitalPinToPort(m2b);
  _outM2b = portOutputRegister(_m2b);
  _bitMaskM2b = digitalPinToBitMask(m2b);
  _m2bTimerPWM = digitalPinToTimer(m2b);

  _m3aPin = _m3a;
  _m3a = digitalPinToPort(m3a);
  _outM3a = portOutputRegister(_m3a);
  _bitMaskM3a = digitalPinToBitMask(m3a);
  _m3aTimerPWM = digitalPinToTimer(m3a);

  _m3bPin = _m3b;
  _m3b = digitalPinToPort(m3b);
  _outM3b = portOutputRegister(_m3b);
  _bitMaskM3b = digitalPinToBitMask(m3b);
  _m3bTimerPWM = digitalPinToTimer(m3b);

  _m4aPin = _m4a;
  _m4a = digitalPinToPort(m4a);
  _outM4a = portOutputRegister(_m4a);
  _bitMaskM4a = digitalPinToBitMask(m4a);
  _m4aTimerPWM = digitalPinToTimer(m4a);

  _m4bPin = _m4b;
  _m4b = digitalPinToPort(m4b);
  _outM4b = portOutputRegister(_m4b);
  _bitMaskM4b = digitalPinToBitMask(m4b);
  _m4bTimerPWM = digitalPinToTimer(m4b);

  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);
  pinMode(m3a, OUTPUT);
  pinMode(m3b, OUTPUT);
  pinMode(m4a, OUTPUT);
  pinMode(m4b, OUTPUT);
}

//Set regulator constations
void MOTORS::setConst(float RPM, float k) {
  _APS_max = float(RPM) * 360.0 / 60.0;
  _k = k;
}

//Attach encoders pins and interrupt numbers
void MOTORS::attachEnc(uint8_t interruptA, uint8_t interruptB, uint8_t interruptC, uint8_t interruptD, uint8_t directionPinA, uint8_t directionPinB, uint8_t directionPinC, uint8_t directionPinD) {
  pinMode(InterruptToPin(interruptA), INPUT);
  pinMode(InterruptToPin(interruptB), INPUT);
  pinMode(InterruptToPin(interruptC), INPUT);
  pinMode(InterruptToPin(interruptD), INPUT);
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

//Setup motors with user values
MOTORS::MOTORS(uint8_t m1a, uint8_t m1b, uint8_t m2a, uint8_t m2b, uint8_t m3a, uint8_t m3b, uint8_t m4a, uint8_t m4b, uint8_t interruptA, uint8_t interruptB, uint8_t interruptC, uint8_t interruptD, uint8_t directionPinA, uint8_t directionPinB, uint8_t directionPinC, uint8_t directionPinD, float RPM, float k) {
  _APS_max = RPM / 60.0 * 360.0;
  _k = k;
  _softwarePWM = 1;

  _m1aPin = _m1a;
  _m1a = digitalPinToPort(m1a);
  _outM1a = portOutputRegister(_m1a);
  _bitMaskM1a = digitalPinToBitMask(m1a);
  _m1aTimerPWM = digitalPinToTimer(m1a);

  _m1bPin = _m1b;
  _m1b = digitalPinToPort(m1b);
  _outM1b = portOutputRegister(_m1b);
  _bitMaskM1b = digitalPinToBitMask(m1b);
  _m1bTimerPWM = digitalPinToTimer(m1b);

  _m2aPin = _m2a;
  _m2a = digitalPinToPort(m2a);
  _outM2a = portOutputRegister(_m2a);
  _bitMaskM2a = digitalPinToBitMask(m2a);
  _m2aTimerPWM = digitalPinToTimer(m2a);

  _m2bPin = _m2b;
  _m2b = digitalPinToPort(m2b);
  _outM2b = portOutputRegister(_m2b);
  _bitMaskM2b = digitalPinToBitMask(m2b);
  _m2bTimerPWM = digitalPinToTimer(m2b);

  _m3aPin = _m3a;
  _m3a = digitalPinToPort(m3a);
  _outM3a = portOutputRegister(_m3a);
  _bitMaskM3a = digitalPinToBitMask(m3a);
  _m3aTimerPWM = digitalPinToTimer(m3a);

  _m3bPin = _m3b;
  _m3b = digitalPinToPort(m3b);
  _outM3b = portOutputRegister(_m3b);
  _bitMaskM3b = digitalPinToBitMask(m3b);
  _m3bTimerPWM = digitalPinToTimer(m3b);

  _m4aPin = _m4a;
  _m4a = digitalPinToPort(m4a);
  _outM4a = portOutputRegister(_m4a);
  _bitMaskM4a = digitalPinToBitMask(m4a);
  _m4aTimerPWM = digitalPinToTimer(m4a);

  _m4bPin = _m4b;
  _m4b = digitalPinToPort(m4b);
  _outM4b = portOutputRegister(_m4b);
  _bitMaskM4b = digitalPinToBitMask(m4b);
  _m4bTimerPWM = digitalPinToTimer(m4b);

  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);
  pinMode(m3a, OUTPUT);
  pinMode(m3b, OUTPUT);
  pinMode(m4a, OUTPUT);
  pinMode(m4b, OUTPUT);
  pinMode(InterruptToPin(interruptA), INPUT);
  pinMode(InterruptToPin(interruptB), INPUT);
  pinMode(InterruptToPin(interruptC), INPUT);
  pinMode(InterruptToPin(interruptD), INPUT);
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

//Setup motors with deafuls values
MOTORS::MOTORS() {
  _softwarePWM = 1;
  _APS_max = 660.0;
  _k = 2.0;
  _softwarePWM = 1;

  _m1a = digitalPinToPort(5);
  _outM1a = portOutputRegister(_m1a);
  _bitMaskM1a = digitalPinToBitMask(5);
  _m1aTimerPWM = digitalPinToTimer(5);

  _m1b = digitalPinToPort(4);
  _outM1b = portOutputRegister(_m1b);
  _bitMaskM1b = digitalPinToBitMask(4);
  _m1bTimerPWM = digitalPinToTimer(4);

  _m2a = digitalPinToPort(7);
  _outM2a = portOutputRegister(_m2a);
  _bitMaskM2a = digitalPinToBitMask(7);
  _m2aTimerPWM = digitalPinToTimer(7);

  _m2b = digitalPinToPort(6);
  _outM2b = portOutputRegister(_m2b);
  _bitMaskM2b = digitalPinToBitMask(6);
  _m2bTimerPWM = digitalPinToTimer(6);

  _m3a = digitalPinToPort(10);
  _outM3a = portOutputRegister(_m3a);
  _bitMaskM3a = digitalPinToBitMask(10);
  _m3aTimerPWM = digitalPinToTimer(10);

  _m3b = digitalPinToPort(11);
  _outM3b = portOutputRegister(_m3b);
  _bitMaskM3b = digitalPinToBitMask(11);
  _m3bTimerPWM = digitalPinToTimer(11);

  _m4a = digitalPinToPort(8);
  _outM4a = portOutputRegister(_m4a);
  _bitMaskM4a = digitalPinToBitMask(8);
  _m4aTimerPWM = digitalPinToTimer(8);

  _m4b = digitalPinToPort(9);
  _outM4b = portOutputRegister(_m4b);
  _bitMaskM4b = digitalPinToBitMask(9);
  _m4bTimerPWM = digitalPinToTimer(9);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(InterruptToPin(0), INPUT);
  pinMode(InterruptToPin(1), INPUT);
  pinMode(InterruptToPin(4), INPUT);
  pinMode(InterruptToPin(5), INPUT);
  pinMode(54, INPUT);
  pinMode(55, INPUT);
  pinMode(56, INPUT);
  pinMode(57, INPUT);

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

//Off PWM function
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
		case TIMER4D:	bitClear(TCCR4C, COM4D1);	break;
		#endif			
			
		#if defined(TCCR5A)
		case  TIMER5A:  bitClear(TCCR5A, COM5A1);    break;
		case  TIMER5B:  bitClear(TCCR5A, COM5B1);    break;
		case  TIMER5C:  bitClear(TCCR5A, COM5C1);    break;
		#endif
	}
}

//On PWM function
void _onPWM(uint8_t timer, int val)
{
	switch(timer)
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
				#if defined(COM4A0)		// only used on 32U4
				bitClear(TCCR4A, COM4A0);
				#endif
				OCR4A = val;	// set pwm duty
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
				#if defined(COM4D0)		// only used on 32U4
				bitClear(TCCR4C, COM4D0);
				#endif
				OCR4D = val;	// set pwm duty
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

//Set speed to motor A
void MOTORS::set(int16_t pwmA, int16_t pwmB, int16_t pwmC, int16_t pwmD)
{	
  if(_softwarePWM == 1) {
    if (pwmA <= -255)
    {
      _offPWM(_m1aTimerPWM);
      _offPWM(_m1bTimerPWM);
      *_outM1b |= _bitMaskM1b;  //PWM B HIGH
      *_outM1a &= ~_bitMaskM1a; //PWM A LOW
    }
    else if (pwmA >= 255)
    {
      _offPWM(_m1aTimerPWM);
      _offPWM(_m1bTimerPWM);
      *_outM1a |= _bitMaskM1a;  //PWM A HIGH
      *_outM1b &= ~_bitMaskM1b; //PWM B LOW
    }
    else if (pwmA < 0)
    {
      _onPWM(_m1bTimerPWM, pwmA *= -1);
      *_outM1a &= ~_bitMaskM1a; //PWM A LOW
    }
    else if (pwmA > 0)
    {
      _onPWM(_m1aTimerPWM, pwmA);
      *_outM1b &= ~_bitMaskM1b; //PWM B LOW
    }
    else
    {
      _offPWM(_m1aTimerPWM);
      _offPWM(_m1bTimerPWM);
      *_outM1a &= ~_bitMaskM1a; //PWM A LOW
      *_outM1b &= ~_bitMaskM1b; //PWM B LOW
    }

    if (pwmB <= -255)
    {
      _offPWM(_m2aTimerPWM);
      _offPWM(_m2bTimerPWM);
      *_outM2b |= _bitMaskM2b;  //PWM B HIGH
      *_outM2a &= ~_bitMaskM2a; //PWM A LOW
    }
    else if (pwmB >= 255)
    {
      _offPWM(_m2aTimerPWM);
      _offPWM(_m2bTimerPWM);
      *_outM2a |= _bitMaskM2a;  //PWM A HIGH
      *_outM2b &= ~_bitMaskM2b; //PWM B LOW
    }
    else if (pwmB < 0)
    {
      _onPWM(_m2bTimerPWM, pwmB *= -1);
      *_outM2a &= ~_bitMaskM2a; //PWM A LOW
    }
    else if (pwmB > 0)
    {
      _onPWM(_m2aTimerPWM, pwmB);
      *_outM2b &= ~_bitMaskM2b; //PWM B LOW
    }
    else
    {
      _offPWM(_m2aTimerPWM);
      _offPWM(_m2bTimerPWM);
      *_outM2a &= ~_bitMaskM2a; //PWM A LOW
      *_outM2b &= ~_bitMaskM2b; //PWM B LOW
    }

    if (pwmC <= -255)
    {
      _offPWM(_m3aTimerPWM);
      _offPWM(_m3bTimerPWM);
      *_outM3b |= _bitMaskM3b;  //PWM B HIGH
      *_outM3a &= ~_bitMaskM3a; //PWM A LOW
    }
    else if (pwmC >= 255)
    {
      _offPWM(_m3aTimerPWM);
      _offPWM(_m3bTimerPWM);
      *_outM3a |= _bitMaskM3a;  //PWM A HIGH
      *_outM3b &= ~_bitMaskM3b; //PWM B LOW
    }
    else if (pwmC < 0)
    {
      _onPWM(_m3bTimerPWM, pwmC *= -1);
      *_outM3a &= ~_bitMaskM3a; //PWM A LOW
    }
    else if (pwmC > 0)
    {
      _onPWM(_m3aTimerPWM, pwmC);
      *_outM3b &= ~_bitMaskM3b; //PWM B LOW
    }
    else
    {
      _offPWM(_m3aTimerPWM);
      _offPWM(_m3bTimerPWM);
      *_outM3a &= ~_bitMaskM3a; //PWM A LOW
      *_outM3b &= ~_bitMaskM3b; //PWM B LOW
    }
    
    if (pwmD <= -255)
    {
      _offPWM(_m4aTimerPWM);
      _offPWM(_m4bTimerPWM);
      *_outM4b |= _bitMaskM4b;  //PWM B HIGH
      *_outM4a &= ~_bitMaskM4a; //PWM A LOW
    }
    else if (pwmD >= 255)
    {
      _offPWM(_m4aTimerPWM);
      _offPWM(_m4bTimerPWM);
      *_outM4a |= _bitMaskM4a;  //PWM A HIGH
      *_outM4b &= ~_bitMaskM4b; //PWM B LOW
    }
    else if (pwmD < 0)
    {
      _onPWM(_m4bTimerPWM, pwmD *= -1);
      *_outM4a &= ~_bitMaskM4a; //PWM A LOW
    }
    else if (pwmD > 0)
    {
      _onPWM(_m4aTimerPWM, pwmD);
      *_outM4b &= ~_bitMaskM4b; //PWM B LOW
    }
    else
    {
      _offPWM(_m4aTimerPWM);
      _offPWM(_m4bTimerPWM);
      *_outM4a &= ~_bitMaskM4a; //PWM A LOW
      *_outM4b &= ~_bitMaskM4b; //PWM B LOW
    }
  }
  else {
    analogWrite(5, max(pwmA, 0));
    analogWrite(4, sgn(pwmA) * min(pwmA, 0));
    analogWrite(7, max(pwmB, 0));
    analogWrite(6, sgn(pwmB) * min(pwmB, 0));
    analogWrite(10, max(pwmC, 0));
    analogWrite(11, sgn(pwmC) * min(pwmC, 0));
    analogWrite(8, max(pwmD, 0));
    analogWrite(9, sgn(pwmD) * min(pwmD, 0));
  }
}
//return motors encoders trough the function
int MOTORS::encoder(uint8_t enc) {
  switch (enc)
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

//clear motors encoders trough the function
void MOTORS::clear_enc(uint8_t enc) {
  switch (enc)
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

//   
//   _spMA = constrain(spMA, -_APS_max, _APS_max);
//   _spMB = constrain(spMB, -_APS_max, _APS_max);
//   _averenge_speed = ((Encoder[0] - EncoderOld[0]) + (Encoder[1] - EncoderOld[1]) + (Encoder[2] - EncoderOld[2]) + (Encoder[3] - EncoderOld[3])) / float(_rotate_enc) / float (_delta_time) * 1000.0 * 360.0 / 4.0;
//   _goal_speedMA = map(_spMA, -_APS_max, _APS_max, -255, 255);
//   _goal_speedMB = map(_spMB, -_APS_max, _APS_max, -255, 255);
//   _now_speed = float(_rotate_enc) * float(_delta_time) / 360.0 / 1000.0;

// _fixSpeedA = constrain((_averenge_speed - (Encoder[0] - EncoderOld[0]) / _now_speed) * _k + _goal_speedMA, -255, 255);
// _fixSpeedB = constrain((_averenge_speed - (Encoder[1] - EncoderOld[1]) / _now_speed) * _k + _goal_speedMA, -255, 255);
// _fixSpeedC = constrain((_averenge_speed - (Encoder[2] - EncoderOld[2]) / _now_speed) * _k + _goal_speedMB, -255, 255);
// _fixSpeedD = constrain((_averenge_speed - (Encoder[3] - EncoderOld[3]) / _now_speed) * _k + _goal_speedMB, -255, 255);

//Write all motors with regulator
void MOTORS::follow(int16_t spMA, int16_t spMB) {

  _spMA = float(spMA) * 255.0 / float(_APS_max);
  _spMB = float(spMB) * 255.0 / float(_APS_max);
  _spMA = constrain(_spMA, -255, 255);
  _spMB = constrain(_spMB, -255, 255);

  if(int(_oldspMA) != int(_spMA) or int(_oldspMB) != int(_spMB)) {
    EncoderZero[0] = Encoder[0];
    EncoderZero[1] = Encoder[1];
    EncoderZero[2] = Encoder[2];
    EncoderZero[3] = Encoder[3];
    allSredn = 0;
    //Serial.println("AAAAAAAAAAAA");
  }


  if(_spMA != 0 and _spMB != 0) {
    _Enc0 = abs(float(Encoder[0] - EncoderZero[0]) * 255.0 / float(_spMA));
    _Enc1 = abs(float(Encoder[1] - EncoderZero[1]) * 255.0 / float(_spMA));
    _Enc2 = abs(float(Encoder[2] - EncoderZero[2]) * 255.0 / float(_spMB));
    _Enc3 = abs(float(Encoder[3] - EncoderZero[3]) * 255.0 / float(_spMB));
    allSredn = float(_Enc0 + _Enc1 + _Enc2 + _Enc3) / 4.0;
    _fixSpeedA = constrain(_spMA + float(sgn(_spMA) * (allSredn  - _Enc0) * _k) * abs(float(_spMA)) / 255.0, -255, 255);
    _fixSpeedB = constrain(_spMA + float(sgn(_spMA) * (allSredn  - _Enc1) * _k) * abs(float(_spMA)) / 255.0, -255, 255);
    _fixSpeedC = constrain(_spMB + float(sgn(_spMB) * (allSredn  - _Enc2) * _k) * abs(float(_spMB)) / 255.0, -255, 255);
    _fixSpeedD = constrain(_spMB + float(sgn(_spMB) * (allSredn  - _Enc3) * _k) * abs(float(_spMB)) / 255.0, -255, 255);
  }
  else {
    if(_spMA != 0) {
      _Enc0 = abs(float(Encoder[0] - EncoderZero[0]) * 255.0 / float(_spMA));
      _Enc1 = abs(float(Encoder[1] - EncoderZero[1]) * 255.0 / float(_spMA));
      _Enc2 = 0;
      _Enc3 = 0;
      allSredn = float(_Enc0 + _Enc1) / 2.0;
      _fixSpeedA = constrain(_spMA + float(sgn(_spMA) * (allSredn  - _Enc0) * _k) * abs(float(_spMA)) / 255.0, -255, 255);
      _fixSpeedB = constrain(_spMA + float(sgn(_spMA) * (allSredn  - _Enc1) * _k) * abs(float(_spMA)) / 255.0, -255, 255);
      _fixSpeedC = 0;
      _fixSpeedD = 0;
    }
    else {
      if(_spMB != 0) {
        _Enc0 = 0;
        _Enc1 = 0;
        _Enc2 = abs(float(Encoder[2] - EncoderZero[2]) * 255.0 / float(_spMB));
        _Enc3 = abs(float(Encoder[3] - EncoderZero[3]) * 255.0 / float(_spMB));
        allSredn = float(_Enc2 + _Enc3) / 2.0;
        _fixSpeedA = 0;
        _fixSpeedB = 0;
        _fixSpeedC = constrain(_spMB + float(sgn(_spMB) * (allSredn  - _Enc2) * _k) * abs(float(_spMB)) / 255.0, -255, 255);
        _fixSpeedD = constrain(_spMB + float(sgn(_spMB) * (allSredn  - _Enc3) * _k) * abs(float(_spMB)) / 255.0, -255, 255);
      }
      else {
        _Enc0 = 0;
        _Enc1 = 0;
        _Enc2 = 0;
        _Enc3 = 0;
        _fixSpeedA = 0;
        _fixSpeedB = 0;
        _fixSpeedC = 0;
        _fixSpeedD = 0;
      }
    }
  }

  // Serial.print("Speed: ");
  // Serial.print(_fixSpeedA);
  // Serial.print(" ");
  // Serial.print(_fixSpeedB);
  // Serial.print(" ");
  // Serial.print(_fixSpeedC);
  // Serial.print(" ");
  // Serial.println(_fixSpeedD);

  // Serial.print("allSredn: ");
  // Serial.println(allSredn);
  // Serial.print("allEncoders: ");
  // Serial.print(_Enc0);
  // Serial.print(" ");
  // Serial.print(_Enc1);
  // Serial.print(" ");
  // Serial.print(_Enc2);
  // Serial.print(" ");
  // Serial.println(_Enc3);

  set(_fixSpeedA, _fixSpeedB, _fixSpeedC, _fixSpeedD);

  _oldspMA = _spMA;
  _oldspMB = _spMB;
}