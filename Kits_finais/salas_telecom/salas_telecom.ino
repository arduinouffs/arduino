extern void air_control(bool force = false);
extern void inicialize_lcd();
extern void att_lcd(bool force = false);
extern void client_verify();
extern void DHCP_connect();
extern void inicialize_sensors();

void setup() {
  Serial.begin(9600);
  Serial.println(F("Ligado"));
  DHCP_connect();
  inicialize_sensors();
  inicialize_lcd();
  att_lcd(true);
  air_control(true);
}

void loop() {
  air_control();
  att_lcd();
  client_verify();
}

//int main() {
//  Serial.begin(9600);
//  Serial.println(F("Ligado"));
//  DHCP_connect();
//  inicialize_sensors();
//  inicialize_lcd();
//  att_lcd(true);
//  air_control(true);
//  
//  while (true) {
//    air_control();
//    att_lcd();
//    client_verify();
//  }
//}
