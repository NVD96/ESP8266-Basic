#include "XPT2046.h"
#include <Arduino.h>
//CHU Y PHAI SETTUP PINMODE GIONG VOI CAC THU VIEN DA THEM VAO PROJECT
XPT2046::XPT2046(int){
  
  }
void XPT2046::Touch_Init()
{
pinMode(T_OUT, INPUT);
pinMode(T_CS, OUTPUT);
pinMode(T_DIN, OUTPUT);
pinMode(T_CLK, OUTPUT);
digitalWrite(T_CLK, LOW);
digitalWrite(T_CS, HIGH);

}


uint8_t XPT2046::spi_8bit(uint8_t data){
  uint8_t giatri = 0;
  
  for(byte i =0; i<8; i++){
    digitalWrite(T_CLK, LOW);
    if((data & 0x80) == 0x80){
      digitalWrite(T_DIN, HIGH);
      }
      else digitalWrite(T_DIN, LOW);
      giatri = giatri | digitalRead(T_OUT);
      digitalWrite(T_CLK, HIGH);
      data = data<<1;
      giatri = giatri<<1;
    }
  return giatri;
  }


 uint16_t XPT2046::spi_16bit(uint16_t data1){
  uint16_t gt;
  uint8_t data1L, data1H;
  data1H = data1>>8;
  data1L = data1;
  gt = spi_8bit(data1H);
  gt = gt <<8;
  gt = gt | spi_8bit(data1L);
  return gt;
  }


 uint16_t XPT2046::READZ1(){
  uint16_t z1;
  digitalWrite(T_CS, LOW);
  spi_8bit(0xB1);
  z1 = spi_16bit(0x0000);
  z1 = z1>>4;               //Vi funtion spi_8bit dich 8 bit nen no se READ FULL 16bit DATA
  digitalWrite(T_CS, HIGH);
  return z1;
  }

   uint16_t XPT2046::READZ2(){
  uint16_t z2;
  digitalWrite(T_CS, LOW);
  spi_8bit(0xC1);
  z2 = spi_16bit(0x0000);
  z2 = z2>>4;               //Vi funtion spi_8bit dich 8 bit nen no se READ FULL 16bit DATA
  digitalWrite(T_CS, HIGH);
  return z2;
  }


void XPT2046::BYTERAC(){
  digitalWrite(T_CS, LOW);
  spi_8bit(0xD1);
  digitalWrite(T_CS, HIGH);
  }


   uint16_t XPT2046::READX(){
  uint16_t x;
  digitalWrite(T_CS, LOW);
  spi_8bit(0xD1);
  x = spi_16bit(0x0000);
  x = x>>4;               //Vi funtion spi_8bit dich 8 bit nen no se READ FULL 16bit DATA
  digitalWrite(T_CS, HIGH);
  return x;
  }

   uint16_t XPT2046::READY(){
  uint16_t z4;
  digitalWrite(T_CS, LOW);
  spi_8bit(0x90);
  z4 = spi_16bit(0x0000);
  z4= z4>>4;               //Vi funtion spi_8bit dich 8 bit nen no se READ FULL 16bit DATA
  digitalWrite(T_CS, HIGH);
  return z4;
  }
