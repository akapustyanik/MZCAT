#include "Adafruit_VL53L0X.h"

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x01
#define LOX2_ADDRESS 0x02
#define LOX3_ADDRESS 0x03
#define LOX4_ADDRESS 0x04
#define LOX5_ADDRESS 0x05
#define LOX6_ADDRESS 0x06

// set the pins to shutdown
#define SHT_LOX1 31//31
#define SHT_LOX2 34//34
#define SHT_LOX3 33//33
#define SHT_LOX4 30//30
#define SHT_LOX5 35//35
#define SHT_LOX6 32//32

#define MOSFET1 22
#define MOSFET2 23
#define MOSFET3 24
#define MOSFET4 A13
#define MOSFET5 A14
#define MOSFET6 A15

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox5 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox6 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;
VL53L0X_RangingMeasurementData_t measure5;
VL53L0X_RangingMeasurementData_t measure6;

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
  digitalWrite(SHT_LOX6, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  digitalWrite(SHT_LOX5, HIGH);
  digitalWrite(SHT_LOX6, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);
  digitalWrite(SHT_LOX6, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot 1 VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot 2 VL53L0X"));
    while(1);
  }

  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX2
  if(!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot 3 VL53L0X"));
    while(1);
  }

  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  //initing LOX2
  if(!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot 4 VL53L0X"));
    while(1);
  }

  digitalWrite(SHT_LOX5, HIGH);
  delay(10);

  //initing LOX2
  if(!lox5.begin(LOX5_ADDRESS)) {
    Serial.println(F("Failed to boot 5 VL53L0X"));
    while(1);
  }

  digitalWrite(SHT_LOX6, HIGH);
  delay(10);

  //initing LOX2
  if(!lox6.begin(LOX6_ADDRESS)) {
    Serial.println(F("Failed to boot 6 VL53L0X"));
    //while(1);
  }
}

void read_sensors() {
  
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false);
  lox4.rangingTest(&measure4, false);
  lox5.rangingTest(&measure5, false);
  lox6.rangingTest(&measure6, false);
  
  // print sensor one reading
  Serial.print(F("1: "));
  if(measure1.RangeStatus != 4) {     // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  
  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("2: "));
  if(measure2.RangeStatus != 4) {
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("3: "));
  if(measure3.RangeStatus != 4) {
    Serial.print(measure3.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("4: "));
  if(measure4.RangeStatus != 4) {
    Serial.print(measure4.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("5: "));
  if(measure5.RangeStatus != 4) {
    Serial.print(measure5.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("6: "));
  if(measure6.RangeStatus != 4) {
    Serial.print(measure6.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(MOSFET1, OUTPUT);
  pinMode(MOSFET2, OUTPUT);
  pinMode(MOSFET3, OUTPUT);
  pinMode(MOSFET4, OUTPUT);
  pinMode(MOSFET5, OUTPUT);
  pinMode(MOSFET6, OUTPUT);
  digitalWrite(MOSFET1, LOW);
  digitalWrite(MOSFET2, LOW);
  digitalWrite(MOSFET3, LOW);
  digitalWrite(MOSFET4, LOW);
  digitalWrite(MOSFET5, LOW);
  digitalWrite(MOSFET6, LOW);
  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

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

void loop() {
  read_sensors();
  delay(1);
}
