const byte LED = 13;
const byte SW = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(SW, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  boolean val = digitalRead(SW);
  if (val) {
    digitalWrite(LED, HIGH);
  }
  else {
    Serial.println("Turned off.");
    digitalWrite(LED, LOW);
  }
}
