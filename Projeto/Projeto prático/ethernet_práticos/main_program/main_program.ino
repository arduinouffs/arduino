extern void controleDeAr(bool force = false);
//void att_lcd(bool force = false);
extern void json_generator();
extern void DHCP_connect();
extern void controleDeAr(bool force);
//extern void inicialize_lcd();

void setup() {
  Serial.begin(9600);
  Serial.println(F("Ligado"));
  DHCP_connect();
//  inicialize_lcd();
  controleDeAr(true);
}

void loop() {
    controleDeAr();
//    att_lcd();
    json_generator();
}
