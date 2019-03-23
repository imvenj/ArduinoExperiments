boolean table[10][8] = {
  { 0, 0, 0, 1, 1, 0, 0, 0 }, //0
  { 0, 1, 1, 1, 1, 1, 1, 0 }, //1
  { 0, 0, 1, 0, 1, 0, 0, 1 }, //2
  { 0, 0, 1, 0, 1, 1, 0, 0 }, //3
  { 0, 1, 0, 0, 1, 1, 1, 0 }, //4
  { 1, 0, 0, 0, 1, 1, 0, 0 }, //5
  { 1, 0, 0, 0, 1, 0, 0, 0 }, //6
  { 0, 0, 1, 1, 1, 1, 1, 0 }, //7
  { 0, 0, 0, 0, 1, 0, 0, 0 }, //8
  { 0, 0, 0, 0, 1, 1, 0, 0 }, //9
};

void turnOff() {
  for (int i = 2; i < 10; ++i) {
    digitalWrite(i, HIGH); // Turn off all lights;
  }
}

void turnOn() {
  for (int i = 2; i < 10; ++i) {
    digitalWrite(i, LOW); // Turn off all lights;
  }
}

void initialize() {
  for (int i = 2; i < 10; ++i) {
    pinMode(i, OUTPUT); // Set Output
  }
}

void showDigit(int digit) {
  if (digit > 9 || digit < 0) { return; }
  for (int i = 0; i < 8; ++i) {
    if (table[digit][i] == 0) {
      digitalWrite(i+2, LOW);
    }
    else {
      digitalWrite(i+2, HIGH);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initialize();
  turnOff();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(unsigned char i = 0; i < 10; ++i) {
    showDigit(i);
    delay(500);
    turnOff();
  }
//    for (int i = 2; i<10;++i) {
//      turnOff();
//      digitalWrite(i, LOW);
//      int j = i + 1;
//      if (j >= 10) { j -= 8; }
//      digitalWrite(j, LOW);
//      delay(1000);
//    }
}
