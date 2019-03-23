int ledPin = 3;
int analogPin = A5;
int val = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(analogPin);
  analogWrite(ledPin, val / 4);
}
