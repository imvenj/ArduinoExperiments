int buzzPin = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzPin, OUTPUT);
}

#define NTD1 350

#define NTD3 441

#define NTD5 525

#define NTDL5 265

void loop() {
  // put your main code here, to run repeatedly:
  tone(buzzPin, NTD3); delay(100);
  noTone(buzzPin); delay(100);
  
  tone(buzzPin, NTD3); delay(100);
  noTone(buzzPin); delay(300);
  
  tone(buzzPin, NTD3); delay(100);
  noTone(buzzPin); delay(300);
  
  tone(buzzPin, NTD1); delay(100);
  noTone(buzzPin); delay(100);
  
  tone(buzzPin, NTD3); delay(100);
  noTone(buzzPin); delay(300);
  
  tone(buzzPin, NTD5); delay(100);
  noTone(buzzPin); delay(500);

  tone(buzzPin, NTDL5); delay(100);
  noTone(buzzPin); delay(100);
  
  tone(buzzPin, NTDL5); delay(100);
  noTone(buzzPin); delay(1000);
}
