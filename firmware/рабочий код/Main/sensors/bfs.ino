const int16_t queueLen = 200;
uint8_t mazeBfs[mazeSizeX][mazeSizeY][mazeSizeZ];

int16_t preobrI(int16_t a) {
  return (a % queueLen) + queueLen * (a % queueLen < 0);
}

int16_t intPow(int16_t base, int16_t intPow) {
  int16_t a = 1;
  for (intPow; intPow > 0; intPow--) {
    a *= base;
  }
  return a;
}

uint8_t getWall(int8_t xGet, int8_t yGet, int8_t zGet, int8_t myOrient) {
  myOrient = preobrOrient(myOrient);
  xGet = preobrX(xGet);
  yGet = preobrY(yGet);
  zGet = preobrZ(zGet);
  return maze[xGet][yGet][zGet] % intPow(2, myOrient + 1) / intPow(2, myOrient);
}

uint8_t getCeiling(int8_t xSearch, int8_t ySearch, int8_t zSearch, int8_t myOrient) {
  myOrient = (myOrient % 2) + 2 * (myOrient % 2 < 0);
  xSearch = preobrX(xSearch);
  ySearch = preobrY(ySearch);
  zSearch = preobrZ(zSearch);
  return maze[xSearch][ySearch][zSearch] % intPow(2, 5 + myOrient) / intPow(2, 4 + myOrient);
}

uint8_t getSearch(int8_t xSearch, int8_t ySearch, int8_t zSearch, int8_t myOrient, bool delta) {
  myOrient = preobrOrient(myOrient);
  xSearch = preobrX(xSearch);
  ySearch = preobrY(ySearch);
  zSearch = preobrZ(zSearch);
  return maze[preobrX(xSearch - (myOrient % 2) * (myOrient - 2) * delta)][preobrY(ySearch - ((myOrient + 1) % 2) * (myOrient - 1) * delta)][zSearch] % intPow(2, 7) / intPow(2, 6);
}

bool isSlide(int8_t xFunc, int8_t yFunc, int8_t zFunc) {
  xFunc = preobrX(xFunc);
  yFunc = preobrY(yFunc);
  zFunc = preobrZ(zFunc);
  int8_t myX, myY;
  for (int i = -1; i < 2; i++) {
    myX = preobrX(xFunc + i);
    for (int j = -1; j < 2; j++) {
      myY = preobrY(yFunc + j);
      for (int k = 0; k < 2; k++) {
        if (getCeiling(myX, myY, zFunc, k)) {
          return 1;
        }
      }
    }
  }
  return 0;
}

void setWall(int8_t set, int8_t xSet, int8_t ySet, int8_t zSet, int8_t myOrient) {
  myOrient = preobrOrient(myOrient);
  xSet = preobrX(xSet);
  ySet = preobrY(ySet);
  zSet = preobrZ(zSet);
  uint8_t zero1 = getWall(xSet, ySet, zSet, myOrient);
  uint8_t zero2 = getWall(preobrX(xSet - (myOrient % 2) * (myOrient - 2)), preobrY(ySet - ((myOrient + 1) % 2) * (myOrient - 1)), zSet, (myOrient + 2) % 4);
  maze[xSet][ySet][zSet] += (set - zero1) * intPow(2, myOrient);
  maze[preobrX(xSet - (myOrient % 2) * (myOrient - 2))][preobrY(ySet - ((myOrient + 1) % 2) * (myOrient - 1))][zSet] += (set - zero2) * intPow(2, (myOrient + 2) % 4);
}

void setCeiling(int8_t set, int8_t xSearch, int8_t ySearch, int8_t zSearch, int8_t myOrient) {
  myOrient = (myOrient % 2) + 2 * (myOrient % 2 < 0);
  xSearch = preobrX(xSearch);
  ySearch = preobrY(ySearch);
  zSearch = preobrZ(zSearch);
  uint8_t zero1 = getCeiling(xSearch, ySearch, zSearch, myOrient);
  uint8_t zero2 = getCeiling(xSearch, ySearch, preobrZ(zSearch + myOrient * 2 - 1), 1 - myOrient);
  maze[xSearch][ySearch][zSearch] += (set - zero1) * intPow(2, 4 + myOrient);
  maze[xSearch][ySearch][preobrZ(zSearch + myOrient * 2 - 1)] += (set - zero2) * intPow(2, 5 - myOrient);
}

void setSearch(int8_t set, int8_t xSearch, int8_t ySearch, int8_t zSearch, int8_t myOrient, bool delta) {
  myOrient = preobrOrient(myOrient);
  xSearch = preobrX(xSearch);
  ySearch = preobrY(ySearch);
  zSearch = preobrZ(zSearch);
  uint8_t zero = getSearch(xSearch, ySearch, zSearch, myOrient, delta);
  maze[preobrX(xSearch - (myOrient % 2) * (myOrient - 2) * delta)][preobrY(ySearch - ((myOrient + 1) % 2) * (myOrient - 1) * delta)][zSearch] += (set - zero) * intPow(2, 6);
}

uint8_t getMass(int8_t xGet, int8_t yGet, int8_t zGet, int8_t myOrient, bool delta) {
  xGet = preobrX(xGet);
  yGet = preobrY(yGet);
  zGet = preobrZ(zGet);
  return mazeBfs[preobrX(xGet - (myOrient % 2) * (myOrient - 2) * delta)][preobrY(yGet - ((myOrient + 1) % 2) * (myOrient - 1) * delta)][zGet];
}

void setMass(uint8_t set, int8_t xSet, int8_t ySet, int8_t zSet, int8_t myOrient, bool delta) {
  xSet = preobrX(xSet);
  ySet = preobrY(ySet);
  zSet = preobrZ(zSet);
  mazeBfs[preobrX(xSet - (myOrient % 2) * (myOrient - 2) * delta)][preobrY(ySet - ((myOrient + 1) % 2) * (myOrient - 1) * delta)][zSet] = set;
}

int8_t toNewPoint() {
  //Serial.println("aaa");
  for (int i = 0; i < mazeSizeX; i++) {
    for (int j = 0; j < mazeSizeY; j++) {
      for (int n = 0; n < mazeSizeZ; n++) {
        mazeBfs[i][j][n] = 0;
      }
    }
  }
  int16_t queueI = 0, nowI = 0;
  int8_t queue[queueLen][3];
  first_read();
  if (!lastPitch) {
    if (lasers[4] > lateralDist * 2 and !getWall(xRobot, yRobot, zRobot, orient + 1) and !getSearch(xRobot, yRobot, zRobot, orient + 1, 1)) {
      setWall(1, xRobot, yRobot, zRobot, orient + 1);
    }
    if (lasers[0] > minDist * 2 and !getWall(xRobot, yRobot, zRobot, orient) and !getSearch(xRobot, yRobot, zRobot, orient, 1)) {
      setWall(1, xRobot, yRobot, zRobot, orient);
    }
    if (lasers[3] > lateralDist * 2 and !getWall(xRobot, yRobot, zRobot, orient + 3) and !getSearch(xRobot, yRobot, zRobot, orient + 3, 1)) {
      setWall(1, xRobot, yRobot, zRobot, orient + 3);
    }
    if (lasers[5] > minDist * 2 + l3Dist and !getWall(xRobot, yRobot, zRobot, orient + 2) and !getSearch(xRobot, yRobot, zRobot, orient + 2, 1)) {
      setWall(1, xRobot, yRobot, zRobot, orient + 2);
    }
  } else {
    if (!getWall(xRobot, yRobot, zRobot, orient) and !getSearch(xRobot, yRobot, zRobot, orient, 1)) {
      setWall(1, xRobot, yRobot, zRobot, orient);
    }
  }
  int8_t isNew = 1;
  int8_t target[3] = { 0, 0, 0 };
  int8_t blackFlag = 0, bfsFlag = 1;
  const int16_t wayLen = 200;
  int8_t way[wayLen];
  for (int i = 0; i < wayLen; i++) {
    way[i] = 0;
  }
  int8_t nextX, nextY;
  int16_t wayI = wayLen - 1;
  int8_t nowPos[3];
  queue[queueI][0] = xRobot;
  queue[queueI][1] = yRobot;
  queue[queueI][2] = zRobot;
  setMass(2, xRobot, yRobot, zRobot, orient, 0);
  queueI = preobrI(queueI + 1);
  while (bfsFlag) {
    if (nowI != queueI) {
      //Serial.println(maze[queue[nowI][0]][queue[nowI][1]][queue[nowI][2]]);
      //Serial.println(String(queue[nowI][0]) + " " + String(queue[nowI][1]) + " " + String(queue[nowI][2]));
      for (int i = 0; i < 4; i++) {
        if (getWall(queue[nowI][0], queue[nowI][1], queue[nowI][2], i) and !getMass(queue[nowI][0], queue[nowI][1], queue[nowI][2], i, 1) and bfsFlag) {
          queue[queueI][0] = preobrX(queue[nowI][0] - (i % 2) * (i - 2));
          queue[queueI][1] = preobrY(queue[nowI][1] - ((i + 1) % 2) * (i - 1));
          queue[queueI][2] = queue[nowI][2];
          setMass(getMass(queue[nowI][0], queue[nowI][1], queue[nowI][2], i, 0) + 1, queue[queueI][0], queue[queueI][1], queue[queueI][2], i, 0);
          Serial.println(getMass(queue[nowI][0], queue[nowI][1], queue[nowI][2], i, 0));
          //Serial.println(String(queue[queueI][0]) + " " + String(queue[queueI][1]) + " " + String(queue[queueI][2]));
          //Serial.println(getSearch(queue[queueI][0], queue[queueI][1], queue[queueI][2], i, 0));
          Serial.println(String(queue[nowI][0]) + " " + String(queue[nowI][1]) + " " + String(queue[nowI][2]));
          if (!getSearch(queue[queueI][0], queue[queueI][1], queue[queueI][2], i, 0)) {
            target[0] = queue[queueI][0];
            target[1] = queue[queueI][1];
            target[2] = queue[queueI][2];
            bfsFlag = 0;
            Serial.println(String(queue[nowI][0]) + " " + String(queue[nowI][1]) + " " + String(queue[nowI][2]));
          }
          queueI = preobrI(queueI + 1);
        }
        //Serial.println(String(getWall(queue[nowI][0], queue[nowI][1], queue[nowI][2], i)) + " " + String(getMass(queue[nowI][0], queue[nowI][1], queue[nowI][2], i, 1)));
      }
      //Serial.println(maze[queue[nowI][0]][queue[nowI][1]][queue[nowI][2]]);
      for (int i = 0; i < 2; i++) {
        if (getCeiling(queue[nowI][0], queue[nowI][1], queue[nowI][2], i) and !getMass(queue[nowI][0], queue[nowI][1], preobrZ(queue[nowI][2] + i * 2 - 1), 0, 0) and bfsFlag) {
          queue[queueI][0] = queue[nowI][0];
          queue[queueI][1] = queue[nowI][1];
          queue[queueI][2] = preobrZ(queue[nowI][2] + i * 2 - 1);
          setMass(getMass(queue[nowI][0], queue[nowI][1], queue[nowI][2], i, 0) + 1, queue[queueI][0], queue[queueI][1], queue[queueI][2], 0, 0);
          if (!getSearch(queue[queueI][0], queue[queueI][1], queue[queueI][2], 0, 0)) {
            target[0] = queue[queueI][0];
            target[1] = queue[queueI][1];
            target[2] = queue[queueI][2];
            bfsFlag = 0;
            //Serial.println(String(queue[queueI][0]) + " " + String(queue[queueI][1]) + " " + String(queue[queueI][2]));
          }
          queueI = preobrI(queueI + 1);
        }
      }
      nowI = preobrI(nowI + 1);
    } else {
      isNew = 0;
      target[0] = 0;
      target[1] = 0;
      target[2] = 0;
      //Serial.println("aaa");
      bfsFlag = 0;
    }
  }
  nowPos[0] = target[0];
  nowPos[1] = target[1];
  nowPos[2] = target[2];
  bool flagWay = 1;
  while (1) {
    //Serial.println("aaa");
    for (int i = 0; i < 4; i++) {
      Serial.println(String(getMass(nowPos[0], nowPos[1], nowPos[2], i, 0) - 1) + " " + String(getMass(nowPos[0], nowPos[1], nowPos[2], i, 1)));
      if (getMass(nowPos[0], nowPos[1], nowPos[2], i, 0) - 1 == getMass(nowPos[0], nowPos[1], nowPos[2], i, 1) and getWall(nowPos[0], nowPos[1], nowPos[2], i) == 1 and flagWay) {
        nowPos[0] = preobrX(nowPos[0] - (i % 2) * (i - 2));
        nowPos[1] = preobrY(nowPos[1] - ((i + 1) % 2) * (i - 1));
        way[wayI] = preobrOrient(i + 2) + 1;
        wayI--;
        flagWay = 0;
      }
    }
    for (int i = 0; i < 2; i++) {
      if (getMass(nowPos[0], nowPos[1], nowPos[2], 0, 0) - 1 == getMass(nowPos[0], nowPos[1], preobrZ(nowPos[2] + i * 2 - 1), 0, 0) and getCeiling(nowPos[0], nowPos[1], nowPos[2], i) and flagWay) {
        nowPos[2] = preobrZ(nowPos[2] + i * 2 - 1);
        flagWay = 0;
      }
    }
    if (nowPos[0] == xRobot and nowPos[1] == yRobot and nowPos[2] == zRobot) {
      break;
    }
    flagWay = 1;
  }
  //Serial.println("aaa");
  for (int i = 0; i < wayLen; i++) {
    if (way[i]) {
      Serial.print("aaa ");
      Serial.println(way[i] - 1);
      turn(way[i] - 1);
      forward();
    }
  }
  nextX = preobrX(xRobot - orient % 2 * (orient - 2));
  nextY = preobrY(yRobot - (orient + 1) % 2 * (orient - 1));
  if (lastPitch and getCeiling(xRobot, yRobot, zRobot, (-lastPitch + 1) / 2) and getSearch(nextX, nextY, zRobot, orient, 0) and !getCeiling(nextX, nextY, zRobot, (lastPitch + 1) / 2)) {
    forward();
  }
  return isNew;
}