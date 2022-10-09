#include <Wire.h>
#include <SPI.h>

#define CS_PIN  15
#define SPI_SETTING     SPISettings(2000000, MSBFIRST, SPI_MODE0)

//D5-------->T_CLK
//D6-------->T_OUT
//D7-------->T_DIN
//D8-------->T_CS

unsigned int READZ1();
void READZ2(void);
void READX(void);
void READY(void);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  digitalWrite(CS_PIN, HIGH);
}

void loop() {
SPI.beginTransaction(SPI_SETTING); 

      if(READZ1()> 400){
        
      READX();
      READY();
      
      }
SPI.endTransaction(); 
delay(100);

}


void READX(){
  
digitalWrite(CS_PIN, LOW);
SPI.transfer(0xD1);
unsigned int z = SPI.transfer16(0x00);
digitalWrite(CS_PIN, HIGH);
Serial.print(", X = ");
Serial.print(z);

}


void READY(){
  
digitalWrite(CS_PIN, LOW);
SPI.transfer(0x91);
unsigned int z = SPI.transfer16(0x00);
digitalWrite(CS_PIN, HIGH);
Serial.print(", Y = ");
Serial.println(z);

}
unsigned int READZ1(){
  
digitalWrite(CS_PIN, LOW);
SPI.transfer(0xB1);
unsigned int z = SPI.transfer16(0x00);
digitalWrite(CS_PIN, HIGH);
Serial.println(", Z1 = ");
Serial.print(z);
return z;

}
void READZ2(){
  
digitalWrite(CS_PIN, LOW);
SPI.transfer(0xC1);
unsigned int z = SPI.transfer16(0x00);
digitalWrite(CS_PIN, HIGH);
Serial.print(", Z2 = ");
Serial.print(z);

}
