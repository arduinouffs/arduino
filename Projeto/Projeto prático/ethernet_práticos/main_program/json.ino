#define MQ2PIN A1
#define MQ2PIN_DIGITAL A5
#define NOBREAK_ENTRADA_PIN A3
#define NOBREAK_SAIDA_PIN A2
#define VOLT_CAL_ENTRADA 458
#define VOLT_CAL_SAIDA 503
#define pin2 A4 // Vout2 PM10
#define pin1 2 // Vout1 PM2

extern float read_temperature();
extern float read_humidity();
extern float read_voltage(int PIN, float VOLT_CAL);
extern void inicialize_dht();

extern bool ar_condicionado;
extern bool dehumidify;
extern EthernetClient client;

#define sampletime_ms 3000 //sampe 1s ;
unsigned long duration1;
unsigned long duration2;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float pm2p5 = 0;
float pm10 = 0;

void json_generator(EthernetClient client) {
  client.println("HTTP/1.1 200 OK\nContent-Type:aplication/json\nContent-Disposition:attachment;filename=\"data.json\"\nConnection:close\n");
  
  duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;
  
  ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
  pm2p5 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve
  
  ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
  pm10 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62;
  
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
  request += "\",\"ar\": {\"pm2p5\":\"";
  request += pm2p5;
  request += "\",\"pm10\":\"";
  request += pm10;
  request += "\"},\"noin\":\"";
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
