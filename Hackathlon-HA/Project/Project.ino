// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

#define TEMP_DATA (0)
#define HUM_DATA (1)
#define PRES_DATA (2)

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
char mystr[12];

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light
  Serial.begin(38400);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
}

void loop () {
    static int newS = 0;
    static char print_data = 0;
    unsigned char workInt = 0;
    DateTime now = rtc.now();
    int s = now.second();
    if (newS != s) {
        newS = s;
//        Serial.print(now.hour(), DEC);
//        Serial.print(':');
//        Serial.print(now.minute(), DEC);
//        Serial.print(':');
//        Serial.print(now.second(), DEC);
//        Serial.println("  x");
        lcd.setCursor(0,0);
        if(s==0){
          //lcd.clear();
          lcd.print("                ");
          lcd.setCursor(0,0);
        }
        lcd.print((String)"Time: "+now.hour()+":"+now.minute()+":"+now.second());
        Serial.write('X');
        delay(2);
        Serial.readBytes(mystr,12); //Read the serial data and store in var
        if((s % 5)==0){
          lcd.setCursor(0,1);
          lcd.print("                ");
          lcd.setCursor(0,1);
          switch(print_data){
            case TEMP_DATA:
              lcd.print("T="); 
              lcd.print((mystr[1]/10)); lcd.print((mystr[1]%10));
              lcd.print(".");
              lcd.print((mystr[2]/10)); lcd.print((mystr[2]%10));
              lcd.print(" *C");
              print_data++;
            break;

            case HUM_DATA:
              lcd.print("H="); 
              lcd.print((mystr[5]/10)); lcd.print((mystr[5]%10));
              lcd.print(".");
              lcd.print((mystr[6]/10)); lcd.print((mystr[6]%10));
              lcd.print(" %");
              print_data++;
            break;
            
            case PRES_DATA:
              lcd.print("P= 0."); 
              lcd.print((mystr[9]/10)); lcd.print((mystr[9]%10));
              lcd.print((mystr[10]/10)); lcd.print((mystr[10]%10)); 
              lcd.print(" Atm");
              print_data=0;
            break;

            default:
              lcd.print("Error with AC :-(");
            break;
          }
       }
    }
}
