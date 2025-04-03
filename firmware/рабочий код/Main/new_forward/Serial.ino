bool readSerial() {
  static byte crc;
  if (errSerial) {
    Serial3.read();
    errSerial = 0;
  }
  Serial3.readBytes((byte *)&buf, sizeof(buf));
  crc = crc8_bytes((byte *)&buf, sizeof(buf));
  if (crc == 0) {
    for (int i = 0; i < 3; i++) {
      fromSerial.ypr[i] = buf.ypr[i];
      if(i == 2) {
        fromSerial.ypr[i] = int(fromSerial.ypr[i]) % 360;
      }
      fromSerial.ypr[i] -= zeroYprGlobal[i];
    }
    for (int i = 0; i < 4; i++) {
      fromSerial.enc[i] = buf.enc[i];
    }
    return 1;
  }
  errSerial = 1;
  return 0;
}

void writeSerial() {
  toSerial.crc = crc8_bytes((byte *)&toSerial, sizeof(toSerial) - 1);
  Serial3.write((byte *)&toSerial, sizeof(toSerial));
}

byte crc8_bytes(byte *buffer, byte size) {
  byte crc = 0;
  for (byte i = 0; i < size; i++) {
    byte data = buffer[i];
    for (int j = 8; j > 0; j--) {
      crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
      data >>= 1;
    }
  }
  return crc;
}