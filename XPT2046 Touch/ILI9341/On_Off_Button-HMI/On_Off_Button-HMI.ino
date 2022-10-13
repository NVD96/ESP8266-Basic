
#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "XPT2046.h"  //CHU Y PHAI SETTUP PINMODE GIONG VOI CAC THU VIEN DA THEM VAO PROJECT

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>

#define STASSID "Nha nghi ca"
#define STAPSK  "t500vinhtan"

#define FIREBASE_HOST "hmi-iot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "5MqNajktZj4HZFweEjxYtZ9GaPb8tj7jtzePnpL8"

const char* ssid     = STASSID;
const char* password = STAPSK;

String path = "/";
FirebaseData fb;
FirebaseJson json;


XPT2046 XPT(0);   //CHU Y PHAI SETTUP PINMODE GIONG VOI CAC THU VIEN DA THEM VAO PROJECT
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

uint16_t m0;
String m1;
unsigned long t1 = 0;

// Comment out to stop drawing black spots
#define BLACK_SPOT

// Switch position and size
#define FRAME_X 100
#define FRAME_Y 150
#define FRAME_W 120
#define FRAME_H 50

// Red zone size
#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

// Green zone size
#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

uint8_t Tx[10];

void TxPLC();



void setup(void)
{
  Serial.begin(9600, SERIAL_7E1);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");

   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
   Firebase.reconnectWiFi(true);
   if(!Firebase.beginStream(fb, path))
   {
    Serial.println("RESON:" + fb.errorReason());
    }
  
 XPT.Touch_Init();  //CHU Y PHAI SETTUP PINMODE GIONG VOI CAC THU VIEN DA THEM VAO PROJECT
 
  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(1);

  // clear screen
  tft.fillScreen(TFT_BLUE);

  // Draw button (this example does not use library Button class)
  redBtn();
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void loop()
{

    
  if(millis() - t1 > 1000){
       
    if(Firebase.getString(fb, path + "/M0")){
    m1 = fb.stringData();    ///CHU Y KIEU DU LIEU
    m0 = m1.toInt();
        }
    
    if(m0 == 1) {

      TxPLC();
      greenBtn();
      }
    if(m0 == 0) {

      TxPLC();
      redBtn();
      }
    t1 = millis();
  }
    
  if((XPT.READZ1()+ 4095 - XPT.READZ2()) > 300){
    
      XPT.BYTERAC();
      uint16_t x = XPT.READX();
      uint16_t y = XPT.READY();
      delay(50);
      if(XPT.READZ1() < 20){
         if((y > 2300) && (y < 2850)){  //NHAN CHU ""OFF""
            Firebase.setString(fb, path + "/M0", 0);
            TxPLC();
            redBtn();
            
             }

      
         if((y > 1400) && (y < 2000)){  //NHAN CHU ""ON""
            Firebase.setString(fb, path + "/M0", 1);
            TxPLC();
            greenBtn();
            
             }
    
      }
  }
}
//------------------------------------------------------------------------------------------


void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, TFT_BLACK);
}

// Draw a red button
void redBtn()
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_DARKGREY);
  drawFrame();
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ON", GREENBUTTON_X + (GREENBUTTON_W / 2), GREENBUTTON_Y + (GREENBUTTON_H / 2));
  tft.fillCircle(160, 80, 50, TFT_SILVER);
}

// Draw a green button
void greenBtn()
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_DARKGREY);
  drawFrame();
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", REDBUTTON_X + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2));
  tft.fillCircle(160, 80, 50, TFT_YELLOW);
}


void TxPLC(){
  Tx[0] = 0x02;
  Tx[1] = 0x45;
  Tx[3] = 0x30;
  Tx[4] = 0x30;
  Tx[5] = 0x30;
  Tx[6] = 0x30;
  Tx[7] = 0x03;
  if(m0 ==1){
  Tx[2] = 0x37;
  Tx[8] = 0x33;
  Tx[9] = 0x46;
  }
  else {
  Tx[2] = 0x38;
  Tx[8] = 0x34;
  Tx[9] = 0x30;
    }
  for(uint8_t h = 0; h < 10; h++){
    
    Serial.write(Tx[h]);
    }
 }
