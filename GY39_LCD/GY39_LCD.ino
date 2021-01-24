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
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned char Re_buf[15],counter=0;
unsigned char sign=0;
typedef struct
{
    uint32_t P;
    uint16_t Temp;
    uint16_t Hum;
    uint16_t Alt;          
} bme;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);  
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
                uint16_t temp = Bme.Temp/100;
                uint16_t pressure = Bme.P/10000;
                uint16_t humidity = Bme.Hum/100;
                // uint16_t alt = Bme.Hum/100;
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
                lcd.setCursor(0, 0);
                char out[17];
                sprintf(out, "%2dC,%4dH,%3d%%  ", temp, pressure, humidity);
                lcd.print(out);
            }       
        }  else if (ch == 0x4) {
            for(i=0;i<8;i++) {
              sum+=Re_buf[i];
            }
            if(sum==Re_buf[i]) //检查帧头，帧尾
            {
                data_16[0]=(Re_buf[4]<<8)|Re_buf[5];
                data_16[1]=(Re_buf[6]<<8)|Re_buf[7];
                uint32_t lux = (((uint32_t)data_16[0])<<16)|data_16[1];
                lux = lux/100;
                /*
                Serial.print("Lightness: ");
                Serial.print(lux);
                Serial.println(" lux");*/
                lcd.setCursor(0, 1);
                char out[17];
                sprintf(out, "%d LUX     ", lux);
                lcd.print(out);
            }
        } else { // 未知数据
            /*
            Serial.println();
            Serial.print("Unknown frame data length: ");
            Serial.println(ch);
            Serial.println();
            */
            lcd.clear();
            lcd.print("Unknown frame!");
        }

        sign=0;
    }
}
