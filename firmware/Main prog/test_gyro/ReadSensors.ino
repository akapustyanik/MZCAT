



/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
*/
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  digitalWrite(SHT_LOX5, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }

  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX2
  if (!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot 3 VL53L0X"));
    while (1);
  }

  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  //initing LOX4
  if (!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot 4 VL53L0X"));
    while (1);
  }

  digitalWrite(SHT_LOX5, HIGH);
  delay(10);

  //initing LOX5
  if (!lox5.begin(LOX5_ADDRESS)) {
    Serial.println(F("Failed to boot 5 VL53L0X"));
    while (1);
  }
}

void read_five_sensors() {

  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false);
  lox4.rangingTest(&measure4, false);
  lox5.rangingTest(&measure5, false);

  //    Serial.print(measure1.RangeMilliMeter);
  //    Serial.print(' ');
  //    Serial.print(measure2.RangeMilliMeter);
  //    Serial.print(' ');
  //    Serial.print(measure3.RangeMilliMeter);
  //    Serial.print(' ');
  //    Serial.print(measure4.RangeMilliMeter);
  //    Serial.print(' ');
  //    Serial.println(measure5.RangeMilliMeter);
  if (measure1.RangeMilliMeter > 150)
  {
    front_wall = 0;
  }
  else
  {
    front_wall = 1;
  }
  if (measure2.RangeMilliMeter > 150)
  {
    left_wall = 0;
  }
  else
  {
    left_wall = 1;
  }
  if (measure3.RangeMilliMeter > 150)
  {
    right_wall = 0;
  }
  else
  {
    right_wall = 1;
  }
}

void read_gyro()
{
  //
  if(myIMU.readFloatGyroX()<-40) ax = 3;
  else if(myIMU.readFloatGyroX()<-20) ax = 2;
  else if(myIMU.readFloatGyroX()<-9) ax = 1; 
  else if(myIMU.readFloatGyroX()>-7) ax = -1;
  else if(myIMU.readFloatGyroX()>4) ax = -2;
  else if(myIMU.readFloatGyroX()>24) ax = -3;
  else ax = 0;
  if(myIMU.readFloatGyroY()<-40) ay = 3;
  else if(myIMU.readFloatGyroY()<-20) ay = 2;
  else if(myIMU.readFloatGyroY()<-9) ay = 1; 
  else if(myIMU.readFloatGyroY()>-7) ay = -1;
  else if(myIMU.readFloatGyroY()>4) ay = -2;
  else if(myIMU.readFloatGyroY()>24) ay = -3;
  else ay = 0;
  if(myIMU.readFloatGyroZ()<-40) az = 3;
  else if(myIMU.readFloatGyroZ()<-20) az = 2;
  else if(myIMU.readFloatGyroZ()<-9) az = 1; 
  else if(myIMU.readFloatGyroZ()>-7) az = -1;
  else if(myIMU.readFloatGyroZ()>4) az = -2;
  else if(myIMU.readFloatGyroZ()>24) az = -3;
  else az = 0;
  courseX = courseX + ax;
  courseY = courseY + ay;
  courseZ = courseZ + az;
}
void setup_sensors() {
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(A13, OUTPUT);
  pinMode(A14, OUTPUT);
  pinMode(A15, OUTPUT);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(A13, LOW);
  digitalWrite(A14, LOW);
  digitalWrite(A15, LOW);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);
  pinMode(SHT_LOX5, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));


  Serial.println(F("Starting..."));
  setID();
}
