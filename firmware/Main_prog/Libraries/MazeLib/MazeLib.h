#ifndef MazeLib_h
#define MazeLib_h

#include <Arduino.h>

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
#define InterruptToPin(p) ((p) == 0 ? 2 : ((p) == 1 ? 3 : ((p) >= 2 && (p) <= 5 ? 23 - (p) : NOT_AN_INTERRUPT)))

extern volatile uint8_t* _directionInRegA;
extern volatile uint8_t* _directionInRegB;
extern volatile uint8_t* _directionInRegC;
extern volatile uint8_t* _directionInRegD;

extern uint8_t _directionBitMaskA;
extern uint8_t _directionBitMaskB;
extern uint8_t _directionBitMaskC;
extern uint8_t _directionBitMaskD;

extern volatile int32_t Encoder[4];
extern volatile int32_t EncoderZero[4];

class MOTORS {
  public:
    MOTORS(uint8_t m1a, uint8_t m1b, uint8_t m2a, uint8_t m2b, uint8_t m3a, uint8_t m3b, uint8_t m4a, uint8_t m4b, uint8_t interruptA, uint8_t interruptB, uint8_t interruptC, uint8_t interruptD, uint8_t directionPinA, uint8_t directionPinB, uint8_t directionPinC, uint8_t directionPinD, float RPM, float k);
    MOTORS();
    int32_t encoder(uint8_t enc);
    void attachMotors(uint8_t m1a, uint8_t m1b, uint8_t m2a, uint8_t m2b, uint8_t m3a, uint8_t m3b, uint8_t m4a, uint8_t m4b);
    void attachEnc(uint8_t interruptA, uint8_t interruptB, uint8_t interruptC, uint8_t interruptD, uint8_t directionPinA, uint8_t directionPinB, uint8_t directionPinC, uint8_t directionPinD);
    void setConst(float RPM, float k);
    void clear_enc(uint8_t enc);
    void follow(int16_t spMA, int16_t spMB);
    void set(int16_t pwmA, int16_t pwmB, int16_t pwmC, int16_t pwmD);
    void softwarePWM(bool softwarePWM);
  private:
    //Setup
    float _APS_max = 660.0;
    float _k = 2;
    uint8_t _m1a;
    uint8_t _m2a;
    uint8_t _m3a;
    uint8_t _m4a;
    uint8_t _m1b;
    uint8_t _m2b;
    uint8_t _m3b;
    uint8_t _m4b;
    uint8_t _m1aPin = 5;
    uint8_t _m2aPin = 7;
    uint8_t _m3aPin = 10;
    uint8_t _m4aPin = 8;
    uint8_t _m1bPin = 4;
    uint8_t _m2bPin = 6;
    uint8_t _m3bPin = 11;
    uint8_t _m4bPin = 9;
    //Encoders
    uint8_t _enc;
    //PWM
    bool _softwarePWM;
    uint8_t _m1aTimerPWM;
    uint8_t _m1bTimerPWM;
    uint8_t _m2aTimerPWM;
    uint8_t _m2bTimerPWM;
    uint8_t _m3aTimerPWM;
    uint8_t _m3bTimerPWM;
    uint8_t _m4aTimerPWM;
    uint8_t _m4bTimerPWM;
    volatile uint8_t *_outM1a;
    volatile uint8_t *_outM1b;
    volatile uint8_t *_outM2a;
    volatile uint8_t *_outM2b;
    volatile uint8_t *_outM3a;
    volatile uint8_t *_outM3b;
    volatile uint8_t *_outM4a;
    volatile uint8_t *_outM4b;
    uint8_t _bitMaskM1a;
    uint8_t _bitMaskM1b;
    uint8_t _bitMaskM2a;
    uint8_t _bitMaskM2b;
    uint8_t _bitMaskM3a;
    uint8_t _bitMaskM3b;
    uint8_t _bitMaskM4a;
    uint8_t _bitMaskM4b;
    //Regulator
    int32_t _Enc0, _Enc1, _Enc2, _Enc3;
    int16_t _fixSpeedA;
    int16_t _fixSpeedB;
    int16_t _fixSpeedC;
    int16_t _fixSpeedD;
    float _spMA;
    float _spMB;
    float _oldspMA = 0;
    float _oldspMB = 0;
    volatile int32_t allSredn = 0;
};

#endif