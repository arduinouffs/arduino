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
extern String read_ppm();

extern bool ar_condicionado;
extern bool dehumidify;
extern EthernetClient client;

void json_generator(EthernetClient client) {
  client.println("HTTP/1.1 200 OK\nContent-Type:aplication/json\nContent-Disposition:attachment;filename=\"d.json\"\nConnection:close\n");
    
  String request = "";
  request += "{\"t\":\"";
  request += read_temperature(); // temperatura
  request += "\",\"u\":\"";
  request += read_humidity(); // umidade
  request += "\",\"m\":\"";
  request += analogRead(MQ2PIN); // tensao do MQ-2, quanto mais mais fumaça
  request += "\",\"f\":\"";
  if (!digitalRead(MQ2PIN_DIGITAL)) request += "1"; // reposta digital do MQ-2 para fumaça 
  else request += "0";
  request += ",\"n\":\"";
  request += read_voltage(NOBREAK_ENTRADA_PIN, VOLT_CAL_ENTRADA); // voltagem de entrada no Nobreak
  request += "\",\"z\":\"";
  request += read_voltage(NOBREAK_SAIDA_PIN, VOLT_CAL_SAIDA); // voltagem de saída no Nobreak
  request += "\",\"s\":\"";
  if (ar_condicionado) request += "1";
  else if (dehumidify) request += "2";
  else request += "0"; // status do ar condicionado, 1 ar gelado, 2 desumidificação, 0 desligado
  request += "\"}";
  client.print(request);
}

void inicialize_sensors() {
  pinMode(MQ2PIN_DIGITAL, INPUT);
  inicialize_dht();
}
