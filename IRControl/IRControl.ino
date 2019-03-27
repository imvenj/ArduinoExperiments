#include <IRremote.h>
#include <Servo.h>
int servoPin = 9;
int recivePin = 11;
int ledPin = 7;
decode_results results;
static boolean clw = false;

IRrecv irrecv(recivePin);
Servo myServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(servoPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  irrecv.enableIRIn();
  myServo.attach(servoPin);
}

void ledBlink(byte n) {
  for(byte i = 0; i < n; ++i) {
    digitalWrite(ledPin, HIGH);
    delay(500 / n);
    digitalWrite(ledPin, LOW);
    delay(500 / n);
  }
}

void rotate(byte n, boolean clw) {
  int currentAngle = myServo.read();
  int dir = clw ? 1 : -1;
  int angle = currentAngle + n * dir * 10;
  Serial.print("current angle: ");
  Serial.print(angle);
  Serial.print("Current direction: ");
  Serial.println(clw ? "cw" : "ccw");
  if ((currentAngle <= 180 && angle > 180) || (currentAngle >=  0 && angle < 0)) {
    ledBlink(3); 
    return;
  }
  if (angle > 180) { angle = 180; }
  if (angle < 0) { angle = 0; ledBlink(3); }
  ledBlink(1);
  myServo.write(angle);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (irrecv.decode(&results)) {
    switch (results.value) {
    case 0xFFA25D:
        Serial.println("CH-");
        break;
    case 0xFF629D:
        Serial.println("CH");
        break;
    case 0xFFE21D:
        Serial.println("CH+");
        break;
    case 0xFF22DD:
        Serial.println("Fast backward");
        ledBlink(2);
        myServo.write(0);
        break;
    case 0xFF02FD:
        Serial.println("Fast forward");
        ledBlink(2);
        myServo.write(180);
        break;
    case 0xFFC23D:
        Serial.println("Play/Pause");
        break;
    case 0xFFE01F:
        Serial.println("-");
        ledBlink(1);
        clw = false;
        break;
    case 0xFFA857:
        Serial.println("+");
        ledBlink(1);
        clw = true;
        break;
    case 0xFF906F:
        Serial.println("EQ");
        break;
    case 0xFF6897:
        Serial.println("0");
        ledBlink(1);
        break;
    case 0xFF9867:
        Serial.println("100+");
        break;
    case 0xFFB04F:
        Serial.println("200+");
        break;
    case 0xFF30CF:
        Serial.println("1");
        rotate(1, clw);
        break;
    case 0xFF18E7:
        Serial.println("2");
        rotate(2, clw);
        break;
    case 0xFF7A85:
        Serial.println("3");
        rotate(3, clw);
        break;
    case 0xFF10EF:
        Serial.println("4");
        rotate(4, clw);
        break;
    case 0xFF38C7:
        Serial.println("5");
        rotate(5, clw);
        break;
    case 0xFF5AA5:
        Serial.println("6");
        rotate(6, clw);
        break;
    case 0xFF42BD:
        Serial.println("7");
        rotate(7, clw);
        break;
    case 0xFF4AB5:
        Serial.println("8");
        rotate(8, clw);
        break;
    case 0xFF52AD:
        Serial.println("9");
        rotate(9, clw);
        break;
    case 0xFFFFFFFFL:
        break;
    default:
        break;
    }
    irrecv.resume();
  }
}
