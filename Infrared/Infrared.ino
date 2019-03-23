#include <IRremote.h>

int recivePin = 11;
decode_results results;

IRrecv irrecv(recivePin);

int longPressCount = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)) {
    if (results.value != 0xFFFFFFFF) {
      longPressCount = 0;
      Serial.print("You pressed: ");
    }
    switch (results.value) {
    case 0xFFA25D:
        Serial.println("CH-");
        break;
    case 0xFF629D:
        Serial.println("CH");
        break;
    case 0xFFE21D:
        Serial.println("CH+");
        break;
    case 0xFF22DD:
        Serial.println("Fast backward");
        break;
    case 0xFF02FD:
        Serial.println("Fast forward");
        break;
    case 0xFFC23D:
        Serial.println("Play/Pause");
        break;
    case 0xFFE01F:
        Serial.println("-");
        break;
    case 0xFFA857:
        Serial.println("+");
        break;
    case 0xFF906F:
        Serial.println("EQ");
        break;
    case 0xFF6897:
        Serial.println("0");
        break;
    case 0xFF9867:
        Serial.println("100+");
        break;
    case 0xFFB04F:
        Serial.println("200+");
        break;
    case 0xFF30CF:
        Serial.println("1");
        break;
    case 0xFF18E7:
        Serial.println("2");
        break;
    case 0xFF7A85:
        Serial.println("3");
        break;
    case 0xFF10EF:
        Serial.println("4");
        break;
    case 0xFF38C7:
        Serial.println("5");
        break;
    case 0xFF5AA5:
        Serial.println("6");
        break;
    case 0xFF42BD:
        Serial.println("7");
        break;
    case 0xFF4AB5:
        Serial.println("8");
        break;
    case 0xFF52AD:
        Serial.println("9");
        break;
    case 0xFFFFFFFFL:
        longPressCount++;
        if (longPressCount == 5) {
          // Only sent once.
          Serial.println("long press detected.");
        }
        break;
    default:
        Serial.println("???");
        break;
    }
    irrecv.resume();
  }
}
