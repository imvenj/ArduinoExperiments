int buzzPin = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // 80ms 500Hz
  for(int i = 0; i < 80; ++i) {
    digitalWrite(buzzPin, HIGH);
    delay(1);
    digitalWrite(buzzPin, LOW);
    delay(1);
  }
  // 100ms 250Hz
  for(int i = 0; i < 100; ++i) {
    digitalWrite(buzzPin, HIGH);
    delay(2);
    digitalWrite(buzzPin, LOW);
    delay(2);
  }
}
