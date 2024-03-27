void brain()
{
  if ((!(cameraLeft == 0)  or !(cameraRight == 0)) and cam_state == 1)
  {
    delay(5000);
    cam_state = 0;
  }
  clearAllEnc();
  if (right_wall > 150)
  {
    //alpha = alpha + 15;
    state = 2;
  }
  else
  {
    if (front_wall > 150)
    {
      clearAllEnc();
      if (right_wall > 150) a = false;
      else a = true;
      cam_state = 1;
      state = 3;
    }
    else
    {
      //alpha = alpha - 4;
      state = 1;
    }
  }
}

void back_move()
{
  help++;
  back(30);
  if (help > 5)
  {
    motorA.justSpeed(0);
    motorB.justSpeed(0);
    motorC.justSpeed(0);
    motorD.justSpeed(0);
    clearAllEnc();
    help = 0;
    distance = 35;
    state = 0;
  }
}
void move_forward()
{
  if (right2 > 180 or right_wall > 150) a = false;
  else a = true;
  forward(75);
  if ((abs(motorA.encoder()) + abs(motorB.encoder()) + abs(motorC.encoder()) + abs(motorD.encoder())) / 4 > 360 / (7.7 * 3.141592) * distance)
  {
    motorA.justSpeed(0);
    motorB.justSpeed(0);
    motorC.justSpeed(0);
    motorD.justSpeed(0);
    state = 0;
    distance = 30.5;
  }
}
void turn_right()
{
  right();
  if ((abs(motorA.encoder()) + abs(motorB.encoder()) + abs(motorC.encoder()) + abs(motorD.encoder())) > 1100)
  {
    motorA.justSpeed(0);
    motorB.justSpeed(0);
    motorC.justSpeed(0);
    motorD.justSpeed(0);
    clearAllEnc();
    cam_state = 1;
    state = 3;
    //errI = 0;
  }
}
void turn_left()
{
  left();
  //if (abs(alpha) <= abs(z))
  if ((abs(motorA.encoder()) + abs(motorB.encoder()) + abs(motorC.encoder()) + abs(motorD.encoder())) > 1020)
  {
    motorA.justSpeed(0);
    motorB.justSpeed(0);
    motorC.justSpeed(0);
    motorD.justSpeed(0);
    clearAllEnc();
    state = 4;
    //errI = 0;
  }
}
