extern void air_control(bool force = false);
//void att_lcd(bool force = false);
extern void client_verify();
extern void DHCP_connect();
extern void controleDeAr(bool force);
//extern void inicialize_lcd();
extern void inicialize_sensors();

void setup() {
  Serial.begin(9600);
  Serial.println(F("Ligado"));
  DHCP_connect();
  inicialize_sensors();
//  inicialize_lcd();
  air_control(true);
}

void loop() {
    air_control();
//    att_lcd();
    client_verify();
}
