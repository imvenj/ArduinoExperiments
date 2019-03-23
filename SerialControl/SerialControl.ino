int ledPin = 13;
int val;

void setup() {
  // put your setup code here, to run once:
 pinMode(ledPin, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = Serial.read();
  if (val != -1) {
    if (val == 'H') {
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      delay(500);
      Serial.print("Available: ");
      Serial.println(Serial.available(), DEC);
      Serial.print("Char: ");
      Serial.println(val);
    }
  }
}
