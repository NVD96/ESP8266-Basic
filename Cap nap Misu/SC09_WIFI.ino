
#include <ESP8266WiFi.h>

const char *ssid = "NVD";
const char *password = "12345679";

byte buf[10];

WiFiServer server(80);


//IPAddress local_IP(192,168,5,1);
//IPAddress gateway(192,168,5,0);
//IPAddress subnet(255,255,255,0);

void setup() {
  delay(1000);
  Serial.begin(9600, SERIAL_7E1);
  WiFi.softAP(ssid, password, 1, false);
 // WiFi.softAPConfig(local_IP, gateway, subnet);
  //IPAddress myIP = WiFi.softAPIP();
  //Serial.println(myIP);
  server.begin();
}

void loop() {

 WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  //if (client)                                     //Khi chay lenh if(client) phai delay roi moi chay lenh client.available()  NEU KHONG CO HAM DELAY SE BI XUNG DOT VA KHONG CHAY DUOC CODE
  
    while(client){
        delay(50);
        while (client.available()) {
           Serial.write(client.read());
        }
        while (Serial.available()) {
           client.write(Serial.read());
        }
      }  
    // close the connection:
    client.stop();

   
}
