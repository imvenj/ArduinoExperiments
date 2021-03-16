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

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

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

void setup() {
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  
  delay(1); 
  
  Serial.write(0XA5); 
  Serial.write(0X83);    //初始化,连续输出模式
  Serial.write(0X28);    //初始化,连续输出模式
}

void loop() {
  serialEvent();
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

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.write(message);
  display.display();
}

void drawData() {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  Serial.println(pressure);
  Serial.println(humidity);
  Serial.println(alt);
  Serial.println(lux);
  Serial.println("-----");

  char n_str[15];
  char out[20];
  sprintf(out, "Temp: %s%cC", dtostrf(temp, 0, 1, n_str), (char)0xF8);
  Serial.println(out);
  display.println(out);
  sprintf(out, "Pres: %shPa", dtostrf(pressure, 0, 1, n_str));
  Serial.println(out);
  display.println(out);
  sprintf(out, "Humi: %s%%", dtostrf(humidity, 0, 1, n_str));
  Serial.println(out);
  display.println(out);
  sprintf(out, "Alti: %dM", alt);
  Serial.println(out);
  display.println(out);
  sprintf(out, "Brit: %sLx", dtostrf(lux, 0, 2, n_str));
  Serial.println(out);
  display.println(out);
  display.display();
}
