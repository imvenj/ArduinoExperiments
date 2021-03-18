// This Project is for ESP-32

#include <esp32-hal-timer.h>
#define SIG 18
#define INTP 19


volatile bool displayState = false;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  turnOff();
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

void setup() { 
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(OUTPUT, SIG);
  pinMode(INPUT, INTP);
  digitalWrite(SIG, HIGH);
  attachInterrupt(digitalPinToInterrupt(INTP), turnOn, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
   
}

void startTimer() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000000, true);
  timerAlarmEnable(timer);
}

void stopTimer() {
  timerEnd(timer);
}

void turnOn() {
  if (displayState) { return; }
  Serial.println("Turn on.");
  displayState = true;
  startTimer();
}

void turnOff() {
  Serial.println("Turn off.");
  displayState = false;
  stopTimer();
}
