#include <U8glib.h>
#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN A0
#define DHTTYPE DHT11
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
unsigned long tempo = 0;
bool ipDisplay = true;

U8GLIB_SH1106_128X64 u8g(2, 3, 5, 6, 7);  // D0=13, D1=11, CS=10, DC=9, Reset=8
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
}
void loop() {
  u8g.firstPage();
  float temperatura = 50;
  float umidade = 70;
  do {
    u8g.setFont(u8g_font_helvR12);  //
    u8g.drawStr(0, 55, "Temp:");  
    u8g.setPrintPos(75, 55);
    u8g.print(temperatura, 0);
    u8g.print((char)176);
    u8g.print("C");
    
    u8g.drawStr(0, 35, "Humi:");
    u8g.setPrintPos(75, 35);
    u8g.print(umidade, 0);
    u8g.print("%");

    if (ipDisplay) {
      u8g.drawStr(0, 12, "IP:");
      u8g.setPrintPos(18, 12);
      u8g.print(Ethernet.localIP());
    } else {
      u8g.setPrintPos(0, 12);
      u8g.print(Ethernet.localIP());
    }
    
  } while( u8g.nextPage() );
  delay(5000);
}
