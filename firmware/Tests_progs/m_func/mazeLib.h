#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
#define A A3
#define B A2
#define C A0
#define D A1

//class ENC {
//  public:
//    ENC(byte pin1, byte pin2) {
//      _pin1 = pin1;
//      _pin2 = pin2;
//    }
//
//    void ewrite() {
//      if (digitalRead(_pin2) > 0) _myEnc++;//Увеличение энкодера A
//      else _myEnc--;
//    }
//
//    void eclear() {
//      _myEnc = 0;
//    }
//
//    void eread() {
//      return _myEnc;
//    }
//  private:
//    byte _pin1;
//    byte _pin2;
//    uint32_t _myEnc;
//};

class MOTOR {
  public:
    MOTOR(byte pin1, byte pin2, char pinEnc, float RPM, int16_t d_mm, float k) {
      _pinEnc = pinEnc;
      _d_mm = d_mm;
      _pin1 = pin1;
      _pin2 = pin2;
      _RPM = RPM;
      _k = k;
    }

    void justRun(uint16_t msp) {
      _msp = msp;
      analogWrite(_pin1, max(_msp, 0));
      analogWrite(_pin2, sgn(_msp) * min(_msp, 0));
    }

    void justSpeed(float motorSpeed) {
      _motorSpeed = constrain(motorSpeed, -(_RPM * M_PI * _d_mm / 60 / 10), _RPM * M_PI * _d_mm / 60 / 10);         //скорость в см/сек
      _speed = map(_motorSpeed, -(_RPM * M_PI * _d_mm / 600), _RPM * M_PI * _d_mm / 600, -255, 255);                // об/мин мотора * Пи * диаметр в мм, переводим мм/мин в см/cек и переводим в диапозон от -255 до 255
      _motorSpeed = _RPM * map(abs(_speed), 0, 255, 0, 1000) * M_PI * _d_mm / 1000 / 600;                           // об/мин мотора * скорость(от 0 до 1) * Пи * диаметр в мм, переводим мм/мин в см/cек
      _fixSpeed = map(_motorSpeed, -(_RPM * M_PI * _d_mm / 600), _RPM * M_PI * _d_mm / 600, -255, 255);             // переводим в диапозон от -255 до 255

      //      _motorSpeed = constrain(motorSpeed, -(_RPM * M_PI * _d_mm / 60 / 10), _RPM * M_PI * _d_mm / 60 / 10); //скорость в см/сек
      //      _fixSpeed = map(_motorSpeed, -(_RPM * M_PI * _d_mm / 600), _RPM * M_PI * _d_mm / 600, -255, 255);// переводим в диапозон от -255 до 255

      //      Serial.print("fixSpeed ");
      //      Serial.println(_motorSpeed);

      analogWrite(_pin1, max(_fixSpeed, 0));
      analogWrite(_pin2, sgn(_fixSpeed) * min(_fixSpeed, 0));
    }

    void writeEnc() {
      if (digitalRead(_pinEnc) > 0) _enc++;//Увеличение энкодера C
      else _enc--;//Уменьшение энкодера C
    }

    int encoder() {
      return _enc;
    }

    void clear_enc() {
      _enc = 0;
    }

    //
    //      _goalSpeed = constrain(_goalSpeed, -(_RPM * M_PI * _d_mm / 60 / 10), _RPM * M_PI * _d_mm / 60 / 10); // об/мин мотора * Пи * диаметр в мм / переводим в сек / переводим в см - получаем скорость в см/сек
    //      _speed = map(_goalSpeed, -(_RPM * M_PI * _d_mm / 60 / 10), _RPM * M_PI * _d_mm / 60 / 10, -255, 255); // переводим в диапазон от -255 до 255
    //      _encSpeed = abs(_deltaEnc) / (millis() - _start_time);
    //      _fixSpeed = constrain(_speed + (_goalSpeed - _encSpeed) * _k, -255, 255);
    //      //_motorSpeed
    //      analogWrite(_pin1, max(_fixSpeed, 0));
    //      analogWrite(_pin2, sgn(_fixSpeed) * min(_fixSpeed, 0));

    //      if (start_time != _start_time) _enc = 0;
    //      _start_time = start_time;
    //      _encSpeed = _enc * 60000 / (millis() - _start_time)  / 370; //об/мин
    //      _speed =

    void followSpeed(float goalSpeed) {
      _goalSpeed = goalSpeed; //скорость в см/сек
      _goalSpeed = constrain(_goalSpeed, -(_RPM * M_PI * _d_mm / 60 / 10), _RPM * M_PI * _d_mm / 60 / 10);    //ограничиваем до максимально возможной мотором
      _speed = map(_goalSpeed, -(_RPM * M_PI * _d_mm / 600), _RPM * M_PI * _d_mm / 600, -255, 255);           // об/мин мотора * Пи * диаметр, переводим в см/сек и переводим в диапозон от -255 до 255
      _motorSpeed = _RPM * map(abs(_speed), 0, 255, 0, 100) * M_PI * _d_mm / 60 / 100 / 10;                   // об/мин мотора / (переводим в об/сек) * скорость(от 0 до 1) * Пи * диаметр в мм / (переводим в см) - поучаем см/сек
      _encSpeed = abs(_deltaEnc) * M_PI * _d_mm / (millis() - _start_time) * 100 / 370;                       //значение энкодера  * Пи * диаметр в мм / (переводим в см) / время в мс * (переводим мс в сек) / кол-во тиков за 1 оборот колеса - получаем см/сек
      _fixSpeed = constrain(_speed + sgn(_speed) * (_motorSpeed - _encSpeed) * _k, -255, 255);
      //      Serial.print("goalSpeed ");
      //      Serial.print(_goalSpeed);  
      //      Serial.print(" Speed ");
      //      Serial.print(_speed);
      //      Serial.print(" encSpeed ");
      //      Serial.print(_encSpeed);
      //      Serial.print(" motorSpeed ");
      //      Serial.print(_motorSpeed);
      //      Serial.print(" err ");
      //      Serial.println(_motorSpeed - _encSpeed);
      //      Serial.print("fixSpeed ");
            Serial.println(_fixSpeed);
      //      Serial.println(" ");
      analogWrite(_pin1, max(_fixSpeed, 0));
      analogWrite(_pin2, sgn(_fixSpeed) * min(_fixSpeed, 0));
      _start_time = millis();
      _deltaEnc = _enc - _encOld;
      _encOld = _enc;
    }

  private:
    float _k;
    byte _pin1;
    byte _pin2;
    double _enc;
    double _deltaEnc;
    double _encOld;
    char _pinEnc;
    float _goalSpeed;
    float _encSpeed;
    double _motorSpeed;
    float _speed;
    uint16_t _msp;
    float _RPM;
    int16_t _fixSpeed;
    int16_t _d_mm;
    uint32_t _start_time;
};
