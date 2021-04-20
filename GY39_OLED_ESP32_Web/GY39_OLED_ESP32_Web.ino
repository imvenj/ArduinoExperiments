/////////////////////
/*
GY39----MINI
VCC----VCC
GND----GND
1:RX---TX,send A5 02 A7 to GY-39
2:TX---RX
3:MINI_TX---FT232_RX
*/
//////////////////

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ticker.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SDA 21
#define SCL 22
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned char Re_buf[15],counter=0;
unsigned char sign=0;
typedef struct
{
    uint32_t P;
    uint16_t Temp;
    uint16_t Hum;
    uint16_t Alt;          
} bme;


float temp = 0.0f;
float pressure = 0.0f;
float humidity = 0.0f;
uint16_t alt = 0;
float lux = 0.0f;

#include <esp32-hal-timer.h>
#define INTP 19

volatile bool displayState = true;

// If you want custom output interval set to 0
// FIXME: when set to zero, strange non output bug occur.
#define CONTINUIOUS_SENSOR_OUTPUT 1
// Set custom output interval to 1500 ms
#define SENSOR_OUTPUT_INTERVAL 1500
#define OLED_ON_INTERVAL 20000000
#define WIFI_OUTPUT_INTERVAL 10000

hw_timer_t * timer0 = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer0() {
  portENTER_CRITICAL_ISR(&timerMux0);
  turnOff();
  portEXIT_CRITICAL_ISR(&timerMux0);
}

void setupButtonInterrupt() {
  pinMode(INTP, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTP), turnOn, CHANGE);
}

void startTimer0() {
  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &onTimer0, true);
  timerAlarmWrite(timer0, OLED_ON_INTERVAL, true);
  timerAlarmEnable(timer0);
}

void stopTimer0() {
  timerEnd(timer0);
}

void turnOn() {
  if (displayState) { return; }
  Serial.println("Turn on.");
  displayState = true;
  startTimer0();
}

void turnOff() {
  Serial.println("Turn off.");
  displayState = false;
  stopTimer0();
}

Ticker sendDataTicker;

char weather_data[100];
void sendDataWrapper() {
  sendDataToServer(weather_data);
}

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiMulti.h>
#include <WebServer.h>

WiFiMulti WiFiMulti;
WiFiClient client;
IPAddress server(192,168,31,155);
char ssid[] = "Pangea";
char pass[] = "fuckgfww";

void setupWifi() {
  // We start by connecting to a WiFi network
  WiFiMulti.addAP(ssid, pass);

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  drawErrorMessage("Waiting for Wifi...");

  while(WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }

  drawErrorMessage("Wifi connected!");
  delay(500);
  
  startTimer0();
  // Send data
  sendDataTicker.attach_ms(WIFI_OUTPUT_INTERVAL, sendDataWrapper);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendDataToServer(const char *weather_info) {
  if (client.connect(server, 4567)) {
    // Serial.println("server connected");
    // Make a HTTP request:
    char req_data[124];
    sprintf(req_data, "GET /send?data=%s HTTP/1.1", weather_info);
    client.println(req_data);
    client.println("Host: venj.me");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
}

void setupSensor() {
  Serial.write(0XA5);
  #if CONTINUIOUS_SENSOR_OUTPUT
  Serial.write(0X83);    //初始化,连续输出模式
  Serial.write(0X28);    //初始化,连续输出模式
  #else
  Serial.write(0X00); 
  Serial.write(0XA5);
  #endif
}

void setupDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  delay(1);
}

void setup() {
  Serial.begin(9600);
  setupSensor();
  setupButtonInterrupt();
  setupDisplay();
  setupWifi();
}

void loop() {
  #if CONTINUIOUS_SENSOR_OUTPUT
  serialEvent();
  #else
  Serial.write(0XA5);
  Serial.write(0X51);
  Serial.write(0XF6);
  delay(50);
  serialEvent();
  Serial.write(0XA5);
  Serial.write(0X52);
  Serial.write(0XF7);
  serialEvent();
  delay(SENSOR_OUTPUT_INTERVAL);
  #endif
}

void serialEvent() {
    while (Serial.available()) {
        Re_buf[counter] =(unsigned char)Serial.read();
        if (counter==0&&Re_buf[0]!=0x5A) { return; } // 检查帧头
        
        counter++;
        if (counter < 4) {
            continue;
        }
        
        if (counter==(5+Re_buf[3])) //接收到数据
        {    
            counter=0; //重新赋值，准备下一帧数据的接收 
            sign=1;
            output();
            #if !CONTINUIOUS_SENSOR_OUTPUT
            break;
            #endif
        }
    }
}

void output() {
    unsigned char i=0,sum=0;
    uint16_t data_16[2]={0};
    bme Bme;
    if(sign)
    {
        unsigned char ch = Re_buf[3];
        if (ch == 0xA) { // 温度、湿度、压强、海拔
            for(i=0;i<14;i++) {
                sum+=Re_buf[i];
            }
            if(sum==Re_buf[i]) //检查帧头，帧尾
            {
                Bme.Temp=(Re_buf[4]<<8)|Re_buf[5];
                data_16[0]=(Re_buf[6]<<8)|Re_buf[7];
                data_16[1]=(Re_buf[8]<<8)|Re_buf[9];
                Bme.P=(((uint32_t)data_16[0])<<16)|data_16[1];
                Bme.Hum=(Re_buf[10]<<8)|Re_buf[11];
                Bme.Alt=(Re_buf[12]<<8)|Re_buf[13];
                temp = (float)Bme.Temp/100.0;
                pressure = (float)Bme.P/10000.0;
                humidity = (float)Bme.Hum/100.0;
                alt = Bme.Alt;
                /*
                Serial.print("Temp: ");
                Serial.print( (float)Bme.Temp/100);
                Serial.print(" DegC  PRESS : ");
                Serial.print( ((float)Bme.P)/100);
                Serial.print(" Pa  HUM : ");   
                Serial.print( (float)Bme.Hum/100);
                Serial.print(" % ALT:");
                Serial.print( (float)Bme.Hum/100);
                Serial.println("m");*/
                drawData();
            }       
        }  else if (ch == 0x4) {
            for(i=0;i<8;i++) {
              sum+=Re_buf[i];
            }
            if(sum==Re_buf[i]) //检查帧头，帧尾
            {
                data_16[0]=(Re_buf[4]<<8)|Re_buf[5];
                data_16[1]=(Re_buf[6]<<8)|Re_buf[7];
                uint32_t luxI = (((uint32_t)data_16[0])<<16)|data_16[1];
                lux = (float)luxI/100.0;
                drawData();
            }
        } else { // 未知数据
          drawErrorMessage("Unknown data.");
        }
        
        sign=0;
    }
}

void drawErrorMessage(const char *message) {
  display.clearDisplay();
  if (!displayState) { // Turn off display
    display.display();
    return;
  }

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.write(message);
  display.display();
}

void drawData() {
  // Format data.
  char n_str[15];
  char temp_str[15];
  char pres_str[15];
  char humi_str[15];
  char alti_str[15];
  char brit_str[15];
  sprintf(temp_str, "%s", dtostrf(temp, 0, 1, n_str));
  sprintf(pres_str, "%s", dtostrf(pressure, 0, 1, n_str));
  sprintf(humi_str, "%s", dtostrf(humidity, 0, 1, n_str));
  sprintf(alti_str, "%d", alt);
  sprintf(brit_str, "%s", dtostrf(lux, 0, 2, n_str));
  sprintf(weather_data, "%s,%s,%s,%s,%s", temp_str, pres_str, humi_str, alti_str, brit_str);

  display.clearDisplay();

  if (!displayState) { // Turn off display
    display.display();
    return;
  }

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  
  char out[20];
  sprintf(out, "Temp: %s%cC", temp_str, (char)0xF8);
  // Serial.println(out);
  display.println(out);
  sprintf(out, "Pres: %shPa", pres_str);
  // Serial.println(out);
  display.println(out);
  sprintf(out, "Humi: %s%%", humi_str);
  // Serial.println(out);
  display.println(out);
  sprintf(out, "Alti: %dM", alt);
  // Serial.println(out);
  display.println(out);
  sprintf(out, "Brit: %sLx", brit_str);
  // Serial.println(out);
  display.println(out);
  display.display();
}
