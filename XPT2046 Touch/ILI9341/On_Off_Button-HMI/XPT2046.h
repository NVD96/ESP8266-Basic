#ifndef XPT2046_h
#define XPT2046_h
#include <Stream.h>
//CHU Y PHAI SETTUP PINMODE GIONG VOI CAC THU VIEN DA THEM VAO PROJECT
#define T_OUT 16
#define T_CS 4
#define T_DIN 5
#define T_CLK 10

class XPT2046 {
public:
  XPT2046(int);
  uint8_t spi_8bit(uint8_t data);
  uint16_t spi_16bit(uint16_t data1);
  uint16_t READZ1();
  uint16_t READZ2();
  void BYTERAC();
  uint16_t READX();
  uint16_t READY();
  void Touch_Init();
  
};
#endif //XPT2046_h
