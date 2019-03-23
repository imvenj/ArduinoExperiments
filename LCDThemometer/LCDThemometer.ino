#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 2, 3, 4, 5, 6, 7, 8, 9);
int potPin = A5;
float temperature = 0;
long val = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.print("LM35 Thermometer");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(potPin);
  temperature = val * 2.5;
  
  lcd.clear();
  lcd.print("LM35 Thermometer");
  lcd.setCursor(0, 1);
  lcd.print((long)temperature / 10);
  lcd.print(".");
  lcd.print((long)temperature % 10);
  lcd.print((char)223);
  lcd.print("C");

  delay(2000);
}
