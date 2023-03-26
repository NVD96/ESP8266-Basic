// Program to exercise the MD_Parola library
//
// Demonstrates minimum required for sprite animated text.
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//
//SECS_IN_DAY               (86400L)
// SECS_IN_HR                (3600L)
// SECS_IN_MIN               (60L)
//define SEVENZYYEARS 2208988800UL
//timeOffset = 25200 = 7*3600 mui gio +7 UTC
//(((this->getEpochTime()  / 86400L) + 4 ) % 7); //0 is Sunday
//((this->getEpochTime()  % 86400L) / 3600); GIO
//((this->getEpochTime() % 3600) / 60);PHUT
//(this->getEpochTime() % 60);GIAY
//_currentEpoc = secsSince1900 - SEVENZYYEARS;
        /*getEpochTime()
          this->_timeOffset + // User offset
          this->_currentEpoc + // Epoch returned by the NTP server
          ((millis() - this->_lastUpdate) / 1000); // Time since last update
          */
/*
getDay()– trả về một số int tương ứng với ngày trong tuần (0 đến 6) bắt đầu từ Chủ nhật;

getHours()– trả về một số int với giờ hiện tại (0 đến 23) ở định dạng 24 giờ;

getMinutes()– trả về một số int với số phút hiện tại (0 đến 59);

getSeconds()– trả về một số int với giây hiện tại;

getEpochTime()– trả về một khoảng thời gian dài không dấu với thời gian kỷ nguyên (số giây đã trôi qua kể từ ngày 1 tháng 1 năm 1970 (nửa đêm GMT);
*/

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


const char *ssid     = "Nha nghi ca";
const char *password = "t500vinhtan";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200);

//Week Days
String weekDays[7]={"CN", "Thứ 2", "Thứ 3", "Thứ 4", "Thứ 5", "Thứ 6", "Thứ 7"};

//Month names
String months[13]={"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};

String gio;
String phut;
String giay;

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    D8

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup(void)
{
    Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  
  P.begin();
  P.setIntensity(0);
  P.displayClear();
}

void loop(void)
{
  timeClient.update();
 // Serial.println((timeClient.getEpochTime()/(86400*365) + 1970)) ;

///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime); 
    
      int monthDay = ptm->tm_mday;
      Serial.print("Month day: ");
      Serial.println(monthDay);
    
      int currentMonth = ptm->tm_mon+1;
      Serial.print("Month: ");
      Serial.println(currentMonth);
    
      String currentMonthName = months[currentMonth];
      Serial.print("Month name: ");
      Serial.println(currentMonthName);
    
      int currentYear = ptm->tm_year+1900;
      Serial.print("Year: ");
      Serial.println(currentYear);
    
      //Print complete date:
      String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
      Serial.print("Current date: ");
      Serial.println(currentDate);
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
   if((timeClient.getSeconds() % 5) == 0){
    
     P.setTextAlignment(PA_CENTER);
     P.print(weekDays[timeClient.getDay()]);
     delay(1000);
    }


  if (timeClient.getHours() > 9){
   gio = String(timeClient.getHours());
    }
   else {
    gio = "0" + String(timeClient.getHours());
    }
    
  if (timeClient.getMinutes() > 9){
   phut = String(timeClient.getMinutes());
    }
   else {
    phut = "0" + String(timeClient.getMinutes());
    }

  if (timeClient.getSeconds() > 9){
   giay = String(timeClient.getSeconds());
    }
   else {
    giay = "0" + String(timeClient.getSeconds());
    }

  String thoigian1 = gio + ":" + phut;
  String thoigian2 = gio + " " + phut;
  
  P.setTextAlignment(PA_CENTER);
  P.print(thoigian1);
  //P.print(timeClient.getFormattedTime());
  delay(500);
  P.print(thoigian2);
  delay(500);
}
