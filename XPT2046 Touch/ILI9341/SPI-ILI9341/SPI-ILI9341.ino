#include <Wire.h>
#include <SPI.h>

#define CS_PIN  15
#define DC_PIN  9
#define SPI_SETTING     SPISettings(16000000, MSBFIRST, SPI_MODE0)

#define ILI9341_TFTWIDTH 240  ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT 320 ///< ILI9341 max TFT height

#define ILI9341_NOP 0x00     ///< No-op register
#define ILI9341_SWRESET 0x01 ///< Software reset register
#define ILI9341_RDDID 0x04   ///< Read display identification information
#define ILI9341_RDDST 0x09   ///< Read Display Status

#define ILI9341_SLPIN 0x10  ///< Enter Sleep Mode
#define ILI9341_SLPOUT 0x11 ///< Sleep Out
#define ILI9341_PTLON 0x12  ///< Partial Mode ON
#define ILI9341_NORON 0x13  ///< Normal Display Mode ON

#define ILI9341_RDMODE 0x0A     ///< Read Display Power Mode
#define ILI9341_RDMADCTL 0x0B   ///< Read Display MADCTL
#define ILI9341_RDPIXFMT 0x0C   ///< Read Display Pixel Format
#define ILI9341_RDIMGFMT 0x0D   ///< Read Display Image Format
#define ILI9341_RDSELFDIAG 0x0F ///< Read Display Self-Diagnostic Result

#define ILI9341_INVOFF 0x20   ///< Display Inversion OFF
#define ILI9341_INVON 0x21    ///< Display Inversion ON
#define ILI9341_GAMMASET 0x26 ///< Gamma Set
#define ILI9341_DISPOFF 0x28  ///< Display OFF
#define ILI9341_DISPON 0x29   ///< Display ON

#define ILI9341_CASET 0x2A ///< Column Address Set
#define ILI9341_PASET 0x2B ///< Page Address Set
#define ILI9341_RAMWR 0x2C ///< Memory Write
#define ILI9341_RAMRD 0x2E ///< Memory Read

#define ILI9341_PTLAR 0x30    ///< Partial Area
#define ILI9341_VSCRDEF 0x33  ///< Vertical Scrolling Definition
#define ILI9341_MADCTL 0x36   ///< Memory Access Control
#define ILI9341_VSCRSADD 0x37 ///< Vertical Scrolling Start Address
#define ILI9341_PIXFMT 0x3A   ///< COLMOD: Pixel Format Set

#define ILI9341_FRMCTR1                                                        \
  0xB1 ///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2 0xB2 ///< Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3                                                        \
  0xB3 ///< Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR 0xB4  ///< Display Inversion Control
#define ILI9341_DFUNCTR 0xB6 ///< Display Function Control

#define ILI9341_PWCTR1 0xC0 ///< Power Control 1
#define ILI9341_PWCTR2 0xC1 ///< Power Control 2
#define ILI9341_PWCTR3 0xC2 ///< Power Control 3
#define ILI9341_PWCTR4 0xC3 ///< Power Control 4
#define ILI9341_PWCTR5 0xC4 ///< Power Control 5
#define ILI9341_VMCTR1 0xC5 ///< VCOM Control 1
#define ILI9341_VMCTR2 0xC7 ///< VCOM Control 2

#define ILI9341_RDID1 0xDA ///< Read ID 1
#define ILI9341_RDID2 0xDB ///< Read ID 2
#define ILI9341_RDID3 0xDC ///< Read ID 3
#define ILI9341_RDID4 0xDD ///< Read ID 4

#define ILI9341_GMCTRP1 0xE0 ///< Positive Gamma Correction
#define ILI9341_GMCTRN1 0xE1 ///< Negative Gamma Correction
//#define ILI9341_PWCTR6     0xFC

// Color definitions
#define ILI9341_BLACK 0x0000       ///<   0,   0,   0
#define ILI9341_NAVY 0x000F        ///<   0,   0, 123
#define ILI9341_DARKGREEN 0x03E0   ///<   0, 125,   0
#define ILI9341_DARKCYAN 0x03EF    ///<   0, 125, 123
#define ILI9341_MAROON 0x7800      ///< 123,   0,   0
#define ILI9341_PURPLE 0x780F      ///< 123,   0, 123
#define ILI9341_OLIVE 0x7BE0       ///< 123, 125,   0
#define ILI9341_LIGHTGREY 0xC618   ///< 198, 195, 198
#define ILI9341_DARKGREY 0x7BEF    ///< 123, 125, 123
#define ILI9341_BLUE 0x001F        ///<   0,   0, 255
#define ILI9341_GREEN 0x07E0       ///<   0, 255,   0
#define ILI9341_CYAN 0x07FF        ///<   0, 255, 255
#define ILI9341_RED 0xF800         ///< 255,   0,   0
#define ILI9341_MAGENTA 0xF81F     ///< 255,   0, 255
#define ILI9341_YELLOW 0xFFE0      ///< 255, 255,   0
#define ILI9341_WHITE 0xFFFF       ///< 255, 255, 255
#define ILI9341_ORANGE 0xFD20      ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define ILI9341_PINK 0xFC18        ///< 255, 130, 198

/*const uint8_t initcmd[] = {
  0xEF, 3, 0x03, 0x80, 0x02,
  0xCF, 3, 0x00, 0xC1, 0x30,
  0xED, 4, 0x64, 0x03, 0x12, 0x81,
  0xE8, 3, 0x85, 0x00, 0x78,
  0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  0xF7, 1, 0x20,
  0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F
};*/


void Start();
void Stop();
void CMD(uint8_t data);
void TxData(uint8_t data1);
void TxData16(uint16_t data2);
//void tft_init();
void tftbegin();
void addrWindow(uint16_t hang, uint16_t cot);
void drawPixel(uint16_t dhang, uint16_t dcot, uint16_t color);

void setup() {
  pinMode(CS_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
 Serial.begin(9600);
  SPI.begin();
  digitalWrite(CS_PIN, HIGH); 
  digitalWrite(DC_PIN, HIGH);
  CMD(ILI9341_SWRESET);
  //tft_init();
  tftbegin();
  CMD(ILI9341_SLPOUT);
  CMD(ILI9341_DISPON);
}


void loop() {
 for(int k = 0; k < 100; k++){
  drawPixel(k, 5, ILI9341_YELLOW);
  }
}

void Start(){
  SPI.beginTransaction(SPI_SETTING);
  digitalWrite(CS_PIN, LOW);
  }

void Stop(){
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  }

void CMD(uint8_t data){
  Start();
  digitalWrite(DC_PIN, LOW);
  SPI.transfer(data);
  digitalWrite(DC_PIN, HIGH);
  Stop();
  }

void TxData(uint8_t data1){
  Start();
  SPI.transfer(data1);
  Stop();
 }
 
void TxData16(uint16_t data2){
  uint8_t data2L, data2H; 
  data2H = data2>>8;
  data2L = data2;
  Start();
  SPI.transfer(data2H);
  SPI.transfer(data2L);
  Stop();
}

/*void tft_init(){
  uint16_t temp = 0;
  for(int i = 0; i<106; i++){
    CMD(initcmd[i]);
    temp = i + initcmd[i+1];
    for(int j = 0; j < initcmd[i+1]; j++){
      TxData(initcmd[i+j+2]);
      }
      i = temp;
    }
}*/

void tftbegin(){
  CMD(0xEF);
  TxData(0x03);
  TxData(0x80);
  TxData(0x02);

    CMD(0xCF);
  TxData(0x00);
  TxData(0xC1);
  TxData(0x30);

     CMD(0xED);
  TxData(0x64);
  TxData(0x03);
  TxData(0x12);
  TxData(0x81);

   CMD(0xE8);
  TxData(0x85);
  TxData(0x00);
  TxData(0x78);

  CMD(0xCB);
  TxData(0x39);
  TxData(0x2C);
  TxData(0x00);
  TxData(0x34);
  TxData(0x02);
  
    CMD(0xF7);
  TxData(0x20);

    CMD(0xEA);
  TxData(0x00);
  TxData(0x00);

  CMD(ILI9341_PWCTR1);
  TxData(0x23);

    CMD(ILI9341_PWCTR2);
  TxData(0x10);

    CMD(ILI9341_VMCTR1);
  TxData(0x3E);
  TxData(0x28);

     CMD(ILI9341_VMCTR2);
  TxData(0x86);
  
     CMD(ILI9341_MADCTL);
  TxData(0x48);

     CMD(ILI9341_VSCRSADD);
  TxData(0x00);
  
     CMD(ILI9341_PIXFMT);
  TxData(0x55);

    CMD(ILI9341_FRMCTR1);
  TxData(0x00);
  TxData(0x18);

     CMD(ILI9341_DFUNCTR);
  TxData(0x08);
  TxData(0x82);
  TxData(0x27);

      CMD(0xF2);
  TxData(0x00);

      CMD( ILI9341_GAMMASET);
  TxData(0x01);

    CMD(ILI9341_GMCTRP1);
  TxData(0x0F);
  TxData(0x31);
  TxData(0x2B);
  TxData(0x0C);
  TxData(0x0E);
  TxData(0x08);
  TxData(0x4E);
  TxData(0xF1);
  TxData(0x37);
  TxData(0x07);
  TxData(0x10);
  TxData(0x03);
  TxData(0x0E);
  TxData(0x09);
  TxData(0x00);

  CMD(ILI9341_GMCTRN1);
  TxData(0x00);
  TxData(0x0E);
  TxData(0x14);
  TxData(0x03);
  TxData(0x11);
  TxData(0x07);
  TxData(0x31);
  TxData(0xC1);
  TxData(0x48);
  TxData(0x08);
  TxData(0x0F);
  TxData(0x0C);
  TxData(0x31);
  TxData(0x36);
  TxData(0x0F);
  }

void addrWindow(uint16_t hang, uint16_t cot){
  CMD(ILI9341_CASET);
  TxData16(hang);
  TxData16(hang);
  CMD(ILI9341_PASET);
  TxData16(cot);
  TxData16(cot);
  CMD(ILI9341_RAMWR);
}

void drawPixel(uint16_t dhang, uint16_t dcot, uint16_t color){
  addrWindow(dhang, dcot);
  TxData16(color);
}
