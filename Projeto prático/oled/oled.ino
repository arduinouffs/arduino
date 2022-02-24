#include <U8glib.h>
#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN A0
#define DHTTYPE DHT11

unsigned long tempo = 0;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
unsigned long tempo = 0;
bool ipDisplay = true;

U8GLIB_SH1106_128X64 u8g(2, 3, 6, 5, 4);  // D0=13, D1=11, CS=10, DC=9, Reset=8
EthernetServer server(80);     // Cria um servidor 

String IpAddressToString(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  server.begin();
  if (IpAddressToString(Ethernet.localIP()).length() > 13) {
    ipDisplay = false;
  }
    u8g.setFont(u8g_font_helvR12);
}
void loop() {
  u8g.firstPage();
  float temperatura = 50;
  float umidade = 70;
  
  do {
    if (ipDisplay) {
      u8g.drawStr(0, 12, "IP:");
      u8g.setPrintPos(18, 12);
    } else {
      u8g.setPrintPos(0, 12);
    }
    u8g.print(Ethernet.localIP());

    if (millis() > tempo + 60000) {
      u8g.drawStr(0, 55, "Temp:");  
      u8g.setPrintPos(75, 55);
      u8g.print(temperatura);
      u8g.print((char)176);
      u8g.print("C");
      
      u8g.drawStr(0, 35, "Umid:");
      u8g.setPrintPos(75, 35);
      u8g.print(umidade);
      u8g.print("%"); 
    } else if (millis() > tempo + 50000) {
      u8g.drawStr(0, 55 , "CO2");
    } else if (millis() > tempo + 40000) {
      
    } else if (millis() > tempo + 30000) {
      
    } else if (millis() > tempo + 20000) {
      
    }
    
  } while( u8g.nextPage() );
  tempo = millis();
  
  delay(5000);
}
