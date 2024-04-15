
void forward_test(int speed_motors)
{

  //while ((abs(motorA.encoder()) + abs(motorB.encoder()) + abs(motorC.encoder()) + abs(motorD.encoder())) / 4 < 360 / (diam * 3.14) * distance)
  int err1 = abs(motorA.encoder()) - abs(motorD.encoder());
  int err2 = abs(motorB.encoder()) - abs(motorC.encoder());

  motorA.followSpeed(constrain(speed_motors + err1 * 0.05, -68.5, 68.5));
  motorB.followSpeed(constrain(speed_motors + err2 * 0.05, -68.5, 68.5));
  motorC.followSpeed(constrain(speed_motors - err2 * 0.05, -68.5, 68.5));
  motorD.followSpeed(constrain(speed_motors - err1 * 0.05, -68.5, 68.5));
}
float err, err2;
void forward(int speed_motors)
{
  if (a)
  {
    err = (right_wall - 70) * 0.5;
    err2 = (right2 - 150) * 0.8;
  }
  else {
    err = 0;
    err2 = 0;
  }
  motorA.justRun(speed_motors - err - err2);
  motorB.justRun(speed_motors - err - err2);
  motorC.justRun(speed_motors + err + err2 + 3);
  motorD.justRun(speed_motors + err + err2 + 3);
}
void back(int speed_motors)
{
  motorA.followSpeed(-25);
  motorB.followSpeed(-25);
  motorC.followSpeed(-25);
  motorD.followSpeed(-25);
}
void left()
{
  //  errP = ((alpha - z) * 1.6);
  //  errI = errI + errP * 0.55;
  //  if (abs(errI) > 10)
  //  {
  //    errI = sgn(errI) * 10;
  //  }//-(errP + errI)
  motorA.followSpeed(25);
  motorB.followSpeed(25);
  motorC.followSpeed(-25);
  motorD.followSpeed(-25);
}

void right()
{
  //  errP = ((alpha - z) * 1.6);
  //  errI = errI + errP * 0.55;
  //  if (abs(errI) > 10)
  //  {
  //    errI = sgn(errI) * 10;
  //  }
  motorA.followSpeed(-25);
  motorB.followSpeed(-25);
  motorC.followSpeed(25);
  motorD.followSpeed(25);
}
boolean correction = 1;
int corr_time = 3000, wall_dist = 50, kwall = 1, u, v = 100;
//void loop() {
//  // put your main code here, to run repeatedly:
//  if (correction) {
//    motorA.justRun(100);
//    motorB.justRun(100);
//    motorC.justRun(100);
//    motorD.justRun(100);
//    delay(10);
//  }
//  if (digitalRead(2) != 0 && digitalRead(3) != 0) {
//    correction = 0;
//    for (int t; t < corr_time; t++) {
//      u = mesure1.readDistMill() - wall_dist * kwall;
//      motorA.justRun(u + v);
//      motorB.justRun(u + v);
//      motorC.justRun(u - v);
//      motorD.justRun(U - v);
//    }
//  }
