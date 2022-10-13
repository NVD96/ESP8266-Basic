
#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "XPT2046.h"  //CHU Y PHAI SETTUP PINMODE GIONG VOI CAC THU VIEN DA THEM VAO PROJECT

XPT2046 XPT(0);   //CHU Y PHAI SETTUP PINMODE GIONG VOI CAC THU VIEN DA THEM VAO PROJECT
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// This is the file name used to store the touch coordinate
// calibration data. Change the name to start a new calibration.
#define CALIBRATION_FILE "/TouchCalData3"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

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


void setup(void)
{
 XPT.Touch_Init();  //CHU Y PHAI SETTUP PINMODE GIONG VOI CAC THU VIEN DA THEM VAO PROJECT

  Serial.begin(9600);
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
  if((XPT.READZ1()+ 4095 - XPT.READZ2()) > 300){
    
      XPT.BYTERAC();
      uint16_t x = XPT.READX();
      uint16_t y = XPT.READY();
      delay(200);
      if(XPT.READZ1() < 20){
         if((y > 2300) && (y < 2850)){  //NHAN CHU ""OFF""

            redBtn();
             }

      
         if((y > 1400) && (y < 2000)){  //NHAN CHU ""ON""

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
