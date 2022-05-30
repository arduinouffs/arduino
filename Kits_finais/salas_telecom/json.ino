#define MQ2PIN A1
#define MQ2PIN_DIGITAL A5
#define NOBREAK_ENTRADA_PIN A3
#define NOBREAK_SAIDA_PIN A2
#define VOLT_CAL_ENTRADA 458
#define VOLT_CAL_SAIDA 503

extern float read_temperature();
extern float read_humidity();
extern float read_voltage(int PIN, float VOLT_CAL);
extern void inicialize_dht();

extern bool ar_condicionado;
extern bool dehumidify;
extern EthernetClient client;

void json_generator(EthernetClient client) {
  client.println("HTTP/1.1 200 OK\nContent-Type:aplication/json\nContent-Disposition:attachment;filename=\"data.json\"\nConnection:close\n");
    
  String request = "";
  request += "{\"temp\":\"";
  request += read_temperature();
  request += "\",\"umid\":\"";
  request += read_humidity();;
  request += "\",\"mq2t\":\"";
  request += analogRead(MQ2PIN);
  request += "\",\"fumaca\":\"";
  if (!digitalRead(MQ2PIN_DIGITAL)) request += "1";
  else request += "0";
  request += "\",\"noin\":\"";
  request += read_voltage(NOBREAK_ENTRADA_PIN, VOLT_CAL_ENTRADA);
  request += "\",\"noout\":\"";
  request += read_voltage(NOBREAK_SAIDA_PIN, VOLT_CAL_SAIDA);;
  request += "\",\"sts\":\"";
  if (ar_condicionado) request += "1";
  else if (dehumidify) request += "2";
  else request += "0";
  request += "\"}";
  client.print(request);
}

void inicialize_sensors() {
  pinMode(MQ2PIN_DIGITAL, INPUT);
  inicialize_dht();
}
