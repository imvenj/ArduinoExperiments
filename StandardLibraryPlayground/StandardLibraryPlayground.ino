#define BUF 1024
const char *greet = "Hello, ";
const char *myName = "arduino.";

void setup() {
  Serial.begin(9600);
}

void loop() {
  char *message = (char *)malloc(sizeof(char) * BUF);
  strncat(message, greet, 512);
  strncat(message, myName, 512);
  Serial.println(message);
  free(message);
  delay(5000);
}
