int8_t preobrX(int8_t a) {
  return (a % mazeSizeX) + mazeSizeX * (a % mazeSizeX < 0);
}

int8_t preobrY(int8_t a) {
  return (a % mazeSizeY) + mazeSizeY * (a % mazeSizeY < 0);
}

int8_t preobrZ(int8_t a) {
  return (a % mazeSizeZ) + mazeSizeZ * (a % mazeSizeZ < 0);
}

int8_t preobrOrient(int8_t a) {
  return (a % 4) + 4 * (a % 4 < 0);
}

uint8_t getLabel(int8_t xGet, int8_t yGet, int8_t zGet) {
  xGet = preobrX(xGet);
  yGet = preobrY(yGet);
  zGet = preobrZ(zGet);
  return maze[xGet][yGet][zGet] / intPow(2, 7);
}
void setLabel(int8_t set, int8_t xSet, int8_t ySet, int8_t zSet) {
  xSet = preobrX(xSet);
  ySet = preobrY(ySet);
  zSet = preobrZ(zSet);
  uint8_t zero = getLabel(xSet, ySet, zSet);
  maze[xSet][ySet][zSet] += (set - zero) * intPow(2, 7);
}

void turn(int8_t newOrient, int16_t vMin, int16_t deltaMin, float kViravn1, float kViravn2) {
  uint8_t errSerial = 0;
  while (errSerial < 7) {
    if (Serial3.available()) {
      if (millis() > myTimer + sleep) {
        read_lasers();
        myTimer = millis();
      }
      if (!readSerial()) Serial.println("err with Serial3");
      else {
        errSerial++;
      }
    }
  }

  //return;
  int8_t isHelps[2] = { 0, 0 };
  read_lasers();
  if (lasers[0] <= minDist * 2) {
    viravn();
  } else {
    delay(250);
  }
  newOrient = preobrOrient(newOrient);
  int8_t deltaOrient = newOrient - orient;
  if (abs(deltaOrient) > 2) {
    deltaOrient -= 4 * sgn(deltaOrient);
  }
  if (!deltaOrient) {
    return;
  }
  float zeroYpr = round(float(fromSerial.ypr[0]) / 90.0) * 90.0;
  int16_t v;
  int8_t nowHelp = 0;
  uint32_t myTimer1;
  int8_t backCount = 0;
  int8_t extraCount = 0;
  while (Serial1.available()) {
    Serial1.read();
  }
  while (Serial2.available()) {
    Serial2.read();
  }
  for (int i = 0; i < abs(deltaOrient); i++) {
    myTimer1 = millis();
    while (abs(sgn(deltaOrient) * 90 - (fromSerial.ypr[0] - zeroYpr)) > deltaMin) {
      if ((digitalRead(forwBut[0]) or digitalRead(forwBut[1])) and backCount < 5) {
        forwardEncs(-50, vForward / 2);
        backCount++;
      }
      if (millis() > sleep + myTimer) {
        read_lasers();
        if (abs(sgn(deltaOrient) * 45) > abs(fromSerial.ypr[0] - zeroYpr)) {
          v = max(abs(fromSerial.ypr[0] - zeroYpr) * kAccelTurn, vMin * 4) * sgn(deltaOrient);
        } else {
          v = max(abs(sgn(deltaOrient) * 90 - (fromSerial.ypr[0] - zeroYpr)) * kAccelTurn / 2, vMin) * sgn(deltaOrient);
        }
        if (myTimer1 + 4000 < millis()) {
          v = vForward * 3 * sgn(deltaOrient);
        }
        if (myTimer1 + 6000 < millis() and extraCount < 1) {
          forwardEncs(-150, vForward);
          extraCount++;
        }
        if (myTimer1 + 8000 < millis() and extraCount < 2) {
          forwardEncs(300, vForward);
          extraCount++;
        }
        toSerial.vMotor[0] = toSerial.vMotor[2] = v;
        toSerial.vMotor[1] = toSerial.vMotor[3] = -v;
        writeSerial();
        myTimer = millis();
      }
      if (Serial3.available()) {
        if (!readSerial()) Serial.println("err with Serial3");
      }
      if (Serial1.available()) {
        nowHelp = Serial1.read() - int(ASCII_CONVERT);
        if (abs(fromSerial.ypr[0] - zeroYpr) < 90.0 * 7 / 24 and nowHelp != 0 and lasers[4] < lateralDist * 2) {
          throwHelp(nowHelp, 1);
          myTimer1 = millis();
        }
        if (abs(fromSerial.ypr[0] - zeroYpr) > 90.0 * 17 / 24 and nowHelp != 0) {
          isHelps[0] = nowHelp;
        }
      }
      if (Serial2.available()) {
        nowHelp = Serial2.read() - int(ASCII_CONVERT);
        if (abs(fromSerial.ypr[0] - zeroYpr) < 90.0 * 7 / 24 and nowHelp != 0 and lasers[3] < lateralDist * 2) {
          throwHelp(nowHelp, -1);
          myTimer1 = millis();
        }
        if (abs(fromSerial.ypr[0] - zeroYpr) > 90.0 * 17 / 24 and nowHelp != 0) {
          isHelps[1] = nowHelp;
        }
      }
    }
    orient = preobrOrient(orient + sgn(deltaOrient));
    if (isHelps[0] != 0 and lasers[4] < lateralDist * 2) {
      throwHelp(isHelps[0], 1);
    }
    if (isHelps[1] != 0 and lasers[3] < lateralDist * 2) {
      throwHelp(isHelps[1], -1);
    }
    isHelps[0] = 0;
    isHelps[1] = 0;
    zeroYpr += 90 * sgn(deltaOrient);
  }
  if (abs(deltaOrient) <= 2) {
    zeroYprGlobal[0] -= 5 * sgn(deltaOrient) * (abs(deltaOrient) >= 2);
  }
  toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
  while (millis() < sleep + myTimer)
    ;
  writeSerial();
  myTimer = millis();
  // if ((barrier[1] == 1 or barrier[1] == 2) and abs(barrier[0]) + abs(deltaOrient) == 2) {
  //   forwardGyro(deltaOrient * -barrier[0] * 100, vForward / 2);
  //   delay(500);
  // }
  read_lasers();
  if (lasers[5] < minDist * 2 and lasers[5] > 60) {
    viravnBack();
  }
}

void throwHelp(int8_t nowH, int8_t myOrient) {
  uint32_t myTimer1;
  int8_t state;
  if (nowH != 0 and nowHelps[preobrOrient(orient + myOrient)] == 0
      and getLabel(xRobot, yRobot, zRobot) == 0 and !getWall(xRobot, yRobot, zRobot, orient + myOrient)) {
    while (millis() < sleep + myTimer)
      ;
    toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
    writeSerial();
    myTimer = millis();
    myTimer1 = millis();
    int16_t wait = 5000 / nowH / 2;
    state = ((millis() - myTimer1) / wait) % 2;
    while (myTimer1 + 5000 > millis()) {
      if (state != ((millis() - myTimer1) / wait) % 2) {
        state = ((millis() - myTimer1) / wait) % 2;
        for (int i = 5 - (myOrient + 1) * 5 / 2; i < 10 - (myOrient + 1) * 5 / 2; i++) {
          strip.setPixelColor(i, strip.Color(255 * state, 0, 0));  //  Set pixel's color (in RAM)
        }
        strip.show();
      }
      if (millis() > sleep + myTimer) {
        read_lasers();
        writeSerial();
        myTimer = millis();
      }
      if (Serial3.available()) {
        if (!readSerial()) Serial.println("err with Serial3");
      }
    }
    for (int i = 5 - (myOrient + 1) * 5 / 2; i < 10 - (myOrient + 1) * 5 / 2; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  //  Set pixel's color (in RAM)
    }
    strip.show();  //  Update strip to match
    nowHelps[preobrOrient(orient + myOrient)] = 1;
    for (int i = 0; i < helps[nowH - 1]; i++) {
      if (numHelps < numHelpsMax) {
        myServo.write(srednServo * (2 - myOrient) / 2);
        delay(500);
        myServo.write(srednServo + 10 * myOrient);
        delay(500);
        numHelps++;
      }
    }
    first_read();
  }
}

void forward(int16_t v, int16_t distEnc, float kGyro, float kLaser1, float kLaser2, float kViravn1) {
  //return;
  digitalWrite(13, zRobot == 1);
  setSearch(1, xRobot, yRobot, zRobot, orient, 0);

  int8_t nextX = preobrX(xRobot - orient % 2 * (orient - 2)), nextY = preobrY(yRobot - (orient + 1) % 2 * (orient - 1));

  uint32_t debugTimer = millis();

  uint8_t errSerial = 0;
  while (errSerial < 7) {
    if (Serial3.available()) {
      if (millis() > myTimer + sleep) {
        read_lasers();
        myTimer = millis();
      }
      if (!readSerial()) Serial.println("err with Serial3");
      else {
        errSerial++;
      }
    }
  }
  first_read();
  int16_t firstLasers[2] = { lasers[1] <= lateralDist * 2 and lasers[3] <= lateralDist * 2 - l1Dist, lasers[2] <= lateralDist * 2 and lasers[4] <= lateralDist * 2 - l1Dist };
  float encsCheck[2];
  int16_t check[2] = { 0, 0 };
  uint32_t timerCheck[2] = { millis() + 500, millis + 500 };
  int8_t startPitch = 0;
  startPitch = lastPitch;
  lastPitch = 0;
  distEnc += 125 * startPitch;
  int16_t u;
  bool slideNow = isSlide(xRobot, yRobot, zRobot), slideNext = isSlide(nextX, nextY, zRobot);
  float zeroYpr = round(float(fromSerial.ypr[0]) / 90.0) * 90.0;
  float oldEncs = float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0;
  float encs = 0;
  int8_t laserNum;
  float alpha, alpha1;
  const uint32_t startTimer = millis();
  int16_t vReg;
  float laserCorr;
  float horizont = 0;
  bool isHorizont = 0;
  bool zeroHelps = 0;
  int8_t pitchFlag = 1;
  int8_t isHelps[2] = { 0, 0 };
  int8_t nowHelp = 0;
  int16_t blackSquare = 0, blueSquare = 0, silverSquare = 0;
  int8_t numBackwards = 0;
  int8_t numOneSide = 0;
  float corr = 0;
  int16_t numCorr = 1;
  float corrEncs[2] = { -1, -1 };
  bool laserCorrFlag = 0;
  int16_t color;
  while (Serial1.available()) {
    Serial1.read();
  }
  while (Serial2.available()) {
    Serial2.read();
  }
  while (1) {
    if (abs(fromSerial.ypr[0] - zeroYpr) < 10 and abs(fromSerial.ypr[1]) < 10 and millis() - startTimer > 1000 and encs * 2 > distEnc and pitchFlag) {
      if (lasers[5] < squareDist + l3Dist and lasers[5] > 60 and !slideNow) {
        laserCorr = float(lasers[5] - minDist - l3Dist) * float(distEnc) / float(squareDist) * cos(radians(fromSerial.ypr[0] - zeroYpr)) * cos(radians(fromSerial.ypr[1]));
        laserCorrFlag = 1;
        Serial.println(String(lasers[5] - minDist - l3Dist) + " " + String(lasers[5]) + " " + String(cos(radians(fromSerial.ypr[0] - zeroYpr)) * cos(radians(fromSerial.ypr[1]))));
      }
      if (lasers[0] < squareDist and !slideNext) {
        laserCorr = (float(distEnc) - float(lasers[0] - deltaL - minDist) * float(distEnc) / float(squareDist)) * cos(radians(fromSerial.ypr[0] - zeroYpr)) * cos(radians(fromSerial.ypr[1]));
        laserCorrFlag = 1;
      }
      if (laserCorrFlag and abs(laserCorr - encs) < 250) {
        encs = laserCorr;
        laserCorrFlag = 0;
      }
    } else {
    }
    //Serial.println(String(cos(radians(fromSerial.ypr[0] - zeroYpr)) * cos(radians(fromSerial.ypr[1]))) + " " + String(fromSerial.ypr[0] - zeroYpr) + " " + String(fromSerial.ypr[1]));
    if (millis() > startTimer + 1500) {
      if (digitalRead(forwBut[0]) and digitalRead(forwBut[1])) {
        break;
        if (abs(fromSerial.ypr[1]) > 5) {
          lastHorizont = 1;
        } else {
          lastHorizont = 0;
        }
      }
      if (encs > distEnc) {
        if (abs(fromSerial.ypr[1]) > 5) {
          lastHorizont = 1;
        } else {
          lastHorizont = 0;
        }
        break;
      }
    }

    if (abs(fromSerial.ypr[1]) > 5) {
      pitchFlag = 0;
    }

    if (encs < distEnc * (3 + abs(startPitch)) / 4 and encs > distEnc / 4 and abs(fromSerial.ypr[1]) < 5 and blackSquare >= 0) {
      color = analogRead(0);
      if (color > blackColor[0] and color < blackColor[1]) {
        blackSquare++;
      } else {
        blackSquare = 0;
      }
    }
    if (blackSquare > 50) {
      blackSquare = -1;
      while (millis() < sleep + myTimer)
        ;
      toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
      writeSerial();
      myTimer = millis();
      setSearch(1, nextX, nextY, zRobot, orient, 0);
      for (int i = 0; i < 4; i++) {
        setWall(0, nextX, nextY, zRobot, i);
      }
      delay(500);
      //forwardGyro(-encs + startPitch * 100);
      numBackwards = 0;
      while (getCeiling(xRobot, yRobot, zRobot, (-startPitch + 1) / 2)) {
        //forwardGyro(-distEnc + startPitch * 100);
        nextX = preobrX(xRobot + orient % 2 * (orient - 2));
        nextY = preobrY(yRobot + (orient + 1) % 2 * (orient - 1));
        xRobot = nextX;
        yRobot = nextY;
        zRobot = preobrZ(zRobot - startPitch);
        numBackwards++;
      }
      forwardGyro(-encs - distEnc * numBackwards + startPitch * (175) * (numBackwards + 1) - 50 * (startPitch == 0));
      lastPitch = 0;
      break;
    }
    if (encs > distEnc * 17 / 24 and abs(fromSerial.ypr[1]) < 5 and blueSquare >= 0) {
      color = analogRead(0);
      if (color > blueColor[0] and color < blueColor[1]) {
        blueSquare++;
      } else {
        blueSquare = 0;
      }
    }
    if (blueSquare > 1) {
      blueSquare = -1;
    }
    if (encs > distEnc * 17 / 24 and (abs(horizont) < 75 or startPitch != 0) and abs(fromSerial.ypr[1]) < 5 and silverSquare >= 0) {
      color = analogRead(0);
      if (color > silverColor[0] and color < silverColor[1]) {
        silverSquare++;
      } else {
        silverSquare = 0;
      }
    }
    if (silverSquare > 5) {
      silverSquare = -1;
    }
    if ((digitalRead(forwBut[0]) xor digitalRead(forwBut[1])) == 1 and lasers[0] > minDist
        and (barrier[1] < 0 or barrier[1] > 2 or (barrier[0] == 1 and digitalRead(forwBut[0])) or (barrier[0] == -1 and digitalRead(forwBut[1]))) and numOneSide < 2) {
      while (millis() < sleep + myTimer)
        ;
      toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
      writeSerial();
      myTimer = millis();
      barrier[1] = 0;
      if (digitalRead(forwBut[0])) {
        barrier[0] = 1;
      } else {
        barrier[0] = -1;
      }
      oneSide(-100, barrier[0]);
      oneSide(-100, -barrier[0]);
      oneSide(-50, -barrier[0]);
      forwardGyro(100, vForward / 2);
      oldEncs = float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0;
      numOneSide++;
    }
    if (encs > distEnc * 3 / 4 and abs(horizont) > 100 and abs(fromSerial.ypr[1]) > 15 and isHorizont == 0) {
      if (startPitch == 0) {
        distEnc += sgn(horizont) * 125;
      }
      isHorizont = 1;
    }
    if (encs > distEnc / 2 and zeroHelps == 0) {
      if (nowHelps[0] + nowHelps[1] + nowHelps[2] + nowHelps[3] > 0) {
        setLabel(1, xRobot, yRobot, zRobot);
      }
      nowHelps[0] = nowHelps[1] = nowHelps[2] = nowHelps[3] = 0;
      zeroHelps = 1;
    }

    if (millis() > sleep + myTimer) {
      debugTimer = millis();
      if (encs < distEnc / 2) {
        vReg = max(encs * v * 2 / distEnc, v / 2);
      } else {
        vReg = max(float((distEnc - encs) * v * 2 / distEnc) / 2, v / 4);
      }

      read_lasers();

      if (barrier[1] < 0 or barrier[1] > 2) {
        if ((lasers[1] <= lateralDist * 2 and lasers[3] <= lateralDist * 2 - l1Dist) != firstLasers[0] and firstLasers[0] != 2 and firstLasers[1] != 2 and !(slideNow and slideNext)) {
          corrEncs[0] = 450 + ((lasers[1] <= lateralDist * 2 and lasers[3] <= lateralDist * 2 - l1Dist) - firstLasers[0]) * 50;
          encsCheck[0] = encs;
          check[0] = (lasers[1] <= lateralDist * 2 and lasers[3] <= lateralDist * 2 - l1Dist) + 1;
          firstLasers[0] = 2;
        }
        if ((lasers[2] <= lateralDist * 2 and lasers[4] <= lateralDist * 2 - l1Dist) != firstLasers[1] and firstLasers[0] != 2 and firstLasers[1] != 2 and !(slideNow and slideNext)) {
          corrEncs[1] = 450 + ((lasers[2] <= lateralDist * 2 and lasers[4] <= lateralDist * 2 - l1Dist) - firstLasers[1]) * 50;
          encsCheck[1] = encs;
          check[1] = (lasers[2] <= lateralDist * 2 and lasers[4] <= lateralDist * 2 - l1Dist) + 1;
          firstLasers[1] = 2;
        }
      }

      if (check[0] - 1 != (lasers[1] <= lateralDist * 2 and lasers[3] <= lateralDist * 2 - l1Dist) or (lasers[1] >= lateralDist * 2 and check[0] - 1 == 0)) {
        timerCheck[0] = millis();
        check[0] = 0;
      } else {
        if (timerCheck[0] + 500 < millis() and corrEncs[0] >= 0) {
          if (abs(encs - (encs - encsCheck[0] + corrEncs[0])) < 250) {
            encs = encs - encsCheck[0] + corrEncs[0];
          }
          check[0] = 0;
          corrEncs[0] = -1;
        }
      }

      if (check[1] - 1 != (lasers[2] <= lateralDist * 2 and lasers[4] <= lateralDist * 2 - l1Dist) or (lasers[2] >= lateralDist * 2 and check[1] - 1 == 0)) {
        timerCheck[1] = millis();
        check[1] = 0;
      } else {
        if (timerCheck[1] + 500 < millis() and corrEncs[1] >= 0) {
          if (abs(encs - (encs - encsCheck[1] + corrEncs[1])) < 250) {
            encs = encs - encsCheck[1] + corrEncs[1];
          }
          check[1] = 0;
          corrEncs[1] = -1;
        }
      }

      laserNum = 0;
      if (barrier[1] < 0 or barrier[1] > 2) {
        if (lasers[1] <= lateralDist * 2 and lasers[3] <= lateralDist * 2 - l1Dist) {
          laserNum = 3;
        }
        if (lasers[2] <= lateralDist * 2 and lasers[4] <= lateralDist * 2 - l1Dist and ((laserNum == 3 and min(lasers[1], lasers[3]) < min(lasers[2], lasers[4])) or laserNum != 3)) {
          laserNum = 4;
        }
      }

      if (!laserNum) {
        u = float(fromSerial.ypr[0] - zeroYpr) * kGyro;
        numCorr = 1;
      } else {
        u = ((lateralDist + 40 * (laserNum == 4) - lasers[laserNum - 2]) * kLaser1 + (lasers[laserNum] - lasers[laserNum - 2] + l1Dist) * kLaser2) * (laserNum * 2 - 7);
        u += float(fromSerial.ypr[0] - zeroYpr) * kGyro;
        alpha1 = fromSerial.ypr[0] - zeroYpr;
        alpha = atan2(lasers[laserNum] - lasers[laserNum - 2] + l1Dist, l2Dist) * (laserNum * 2 - 7);
        corr = float(alpha1 - alpha) * 1.0 / float(numCorr) + corr * (1.0 - 1.0 / float(numCorr));
        numCorr++;
        if (numCorr == 75) {
          //zeroYprGlobal[0] += corr;
          numCorr = 1;
        }
      }

      toSerial.vMotor[0] = toSerial.vMotor[2] = vReg - u;
      toSerial.vMotor[1] = toSerial.vMotor[3] = vReg + u;

      writeSerial();
      myTimer = millis();
    }
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
      encs += (float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0 - oldEncs) * cos(radians(fromSerial.ypr[0] - zeroYpr)) * cos(radians(fromSerial.ypr[1]));
      horizont += sin(radians(fromSerial.ypr[1])) * (float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0 - oldEncs);
      oldEncs = float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0;
    }
    if (Serial1.available()) {
      nowHelp = Serial1.read() - int(ASCII_CONVERT);
      if (encs < distEnc / 4 and nowHelp != 0 and lasers[4] < lateralDist * 2) {
        throwHelp(nowHelp, 1);
      }
      if (encs > distEnc * 3 / 4 and nowHelp != 0) {
        isHelps[0] = nowHelp;
      }
    }
    if (Serial2.available()) {
      nowHelp = Serial2.read() - int(ASCII_CONVERT);
      if (encs < distEnc / 4 and nowHelp != 0 and lasers[3] < lateralDist * 2) {
        throwHelp(nowHelp, -1);
      }
      if (encs > distEnc * 3 / 4 and nowHelp != 0) {
        isHelps[1] = nowHelp;
      }
    }
  }
  while (millis() < sleep + myTimer)
    ;
  toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
  writeSerial();
  myTimer = millis();
  if (abs(horizont) > 100 and abs(fromSerial.ypr[1]) > 15) {
    lastPitch = sgn(fromSerial.ypr[1]);
  }
  if (blackSquare != -1) {
    yRobot = nextY;
    xRobot = nextX;
    setSearch(1, xRobot, yRobot, zRobot, orient, 0);
    if (lastPitch and silverSquare != -1) {
      setCeiling(1, xRobot, yRobot, zRobot, (lastPitch + 1) / 2);
      zRobot = preobrZ(zRobot + lastPitch);
      setSearch(1, xRobot, yRobot, zRobot, orient, 0);
    }
    if (isHelps[0] != 0 and lasers[4] < lateralDist * 2) {
      throwHelp(isHelps[0], 1);
    }
    if (isHelps[1] != 0 and lasers[3] < lateralDist * 2) {
      throwHelp(isHelps[1], -1);
    }
    if (blueSquare == -1) {
      for (int i = 0; i < 10; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 255));  //  Set pixel's color (in RAM)
      }
      strip.show();
      delay(5000);
      for (int i = 0; i < 10; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));  //  Set pixel's color (in RAM)
      }
      strip.show();
    }
    if (silverSquare == -1) {
      for (int i = 0; i < 10; i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255));  //  Set pixel's color (in RAM)
      }
      strip.show();
      delay(1000);
      allPositions[0] = xRobot;
      allPositions[1] = yRobot;
      allPositions[2] = zRobot;
      allPositions[3] = orient;
      for (int i = 0; i < 10; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));  //  Set pixel's color (in RAM)
      }
      strip.show();
    }
  }
  if (barrier[1] != -1) {
    barrier[1]++;
  }
  EEPROM.put(0, maze);
  EEPROM.put(sizeof(maze), allPositions);
  //delay(1000);
}

void viravn() {
  //return;
  float zeroYpr = round(float(fromSerial.ypr[0]) / 90.0) * 90.0;
  uint32_t myTimer1 = millis();
  int8_t viravnFlag = 1;
  int16_t v1 = vForward, v2 = vForward;
  while (v1 or v2) {
    if (myTimer1 + 2000 < millis()) {
      viravnFlag = 0;
      break;
    }
    if (millis() > sleep + myTimer) {
      read_lasers();

      if (digitalRead(forwBut[0]) == 1) {
        v1 = 0;
      }
      if (digitalRead(forwBut[1]) == 1) {
        v2 = 0;
      }

      toSerial.vMotor[0] = toSerial.vMotor[2] = v1;
      toSerial.vMotor[1] = toSerial.vMotor[3] = v2;

      writeSerial();
      myTimer = millis();
    }
  }
  const uint32_t gyroTimer = millis();
  while (millis() - gyroTimer < 250) {
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
    }
  }

  if (viravnFlag and abs(fromSerial.ypr[0] - zeroYpr) < 10) {
    zeroYprGlobal[0] += fromSerial.ypr[0];
  }

  read_lasers();

  while (lasers[0] < minDist or digitalRead(forwBut[0]) == 1 or digitalRead(forwBut[1]) == 1) {
    if (millis() > sleep + myTimer) {
      read_lasers();

      toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = -vForward / 4;

      writeSerial();

      myTimer = millis();
    }
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
    }
  }
  while (millis() < sleep + myTimer)
    ;
  toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
  writeSerial();
  myTimer = millis();

  if (!lastHorizont and viravnFlag and abs(fromSerial.ypr[1]) < 5) {
    zeroYprGlobal[1] += fromSerial.ypr[1];
  }
}

void viravnBack() {
  //return;
  float zeroYpr = round(float(fromSerial.ypr[0]) / 90.0) * 90.0;
  int16_t v1 = -vForward / 2, v2 = -vForward / 2;
  toSerial.vMotor[0] = toSerial.vMotor[2] = v1;
  toSerial.vMotor[1] = toSerial.vMotor[3] = v2;
  uint32_t myTimer1 = millis();
  while (myTimer1 + 1250 > millis()) {
    if (millis() > sleep + myTimer) {
      read_lasers();
      writeSerial();
      myTimer = millis();
    }
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
    }
  }

  if (abs(fromSerial.ypr[0] - zeroYpr) < 5) {
    zeroYprGlobal[0] += fromSerial.ypr[0];
  }

  read_lasers();

  toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = vForward / 4;

  while (lasers[5] < minDist) {
    if (millis() > sleep + myTimer) {
      read_lasers();
      writeSerial();
      myTimer = millis();
    }
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
    }
  }
  // delay(10000);
  while (millis() < sleep + myTimer)
    ;
  toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
  writeSerial();
  myTimer = millis();
}

void forwardGyro(int16_t distEnc, int16_t v, float kGyro, float kLaser1, float kLaser2, float kViravn1) {
  first_read();

  uint8_t errSerial = 0;
  while (errSerial < 7) {
    if (Serial3.available()) {
      if (millis() > myTimer + sleep) {
        read_lasers();
        myTimer = millis();
      }
      if (!readSerial()) Serial.println("err with Serial3");
      else {
        errSerial++;
      }
    }
  }
  int16_t u;
  float zeroYpr = round(float(fromSerial.ypr[0]) / 90.0) * 90.0;
  float oldEncs = float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0;
  float encs = 0;
  while (abs(encs) < abs(distEnc)) {
    if (millis() > sleep + myTimer) {
      read_lasers();
      u = float(fromSerial.ypr[0] - zeroYpr) * kGyro;
      toSerial.vMotor[0] = toSerial.vMotor[2] = v * sgn(distEnc) - u;
      toSerial.vMotor[1] = toSerial.vMotor[3] = v * sgn(distEnc) + u;
      writeSerial();
      myTimer = millis();
    }
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
      encs += (float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0 - oldEncs) * cos(radians(fromSerial.ypr[0] - zeroYpr)) * cos(radians(fromSerial.ypr[1]));
      oldEncs = float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0;
    }
  }
  while (millis() < sleep + myTimer)
    ;
  toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
  writeSerial();
  myTimer = millis();
}

void oneSide(int32_t distEnc, int8_t side, int16_t v) {
  float encs = 0;
  float oldEncs;
  uint8_t errSerial = 0;
  while (errSerial < 7) {
    if (Serial3.available()) {
      if (millis() > myTimer + sleep) {
        read_lasers();
        myTimer = millis();
      }
      if (!readSerial()) Serial.println("err with Serial3");
      else {
        errSerial++;
      }
    }
  }
  if (side > 0) {
    toSerial.vMotor[0] = toSerial.vMotor[2] = v * sgn(distEnc);
    toSerial.vMotor[1] = toSerial.vMotor[3] = 0;
    oldEncs = float(fromSerial.enc[0] + fromSerial.enc[2]) / 2.0;
  } else {
    toSerial.vMotor[0] = toSerial.vMotor[2] = 0;
    toSerial.vMotor[1] = toSerial.vMotor[3] = v * sgn(distEnc);
    oldEncs = float(fromSerial.enc[1] + fromSerial.enc[3]) / 2.0;
  }
  while (abs(encs) < abs(distEnc)) {
    if (millis() > sleep + myTimer) {
      read_lasers();
      writeSerial();
      myTimer = millis();
    }
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
      if (side > 0) {
        encs += (float(fromSerial.enc[0] + fromSerial.enc[2]) / 2.0 - oldEncs);
        oldEncs = float(fromSerial.enc[0] + fromSerial.enc[2]) / 2.0;
      } else {
        encs += (float(fromSerial.enc[1] + fromSerial.enc[3]) / 2.0 - oldEncs);
        oldEncs = float(fromSerial.enc[1] + fromSerial.enc[3]) / 2.0;
      }
    }
  }
  while (millis() < sleep + myTimer)
    ;
  toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
  writeSerial();
  myTimer = millis();
}
void forwardEncs(int16_t distEnc, int16_t v) {
  first_read();

  uint8_t errSerial = 0;
  while (errSerial < 7) {
    if (millis() > myTimer + sleep) {
      read_lasers();
      myTimer = millis();
    }
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
      else {
        errSerial++;
      }
    }
  }
  int16_t u;
  float zeroYpr = round(float(fromSerial.ypr[0]) / 90.0) * 90.0;
  float oldEncs = float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0;
  float encs = 0;
  toSerial.vMotor[0] = toSerial.vMotor[2] = toSerial.vMotor[1] = toSerial.vMotor[3] = v * sgn(distEnc);
  while (abs(encs) < abs(distEnc)) {
    if (millis() > sleep + myTimer) {
      read_lasers();
      writeSerial();
      myTimer = millis();
    }
    if (Serial3.available()) {
      if (!readSerial()) Serial.println("err with Serial3");
      encs += (float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0 - oldEncs) * cos(radians(fromSerial.ypr[0] - zeroYpr)) * cos(radians(fromSerial.ypr[1]));
      oldEncs = float(fromSerial.enc[0] + fromSerial.enc[1] + fromSerial.enc[2] + fromSerial.enc[3]) / 4.0;
    }
  }
  while (millis() < sleep + myTimer)
    ;
  toSerial.vMotor[0] = toSerial.vMotor[1] = toSerial.vMotor[2] = toSerial.vMotor[3] = 0;
  writeSerial();
  myTimer = millis();
}