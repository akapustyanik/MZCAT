unsigned long mill_A = 0, mill_B = 0, mill_C = 0, mill_D = 0;

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

#define mA_IN1 4
#define mA_IN2 5
#define mB_IN1 6
#define mB_IN2 7
#define mC_IN1 8
#define mC_IN2 9
#define mD_IN1 11
#define mD_IN2 10

#define EncPin_A 18
#define EncPin_B 19
#define EncPin_C 2
#define EncPin_D 3

#define Rx 17
#define Tx 16

int EncMA = 0;
int EncMB = 0;
int EncMC = 0;
int EncMD = 0;

void clear_encoder(int motor)
{
  if (motor == 1) EncMA = 0;
  if (motor == 2) EncMB = 0;
  if (motor == 3) EncMC = 0;
  if (motor == 4) EncMD = 0;
}
void clear_all_encoder()
{
  EncMA = 0;
  EncMB = 0;
  EncMC = 0;
  EncMD = 0;
}

void setup_motors()
{
  pinMode(EncPin_A, INPUT); // Инициализируем пин Encoder_output_A на вход
  pinMode(EncPin_B, INPUT); // Инициализируем пин Encoder_output_B на вход
  pinMode(EncPin_C, INPUT); // Инициализируем пин Encoder_output_C на вход
  pinMode(EncPin_D, INPUT); // Инициализируем пин Encoder_output_D на вход
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}
//изменение энкодеров
void enc_A() {
  if (digitalRead(A3) > 0)
    EncMA++;//Увеличение энкодера A
  else
    EncMA--;//Уменьшение энкодера A
}
void enc_B() {
  if (digitalRead(A2) > 0)
    EncMB++;//Увеличение энкодера B
  else
    EncMB--;//Уменьшение энкодера B
}
void enc_C() {
  if (digitalRead(A0) > 0)
    EncMC++;//Увеличение энкодера C
  else
    EncMC--;//Уменьшение энкодера C
}
void enc_D() {
  if (digitalRead(A1) > 0)
    EncMD++;//Увеличение энкодера D
  else
    EncMD--;//Уменьшение энкодера D
}

//функции управления моторами
void motorA(int speed_A)
{
  speed_A = constrain(speed_A, -255, 255);
  double err = (millis() - mill_A) * 0.001 * speed_A - EncMA;
  int u = err;
  analogWrite(mA_IN1, max(speed_A - u, 0));
  analogWrite(mA_IN2, sgn(speed_A) * min(speed_A + u, 0));
}


void motorB(int speed_B)
{
  speed_B = constrain(speed_B, -255, 255);
  double err = (millis() - 0) * 0.01 * speed_B  - abs(EncMB);
  double u = err;
  int M1A = max(speed_B, 0);
  int M1B = sgn(speed_B) * min(speed_B, 0);
  analogWrite(mB_IN1, max(M1A + u, 0));
  analogWrite(mB_IN2, max(M1B + u, 0));
  Serial.print(err);
  Serial.print(" ");
  Serial.print(M1A);
  Serial.print(" ");
  Serial.print(M1B);
  Serial.print(" ");
  Serial.println(EncMB);
}


void motorC(int speed_C)
{
  speed_C = constrain(speed_C, -255, 255);
  double err = (millis() - mill_C) * 0.001 * speed_C - EncMC;
  int u = err;
  analogWrite(mC_IN1, max(speed_C, 0));
  analogWrite(mC_IN2, sgn(speed_C) * min(speed_C, 0));
}

void motorD(int speed_D)
{
  speed_D = constrain(speed_D, -255, 255);
  double err = (millis() - mill_D) * 0.001 * float(speed_D) - EncMD;
  int u = err;
  analogWrite(mD_IN1, max(speed_D, 0));
  analogWrite(mD_IN2, sgn(speed_D) * min(speed_D, 0));
}

void setup() {
  Serial.begin(115200);
  setup_motors();
  attachInterrupt(5, enc_A, RISING);
  attachInterrupt(4, enc_B, RISING);
  attachInterrupt(0, enc_C, RISING);
  attachInterrupt(1, enc_D, RISING);
}
void loop() {
  motorB(100);
  //motorB(255);
  //motorC(100);
  //motorD(255);
  //  Serial.print(EncMA);
  //  Serial.print(" ");
  //  Serial.print(EncMB);
  //  Serial.print(" ");
  //  Serial.print(EncMC);
  //  Serial.print(" ");
  //  Serial.println(EncMD);
  //mill_B = millis();
  delay(10);
}
