// 设定SR04连接的Arduino引脚
const int GRDPin = 2;
const int VCCPin = 5;
const int TrigPin = 4;
const int EchoPin = 3;
unsigned long distance;

void setup()
{ 
  Serial.begin(9600);
  pinMode(GRDPin, OUTPUT);
  pinMode(VCCPin, OUTPUT);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  digitalWrite(GRDPin, LOW);
  digitalWrite(VCCPin, HIGH);
  Serial.println("Ultrasonic sensor:");
}
void loop()
{
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  distance = pulseIn(EchoPin, HIGH) / 58.00;
  Serial.print(distance);
  Serial.print("cm");
  Serial.println();
  delay(1000);
}
