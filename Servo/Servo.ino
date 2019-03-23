
// Test A
#include <Servo.h>
Servo myServo;
#define potPin A5

int val;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(potPin);
  val = map(val, 0, 1023, 0, 179);
  myServo.write(val);
  delay(15);
}

/*
// Test B
#include <Servo.h>
int servoPin = 5;
Servo myServo;

void setup() {
  pinMode(servoPin, OUTPUT);
  myServo.attach(servoPin);
}

void loop() {
  myServo.write(179);
  delay(1000);
  myServo.write(0);
  delay(1000);
}
*/
