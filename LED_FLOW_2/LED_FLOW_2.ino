int startPin = 2;
int numberOfPins = 6;

void flashTwice() {
  for (int j = 0; j < 2; ++j) {
    for(int i = 0; i < numberOfPins; ++i) {
      digitalWrite(startPin+i, HIGH);
    }
    delay(200);
    for(int i = numberOfPins; i > 0; --i) {
      digitalWrite(startPin+i-1, LOW);
    }
    delay(200);
  }
}

void style1() {
  for(int i = 0; i < numberOfPins; ++i) {
    digitalWrite(startPin+i, HIGH);
    delay(200);
  }
  for(int i = numberOfPins; i > 0; --i) {
    digitalWrite(startPin+i-1, LOW);
    delay(200);
  }
}

void style2() {
  int k = 1;
  // Turn 34,25,16 on
  for(int i = 2; i >= 0; --i) {
    digitalWrite(startPin+i, HIGH);
    digitalWrite(startPin+i+k, HIGH);
    delay(400);
    k += 2;
  }
  k = 5;
  // Turn 16,25,34 off
  for(int i = 0; i < 3; ++i) {
    digitalWrite(startPin+i, LOW);
    digitalWrite(startPin+i+k, LOW);
    delay(400);
    k-=2;
  }
}

void style3() {
  int k = 5;
  // Turn 16,25,34 on and off
  for(int i = 0; i < 3; ++i) {
    digitalWrite(startPin+i, HIGH);
    digitalWrite(startPin+i+k, HIGH);
    delay(400);
    digitalWrite(startPin+i, LOW);
    digitalWrite(startPin+i+k, LOW);
    delay(400);
    k-=2;
  }
  k = 1;
  // Turn 34,25,16 on and off
  for(int i = 2; i >= 0; --i) {
    digitalWrite(startPin+i, HIGH);
    digitalWrite(startPin+i+k, HIGH);
    delay(400);
    digitalWrite(startPin+i, LOW);
    digitalWrite(startPin+i+k, LOW);
    delay(400);
    k += 2;
  }
}

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < numberOfPins; ++i) {
    pinMode(startPin+i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  style1();
  flashTwice();
  style2();
  flashTwice();
  style3();
  flashTwice();
}
