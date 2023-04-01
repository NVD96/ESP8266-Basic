// gui
#include <LoRa.h>
#include <SoftwareSerial.h>
#define tx 5
#define rx 4
                                    //M1,M2 phai noi GND
                                    //CHU Y   AS32-TTL-100 <.......> ESP8266
                                    //          TX                      TX
                                    //          RX                      RX
SoftwareSerial tai(tx,rx); 
/*****************************************/
char thu;
void setup() {
 Serial.begin(9600);
tai.begin(9600); 

}

/*****************************************/

void loop() {
      thu = 0;
     if(tai.available()){
       thu += char(tai.read());
        Serial.print(thu); 
          }
     
              
}
