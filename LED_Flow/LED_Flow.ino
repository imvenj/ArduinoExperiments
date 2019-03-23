int startPin = 2;
int numberOfPins = 6;
int index = 0;

enum MODE {
  PREPARE = 0,
  SETLOW,
  SETHIGH
};

void enumertatePins(MODE mode) {
  for(int i = startPin; i < startPin + numberOfPins; ++i) {
    switch(mode) {
      case PREPARE:
        pinMode(i, OUTPUT);
        break;
      case SETLOW:
        digitalWrite(i, LOW);
        break;
      case SETHIGH:
        digitalWrite(i, HIGH);
        break;
      default:
        break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  enumertatePins(PREPARE);
}

void loop() {
  // put your main code here, to run repeatedly:
  enumertatePins(SETLOW);
  digitalWrite(startPin + index, HIGH);
  index = (index + 1) % numberOfPins;
  delay(100);
}
