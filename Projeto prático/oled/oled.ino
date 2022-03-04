#include <U8glib.h>
#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN A0
#define DHTTYPE DHT11

unsigned long tempo = 0;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
bool ipDisplay = true;
int cont = 0;

U8GLIB_SH1106_128X64 oled(7, 6, 3, 4, 5);  // D0, D1, CS, DC, Reset
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
  oled.setFont(u8g_font_helvR12);
  Serial.println(IpAddressToString(Ethernet.localIP()));
}
void loop() {
  oled.firstPage();
  int temperatura = 50;
  int umidade = 70;

  if (millis() > tempo) {
    do {
      if (ipDisplay) {
        oled.drawStr(0, 12, "IP:");
        oled.setPrintPos(18, 12);
      } else {
        oled.setPrintPos(0, 12);
      }
      oled.print(Ethernet.localIP());

      if (cont == 0) {
        oled.drawStr(0, 55, "Temp:");  
        oled.setPrintPos(75, 55);
        oled.print(temperatura);
        oled.print((char)176);
        oled.print("C");
        
        oled.drawStr(0, 35, "Umid:");
        oled.setPrintPos(75, 35);
        oled.print(umidade);
        oled.print("%");   
      } else if (cont == 1) {
        oled.drawStr(0, 55, "Fumaca:");  
        oled.setPrintPos(75, 55);
        oled.print(temperatura);
        oled.print((char)176);
        oled.print("C");
        
        oled.drawStr(0, 35, "CO2:");
        oled.setPrintPos(75, 35);
        oled.print(umidade);
        oled.print("%");   
      }
      
    } while( oled.nextPage() );
    
    tempo = millis() + 2000;
    cont++;
    if (cont > 1) {
      cont = 0;
    }
  }
  
  
}
