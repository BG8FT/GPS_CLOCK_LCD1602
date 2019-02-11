
#include <TinyGPS++.h>           // Include TinyGPS++ library
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>


const int rs = 6, en = 5, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
TinyGPSPlus gps;
SoftwareSerial SoftSerial(4, 3);    // Configure SoftSerial library


byte last_second;

int gpsYear;
int gpsYear1;
int gpsMonth;
int gpsDay;
int gpsHour;
int TimeZone = 8;                             // Timezone
String TimeStr;
String DateStr;
String SatStr;
int DaysAMonth[] = {                      //number of days a month
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


void setup(void) {

  SoftSerial.begin(9600);
  // Serial.begin(9600);

  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);

}

void loop() {

  while (SoftSerial.available() > 0) {

    if (gps.encode(SoftSerial.read())) {

      if (last_second != gps.time.second()) {
        last_second = gps.time.second();

        TimeStr = "";
        DateStr = "";
        SatStr = "";

        gpsYear = gps.date.year();
        gpsMonth = gps.date.month();
        gpsDay = gps.date.day();
        gpsHour = gps.time.hour();


        // leap year check  -------------------------------------------------------
        if (gps.date.year() % 4 == 0) DaysAMonth[1] = 29;
        else DaysAMonth[1] = 28;

        // time zone adjustment  --------------------------------------------------
        gpsHour += TimeZone;

        // daylight savings time adjustments  -------------------------------------

        if (gpsHour < 0)
        {
          gpsHour += 24;
          gpsDay -= 1;
          if (gpsDay < 1)
          {
            if (gpsMonth == 1)
            {
              gpsMonth = 12;
              gpsYear -= 1;
            }
            else
            {
              gpsMonth -= 1;
            }
            gpsDay = DaysAMonth[gpsMonth - 1];
          }
        }
        if (gpsHour >= 24)
        {
          gpsHour -= 24;
          gpsDay += 1;
          if (gpsDay > DaysAMonth[gpsMonth - 1])
          {
            gpsDay = 1;
            gpsMonth += 1;
            if (gpsMonth > 12) gpsYear += 1;
          }
        }



        gpsYear1 = gpsYear / 100 ;
        DateStr += gpsYear1;
        gpsYear = gpsYear % 100;
        if (gpsYear < 10) DateStr += "0";
        DateStr += gpsYear;
        DateStr += "/";
        if (gpsMonth < 10) DateStr += "0";
        DateStr += gpsMonth;
        DateStr += "/";
        if (gpsDay < 10) DateStr += "0";
        DateStr += gpsDay;


        if (gpsHour < 10) TimeStr = "0";
        TimeStr += gpsHour;
        TimeStr += ":";
        if (gps.time.minute() < 10) TimeStr += "0";
        TimeStr += gps.time.minute();
        TimeStr += ":";
        if (gps.time.second() < 10) TimeStr += "0";
        TimeStr += gps.time.second();
        if (gps.satellites.value() < 10)
        {
          SatStr += "0";
          SatStr += gps.satellites.value();
        }
        else SatStr += gps.satellites.value();



        lcd.setCursor(4, 0);
        lcd.print(TimeStr);
        //Serial.println(TimeStr);

        lcd.setCursor(14, 0);
        lcd.print(SatStr);
        lcd.setCursor(3, 1);
        lcd.print(DateStr);
        //Serial.println(DateStr);
      }


    }

  }
}
