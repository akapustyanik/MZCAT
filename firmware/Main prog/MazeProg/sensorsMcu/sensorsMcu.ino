struct Str {
  int state;
  float u;
};

int state = 0;
int u = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
}

void loop() {
  Str buf;

  if (Serial3.readBytes((byte*)&buf, sizeof(buf))) {
    state = buf.state;
  }

  if (digitalRead(47) == 1) {
    state++;
    while (digitalRead(47) == 1) {
      delay(1);
      Serial.println(digitalRead(47));
    }
  }

  if (state > 4) {
    state = 0;
  }
  Serial.println(digitalRead(47));
  
  buf.state = state;
  buf.u = u;
  Serial3.write((byte*)&buf, sizeof(buf));
  delay(10);
}
