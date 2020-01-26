int middleDistanceSensor = A0;
int rightDistanceSensor = A2;

int E1 = 11;
int M1 = 10;
int E2 = 9;
int M2 = 8;

void setup() {
  Serial.begin(9600);
  pinMode(middleDistanceSensor, INPUT);
  pinMode(rightDistanceSensor, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
}


void loop() {
  int middleSign = analogRead(middleDistanceSensor);
  goForward(80);
  //Serial.println(middleSign);
  if (middleSign > 400) {
    stopMotor();
    //delay(10000);
    int val = turnUntilMaxVal();
    goAroundObject(val);
    delay(10000);
  }

}

int turnUntilMaxVal() {
  //Search maximum value
  int maxVal = analogRead(rightDistanceSensor);
  while (true) {
    turnLeft(80);
    delay(200);
    int val = analogRead(rightDistanceSensor);
    //Serial.println(maxVal);
    if (val > maxVal) {
      maxVal = val;
    }
    Serial.println(val);
    //Local max found, rotate
    if (maxVal - 280 > val) {
      //go back to loal max
      while (true) {
        turnRight(80);
        delay(200);
        if (analogRead(rightDistanceSensor) > maxVal - 40 && analogRead(rightDistanceSensor) < maxVal + 40) {
          stopMotor();
          Serial.println(0);
          Serial.println(analogRead(rightDistanceSensor));
          delay(10000);
          return analogRead(rightDistanceSensor);
        }
      }
    }
  }
}

void goAroundObject(int distance) {
  while (true) {
    int v = p(0.7, distance, analogRead(rightDistanceSensor));
    goForward(180);
    if ( v < 0) {
      Serial.println(v);
      turnRight(v);
      delay(100);
    } else {
      Serial.println(v);
      turnLeft(v);
      delay(100);
    }
  }

}

void goForward(int v) {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  analogWrite(E1, v);
  analogWrite(E2, v);
}

void goBackward(int v) {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, v);
  analogWrite(E2, v);
}

void turnRight(int v) {
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  analogWrite(E1, v);
  analogWrite(E2, v);
}

void turnLeft(int v) {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(E1, v);
  analogWrite(E2, v);
}

void stopMotor() {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, 0);
  analogWrite(E2, 0);
}

void keepDistance() {
  int v = p(0.5, 250, analogRead(middleDistanceSensor));

  if ( v < 0) {
    goBackward(v);
  } else {
    goForward(v);
  }
}

int p(float k, int setPoint, int reading) {
  int v = k * (setPoint - reading);
  return v;
}
