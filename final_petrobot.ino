#include <NewPing.h>
#include <U8g2lib.h>
#include <Wire.h>

#define ENA 14
#define ENB 12
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

#define TRIG 5
#define ECHO 18
#define IR_SURFACE 4
#define IR_DISPLAY 34

NewPing sonar(TRIG, ECHO, 200);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int speedValue = 64; // 25% of 255 ≈ 64
bool tryLeftFirst = true;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(IR_SURFACE, INPUT);
  pinMode(IR_DISPLAY, INPUT);

  u8g2.begin();
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);

  moveForward();
}

void loop() {
  handleEyes();
  int surface = digitalRead(IR_SURFACE);

  if (surface == LOW) {
    int distance = sonar.ping_cm();

    if (distance > 0 && distance < 20) {
      stopCar(); delay(300);

      moveReverse(); delay(800);
      stopCar(); delay(300);

      tryAlternativePath();
      moveForward();
    } else {
      moveForward();
    }
  } else {
    stopCar(); delay(300);

    moveReverse(); delay(800);
    stopCar(); delay(300);

    tryBothTurnsForSurface();
    moveForward();
  }
}

void tryAlternativePath() {
  if (tryLeftFirst) {
    turnLeft(); delay(600);
    if (sonar.ping_cm() < 20) {
      turnRight(); delay(1200);
    }
    tryLeftFirst = false;
  } else {
    turnRight(); delay(600);
    if (sonar.ping_cm() < 20) {
      turnLeft(); delay(1200);
    }
    tryLeftFirst = true;
  }
}

void tryBothTurnsForSurface() {
  // Try turning left first
  turnLeft(); delay(600);
  if (digitalRead(IR_SURFACE) == HIGH) {
    stopCar(); delay(300);
    turnRight(); delay(1200);
    stopCar(); delay(300);
  }
}

void moveForward() {
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveReverse() {
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// OLED Animation
enum EyeState { EYE_OPEN, EYE_CLOSE, EYE_IDLE };
EyeState eyeState = EYE_IDLE;
unsigned long previousMillis = 0;
const long interval1 = 1000;
const long interval2 = 300;

void handleEyes() {
  int read = digitalRead(IR_DISPLAY);
  unsigned long currentMillis = millis();

  if (read == 0) {
    eye_up();
    eyeState = EYE_IDLE;
    previousMillis = currentMillis;
  } else {
    switch (eyeState) {
      case EYE_IDLE:
        eye_open();
        previousMillis = currentMillis;
        eyeState = EYE_CLOSE;
        break;
      case EYE_CLOSE:
        if (currentMillis - previousMillis >= interval1) {
          eye_close();
          previousMillis = currentMillis;
          eyeState = EYE_OPEN;
        }
        break;
      case EYE_OPEN:
        if (currentMillis - previousMillis >= interval2) {
          eye_open();
          previousMillis = currentMillis;
          eyeState = EYE_IDLE;
        }
        break;
    }
  }
}

void eye_up() {
  u8g2.clearBuffer();
  u8g2.setBitmapMode(1);
  u8g2.drawBox(80, 12, 40, 20);
  u8g2.drawBox(10, 12, 40, 20);
  u8g2.drawFilledEllipse(99.5, 13, 19.5, 11);
  u8g2.drawFilledEllipse(29.5, 13, 19.5, 11);
  u8g2.sendBuffer();
}

void eye_open() {
  u8g2.clearBuffer(); 
  u8g2.setBitmapMode(1);
  u8g2.drawBox(14, 13, 36, 36);
  u8g2.drawBox(78, 13, 36, 36);
  u8g2.sendBuffer();
}

void eye_close() {
  u8g2.clearBuffer();
  u8g2.setBitmapMode(1);
  u8g2.drawBox(14, 27, 36, 7);
  u8g2.drawBox(78, 27, 36, 7);
  u8g2.sendBuffer();
}
