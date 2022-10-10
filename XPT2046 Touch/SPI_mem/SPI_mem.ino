
#define T_CLK 0
#define T_CS 4
#define T_DIN 5
#define T_OUT 16

uint16_t z;
uint8_t spi_8bit(uint8_t data);
uint16_t spi_16bit(uint16_t data1);

void setup() {
  // put your setup code here, to run once:

pinMode(T_CLK, OUTPUT);
pinMode(T_CS, OUTPUT);
pinMode(T_DIN, OUTPUT);
pinMode(T_OUT, INPUT);
digitalWrite(T_CLK, LOW);
digitalWrite(T_CS, HIGH);
Serial.begin(9600);

}

void loop() {
  digitalWrite(T_CS, LOW);
  spi_8bit(0xC1);
  z = spi_16bit(0x0000);
  z = z>>4;               //Vi funtion spi_8bit dich 8 bit nen no se READ FULL 16bit DATA
  digitalWrite(T_CS, HIGH);
  Serial.println(z);
}

uint8_t spi_8bit(uint8_t data){
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


 uint16_t spi_16bit(uint16_t data1){
  uint16_t gt;
  uint8_t data1L, data1H;
  data1H = data1>>8;
  data1L = data1;
  gt = spi_8bit(data1H);
  gt = gt <<8;
  gt = gt | spi_8bit(data1L);
  return gt;
  }
