/*
 * MPU6050:
 * - INT  -- D02
 * - SDA  -- A04
 * - SCL  -- A05
 * Powered at 5 V.
 * SD Card:
 * - MOSI -- D11
 * - MISO -- D12
 * - SCLK -- D13
 * - CS   -- D10
 */

#include<Wire.h>
#include <SPI.h>
#include <SD.h>
const int MPU = 0x68;  // I2C address of the MPU-6050
const int SD_CS = 10; // SD CS pin
int minVal=265;
int maxVal=402;
double x;
double y;
double z;
void Read_Write();
void stampC();
void errorFW();
bool Serial_plus_SD = true; // Output to SD and Serial, or only to SD.

// Setup:
void setup()
{    
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);
  Wire.endTransmission(true);
  
  if (Serial_plus_SD)
    Serial.begin(38400);
  if (Serial_plus_SD)
    Serial.println("\nInitializing SD card.");

  if (!SD.begin(SD_CS)) 
  {
    if (Serial_plus_SD)
      Serial.println("Card failed, or not present.");
    while(1)
      ;
  }
  else
  {
    if (Serial_plus_SD)
      Serial.println("Card initialized.");  
      delay(1000);
  }
} 
void loop()
// function that reads the MPU and writes the data to the SD.
{
  int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
  AcX = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY = Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    int xAng = map(AcX,minVal,maxVal,-90,90);
    int yAng = map(AcY,minVal,maxVal,-90,90);
    int zAng = map(AcZ,minVal,maxVal,-90,90);

       x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
       y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
       z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

       
  String dataString = ""; 
  dataString += String(x); dataString += ":";
  dataString += String(y); dataString += ":";
  dataString += String(z); dataString += ":";

  File dataFile = SD.open("MPU6050.txt", FILE_WRITE);
  if (dataFile) 
  {
   // dataFile.println(dataString);
    dataFile.print("TRUC X:=");
    dataFile.println(x);
    dataFile.print("TRUC Y:=");
    dataFile.println(y);
    dataFile.print("TRUC Z:=");
    dataFile.println(z);
    dataFile.close();
    if (Serial_plus_SD)
      //Serial.println(dataString);
      delay(1000);
       Serial.print("TRUC X:");
       Serial.println(x);
       Serial.print("TRUC Y:");
       Serial.println(y);
       Serial.print("TRUC Z:");
       Serial.println(z);
       Serial.println("-----------------------------------------");
       delay(1000);
  }}
 /* else 
    errorFW();
  
  return;
}

void stampC()
{
  // Local variables:
  String dataString = " CALIBRATION "; 
  File dataFile = SD.open("MPU6050.txt", FILE_WRITE);
  if (dataFile) 
  {
    dataFile.println(dataString);
    dataFile.close();
    if (Serial_plus_SD)
      Serial.println(dataString);
      delay(1000);
  }
  else 
    errorFW();
  
  return;
}
void errorFW()
{
  if (Serial_plus_SD)
    Serial.println("Error opening datalog.txt.");
  while(1)
    ;
  return;
}*/
