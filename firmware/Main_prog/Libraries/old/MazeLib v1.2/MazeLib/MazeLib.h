#ifndef MazeLib_h
#define MazeLib_h

#include <Arduino.h>

extern volatile uint8_t* _directionInRegA;
extern volatile uint8_t* _directionInRegB;
extern volatile uint8_t* _directionInRegC;
extern volatile uint8_t* _directionInRegD;

extern uint8_t _directionBitMaskA;
extern uint8_t _directionBitMaskB;
extern uint8_t _directionBitMaskC;
extern uint8_t _directionBitMaskD;

extern volatile int16_t Encoder[4];
extern volatile int16_t EncoderOld[4];

class MOTORS {
  public:
    MOTORS(uint8_t m1a, uint8_t m1b, uint8_t m2a, uint8_t m2b, uint8_t m3a, uint8_t m3b, uint8_t m4a, uint8_t m4b, uint8_t interruptA, uint8_t interruptB, uint8_t interruptC, uint8_t interruptD, uint8_t directionPinA, uint8_t directionPinB, uint8_t directionPinC, uint8_t directionPinD, uint8_t pinPWM, float RPM, float k, int16_t rotate_enc);
    MOTORS();
    int encoder(uint8_t enc);
    void attachMotors(uint8_t m1a, uint8_t m1b, uint8_t m2a, uint8_t m2b, uint8_t m3a, uint8_t m3b, uint8_t m4a, uint8_t m4b);
    void comparePinPWM(uint8_t pinPWM);
    void attachEnc(uint8_t interruptA, uint8_t interruptB, uint8_t interruptC, uint8_t interruptD, uint8_t directionPinA, uint8_t directionPinB, uint8_t directionPinC, uint8_t directionPinD);
    void setConst(float RPM, float k, int16_t rotate_enc);
    void clear_enc(uint8_t enc);
    void follow(int16_t spMA, int16_t spMB);
    void setA(int16_t pwm);
    void setB(int16_t pwm);
    void setC(int16_t pwm);
    void setD(int16_t pwm);
  private:
    //Setup
    float _APS_max;
    float _k;
    int16_t _rotate_enc;
    uint8_t _m1a;
    uint8_t _m2a;
    uint8_t _m3a;
    uint8_t _m4a;
    uint8_t _m1b;
    uint8_t _m2b;
    uint8_t _m3b;
    uint8_t _m4b;
    uint8_t _interruptA;
    uint8_t _interruptB;
    uint8_t _interruptC;
    uint8_t _interruptD;
    uint8_t _pinEncA;
    uint8_t _pinEncB;
    uint8_t _pinEncC;
    uint8_t _pinEncD;
    //Encoders
    uint8_t _enc;
    int32_t _encA_old;
    int32_t _encB_old;
    int32_t _encC_old;
    int32_t _encD_old;
    //PWM
    uint8_t _timerPWM;
    uint8_t _portPWM;
    int16_t _oldPwm;
    volatile uint8_t *_outRegPWM;
    volatile uint8_t *_outM1a;
    volatile uint8_t *_outM1b;
    volatile uint8_t *_outM2a;
    volatile uint8_t *_outM2b;
    volatile uint8_t *_outM3a;
    volatile uint8_t *_outM3b;
    volatile uint8_t *_outM4a;
    volatile uint8_t *_outM4b;
    uint8_t _bitMaskPWM;
    uint8_t _bitMaskM1a;
    uint8_t _bitMaskM1b;
    uint8_t _bitMaskM2a;
    uint8_t _bitMaskM2b;
    uint8_t _bitMaskM3a;
    uint8_t _bitMaskM3b;
    uint8_t _bitMaskM4a;
    uint8_t _bitMaskM4b;
    //Regulator
    int16_t _fixSpeedA;
    int16_t _fixSpeedB;
    int16_t _fixSpeedC;
    int16_t _fixSpeedD;
    double _goal_speedMA;
    double _goal_speedMB;
    double _now_speed;
    double _averenge_speed;
    int16_t _delta_time;
    int16_t _millis_old;
    int16_t _spMA;
    int16_t _spMB;
};

#endif