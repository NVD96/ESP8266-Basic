#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define CS_PIN  15
// MOSI=11, MISO=12, SCK=13

XPT2046_Touchscreen ts(CS_PIN);
#define TIRQ_PIN  2
//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);  // Param 2 - 255 - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

void setup() {
  Serial.begin(38400);
  ts.begin();
  ts.setRotation(1);
  while (!Serial && (millis() <= 1000));
}

void loop() {
  if (ts.touched()) {
    ts.getPoint();
    Serial.print("Pressure = ");
    Serial.print(z);
    Serial.print(", x = ");
    Serial.print(x);
    Serial.print(", y = ");
    Serial.print(y);
    delay(30);
    Serial.println();
  }
}
