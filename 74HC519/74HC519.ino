int latchPin = 8; // st_cp
int clockPin = 12; // sh_cp
int dataPin = 11; // ds

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte a = 0;
  while (true) {
    Serial.println(a);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, a);
    digitalWrite(latchPin, HIGH);
    delay(1000);
    a++;
  }
}
