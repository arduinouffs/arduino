#include <SPI.h>
#include <Ethernet.h>

// caso precise, defina cabeçalho de função aqui
extern void inicialize_dht();
extern float read_temperature();
extern float read_humidity();
extern float read_voltage();

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x04 };
const EthernetServer server(80);     // Cria um servidor WEB

#define MQ2PIN A1
#define MQ2PIN_DIGITAL A5
#define NOBREAK_ENTRADA_PIN A3
#define NOBREAK_SAIDA_PIN A2
#define VOLT_CAL_ENTRADA 458
#define VOLT_CAL_SAIDA 503
#define pin2 A4 // Vout2 PM10
#define pin1 2 // Vout1 PM2
//#define RAW_DATA_LEN 198

#define sampletime_ms 3000 //sampe 1s ;
unsigned long duration1;
unsigned long duration2;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float pm2p5 = 0;
float pm10 = 0;

bool ar_condicionado = false;
bool dehumidify = false;

void json_generator() {
    Ethernet.maintain();
    EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)
    if (client) {  // Existe um cliente em conexão ?       
        Serial.println(F("Send json")); 
        boolean currentLineIsBlank = true;
        while (client.connected()) {          
            if (client.available()) {
                char c = client.read(); // lê 1 byte (character) do cliente
                                
                if (c == '\n' && currentLineIsBlank) {  
                    // ENVIA A PÁGINA WEB
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
                    request += dht.readTemperature();
                    request += "\",\"umid\":\"";
                    request += dht.readHumidity();
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

                    break;                
                }
                
                // toda linha de texto recebida do cliente termina com os caracteres \r\n
                if (c == '\n') {
                    // ultimo caractere da linha do texto recebido
                    // iniciando nova linha com o novo caractere lido
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // um caractere de texto foi recebido do cliente
                    currentLineIsBlank = false;
                }
            } // fim do if (client.available())
        } // fim do while (client.connected())
        
        delay(100);      // da um tempo para o WEB Browser receber o texto
        client.stop(); // termina a conexão
    } // fim do if (client)
}

void (*Reset)() = 0;

void DHCP_connect() {
  if (Ethernet.begin(mac) == 0) {
     Serial.println(F("DHCP FAILED"));
     delay(30000);
     Reset();
  } else {
    Serial.println(F("DHCP DONE"));
  }
  server.begin();
  Serial.println(Ethernet.localIP());
  inicialize_dht();
  pinMode(MQ2PIN_DIGITAL, INPUT);
}
