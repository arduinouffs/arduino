#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
//#include <MQ2.h>
#include <EmonLib.h>
#include <IRLibSendBase.h>    //We need the base code/
#include <IRLib_HashRaw.h>    //Only use raw sender

IRsendRaw mySender;

#define DHTPIN A0 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT
#define MQ2PIN A1
#define MQ2PIN_DIGITAL A5
#define NOBREAK_ENTRADA_PIN A3
#define NOBREAK_SAIDA_PIN A2
#define VOLT_CAL_ENTRADA 458
#define VOLT_CAL_SAIDA 503
#define pin2 A4 // Vout2 PM10
#define pin1 2 // Vout1 PM2.
#define RAW_DATA_LEN 198

unsigned long tempoDisplay = 0;
unsigned short int umaHora = 0;
bool ar_condicionado = false;
bool dehumidify = false;

#define sampletime_ms 3000 //sampe 1s ;
unsigned long duration1;
unsigned long duration2;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float pm2p5 = 0;
float pm10 = 0;

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x04 };
const EthernetServer server(80);     // Cria um servidor WEB
const DHT dht(DHTPIN, DHTTYPE);
//const MQ2 mq2(MQ2PIN);
EnergyMonitor tensao_entrada_nobreak;
EnergyMonitor tensao_saida_nobreak;

void (*Reset)() = 0;
void controleDeAr();
void sender(uint16_t rawData[RAW_DATA_LEN]);

const PROGMEM uint16_t rawDataOff[RAW_DATA_LEN]={
  5994, 7398, 506, 1662, 478, 1710, 498, 1690, 
  458, 1706, 506, 1686, 538, 1630, 462, 1722, 
  546, 1626, 546, 610, 518, 586, 498, 638, 
  518, 586, 498, 634, 522, 586, 498, 634, 
  522, 586, 498, 1666, 546, 1642, 546, 1622, 
  442, 1742, 546, 1646, 526, 1634, 550, 1642, 
  546, 1622, 514, 642, 518, 590, 494, 638, 
  518, 586, 494, 642, 518, 586, 498, 634, 
  522, 586, 498, 1662, 546, 1646, 542, 1626, 
  482, 1702, 538, 1650, 534, 1630, 542, 1646, 
  538, 1650, 462, 674, 510, 598, 490, 642, 
  510, 594, 530, 602, 514, 594, 534, 598, 
  514, 618, 514, 1626, 442, 1742, 542, 594, 
  486, 1674, 546, 1642, 546, 590, 494, 1666, 
  498, 1690, 494, 614, 502, 630, 494, 1674, 
  466, 682, 478, 630, 446, 1734, 490, 626, 
  442, 682, 486, 630, 466, 1714, 462, 670, 
  482, 1686, 394, 1790, 486, 626, 446, 1738, 
  454, 1730, 486, 1686, 414, 714, 482, 1702, 
  486, 626, 478, 650, 482, 1690, 414, 710, 
  482, 630, 394, 730, 486, 1702, 486, 622, 
  482, 1706, 486, 626, 394, 1786, 486, 630, 
  466, 658, 486, 1702, 482, 630, 450, 1734, 
  482, 634, 462, 1718, 450, 678, 486, 1686, 
  446, 1734, 486, 7422, 482, 1000};
#define off 0xff00ff00ff00db245ba45

const PROGMEM uint16_t rawDataOn[RAW_DATA_LEN]={
  5998, 7474, 466, 1718, 470, 1730, 454, 1746, 
  466, 1766, 394, 1770, 466, 1726, 434, 1774, 
  466, 1718, 470, 670, 446, 702, 446, 670, 
  398, 746, 470, 646, 482, 666, 446, 670, 
  470, 646, 442, 1750, 494, 1710, 422, 1770, 
  494, 1698, 414, 1790, 498, 1694, 470, 1734, 
  498, 1690, 542, 622, 478, 642, 478, 638, 
  498, 646, 506, 614, 434, 710, 506, 610, 
  486, 658, 486, 1678, 538, 1670, 506, 1678, 
  478, 1754, 478, 1706, 482, 1726, 454, 1734, 
  482, 1706, 474, 666, 482, 642, 442, 694, 
  482, 638, 446, 694, 454, 666, 450, 694, 
  450, 666, 450, 1762, 454, 662, 478, 642, 
  470, 1738, 450, 1762, 446, 670, 450, 1738, 
  446, 1762, 450, 670, 446, 1762, 450, 1738, 
  442, 698, 450, 670, 446, 1762, 446, 674, 
  446, 694, 450, 670, 446, 1762, 446, 674, 
  446, 1762, 446, 1738, 446, 698, 442, 1742, 
  446, 1742, 466, 1742, 442, 678, 462, 1746, 
  442, 678, 438, 702, 442, 1746, 438, 706, 
  438, 682, 434, 730, 414, 1746, 438, 730, 
  414, 1746, 438, 730, 410, 1750, 434, 734, 
  410, 706, 414, 1794, 414, 706, 414, 1794, 
  414, 706, 414, 1770, 438, 706, 414, 1770, 
  414, 1794, 414, 7550, 414, 1000};
#define on 0xff00ff00ff009b645ba45

const PROGMEM uint16_t rawDataDehumidify[RAW_DATA_LEN]={
  6062, 7422, 458, 1750, 510, 1678, 506, 1702, 
  506, 1678, 478, 1730, 506, 1682, 474, 1730, 
  510, 1682, 414, 722, 506, 614, 446, 690, 
  510, 614, 474, 662, 510, 610, 510, 630, 
  510, 610, 510, 1698, 506, 1682, 534, 1670, 
  506, 1682, 442, 1758, 514, 1682, 446, 1754, 
  510, 1698, 474, 642, 510, 634, 470, 646, 
  510, 614, 462, 674, 510, 614, 414, 718, 
  510, 638, 394, 1782, 510, 1702, 470, 1714, 
  506, 1702, 506, 1678, 478, 1726, 510, 1682, 
  414, 1786, 510, 634, 442, 674, 510, 614, 
  490, 646, 506, 634, 398, 718, 510, 614, 
  490, 642, 510, 1702, 470, 646, 510, 1698, 
  474, 642, 510, 630, 478, 638, 510, 634, 
  474, 1710, 510, 630, 502, 1686, 498, 638, 
  506, 1682, 474, 1726, 482, 1726, 446, 1738, 
  482, 662, 446, 670, 482, 1726, 482, 1702, 
  454, 686, 506, 1678, 474, 1730, 514, 610, 
  470, 1730, 514, 1694, 478, 638, 518, 626, 
  474, 1710, 482, 658, 506, 610, 550, 1658, 
  510, 610, 450, 686, 510, 1678, 466, 670, 
  510, 1682, 462, 670, 510, 1698, 474, 642, 
  510, 634, 470, 1714, 502, 638, 506, 1678, 
  470, 670, 502, 1690, 462, 670, 502, 1706, 
  442, 1738, 502, 7478, 470, 1000};
#define dehumidifyy 0xff00ff00ff00a15e6d925

//#define protocol SANYO // para o controle ZH/JT-01 do KOMECO
#define khz 36

uint16_t inRAM[RAW_DATA_LEN];

void setup() {
  pinMode(MQ2PIN_DIGITAL, INPUT);
  Serial.begin(9600);
  Serial.println(F("Ligado"));
//  for (int i = 0; i < 5; i++) mySender.send(rawDataOff,RAW_DATA_LEN,36);
  sender(rawDataOff);
//  mySender.send(CYKM, off, khz);
  if (Ethernet.begin(mac) == 0) {
     Serial.println(F("DHCP FAILED"));
     delay(30000);
     Reset();
  } else {
    Serial.println(F("DHCP DONE"));
  }
  server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
  dht.begin();
//  mq2.begin();
  tensao_entrada_nobreak.voltage(NOBREAK_ENTRADA_PIN, VOLT_CAL_ENTRADA, 1.7);
  tensao_saida_nobreak.voltage(NOBREAK_SAIDA_PIN, VOLT_CAL_SAIDA, 1.7);
  Serial.println(Ethernet.localIP());
}

void loop() {
    controleDeAr();
    Ethernet.maintain();
    EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)
    if (client) {  // Existe um cliente em conexão ?       
        Serial.println(F("Send")); 
        boolean currentLineIsBlank = true;
        while (client.connected()) {          
            if (client.available()) {
                char c = client.read(); // lê 1 byte (character) do cliente
                                
                if (c == '\n' && currentLineIsBlank) {  
                    // ENVIA A PÁGINA WEB
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: aplication/json");
                    client.println("Content-Disposition: attachment ; filename = \"data.json\"");
                    client.println("Connection: close");
                    client.println();

                    duration1 = pulseIn(pin1, LOW);
                    duration2 = pulseIn(pin2, LOW);
                    lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
                    lowpulseoccupancy2 = lowpulseoccupancy2+duration2;

                    ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
                    pm2p5 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve
                  
                    ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
                    pm10 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

                    tensao_entrada_nobreak.calcVI(17,2000); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS, TEMPO LIMITE PARA FAZER A MEDIÇÃO)    
                    tensao_saida_nobreak.calcVI(17,2000);

                    String request = "";
                    request += "{\"temperatura\": \"";
                    request += dht.readTemperature();
                    request += "\",\"umidade\": \"";
                    request += dht.readHumidity();
//                    request += "\",\"gas_inflamavel\": \"";
//                    request += mq2.readLPG();
//                    request += "\",\"CO2\": \"";
//                    request += mq2.readCO();
//                    request += "\",\"fumaca\": \"";
//                    request += mq2.readSmoke();
                    request += "\",\"mq2_tensao\": \"";
                    request += analogRead(MQ2PIN);
                    request += "\",\"presenca_fumaca\": \"";
                    if (!digitalRead(MQ2PIN_DIGITAL)) request += "1";
                    else request += "0";
                    request += "\",\"ar\": {\"concentracao_pm2p5\": \"";
                    request += pm2p5;
                    request += "\",\"concentracao_pm10\": \"";
                    request += pm10;
                    request += "\"},\"tensao_entrada_nobreak\": \"";
                    request += tensao_entrada_nobreak.Vrms;
                    request += "\",\"tensao_saida_nobreak\": \"";
                    request += tensao_saida_nobreak.Vrms;
                    request += "\",\"ar_condicionado\": \"";
                    if (ar_condicionado) request += "1";
                    else request += "0";
                    request += "\",\"desumidificacao\": \"";
                    if (dehumidify) request += "1";
                    else request += "0";
                    request += "\"}";
                    client.print(request);
                    request = "";

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
        
        delay(1);      // da um tempo para o WEB Browser receber o texto
        client.stop(); // termina a conexão
    } // fim do if (client)
} // fim do loop

void controleDeAr() {
    if (millis() > tempoDisplay) {
      tempoDisplay = millis() + 60000;
      umaHora++;
      if (umaHora == 59) {
//        for (int i = 0; i < 5; i++) mySender.send(rawDataOff,RAW_DATA_LEN,36);
        sender(rawDataOff);
//        mySender.send(UNKNOWN, off, khz);
        ar_condicionado = false;
        dehumidify = false;
        Serial.println(F("Ar desligado1"));
        umaHora = 0;
      }
      float temperatura = dht.readTemperature();
      float umidade = dht.readHumidity();
      Serial.print(F("Temperatura: ")); Serial.println(temperatura);
            
      if (temperatura > 26 && ar_condicionado == false) {
//        for (int i = 0; i < 5; i++) mySender.send(rawDataOn,RAW_DATA_LEN,36);
        sender(rawDataOn);
//        mySender.send(UNKNOWN, on, khz);
        ar_condicionado = true;
        dehumidify = false;
        Serial.println(F("Ar ligado"));
      }

      if (temperatura > 28){
//        for (int i = 0; i < 5; i++) mySender.send(rawDataOn,RAW_DATA_LEN,36);
        sender(rawDataOn);
//        mySender.send(UNKNOWN, on, khz);
        ar_condicionado = true;
        dehumidify = false;
        Serial.println(F("Forçando Ar ligado"));
      }

      if (temperatura <= 23 && ar_condicionado) {
//        for (int i = 0; i < 5; i++) mySender.send(rawDataOff,RAW_DATA_LEN,36);
        sender(rawDataOff);
//        mySender.send(UNKNOWN, off, khz);
        ar_condicionado = false;
        dehumidify = false;
        Serial.println(F("Ar desligado2")); //normal
      }
        

      if (umidade > 60 && dehumidify == false && ar_condicionado == false) {
//        for (int i = 0; i < 5; i++) mySender.send(rawDataDehumidify,RAW_DATA_LEN,36);
        sender(rawDataDehumidify);
//        mySender.send(UNKNOWN, dehumidifyy, khz);
        Serial.println(F("Desumidificação em ação"));
        dehumidify = true;
      } else {
        if (umidade < 55 && dehumidify) {
//          for (int i = 0; i < 5; i++) mySender.send(rawDataOff,RAW_DATA_LEN,36);
          sender(rawDataOff);
//          mySender.send(UNKNOWN, off, khz);
          Serial.println(F("Ar desligado3"));
          dehumidify = false;
        }
      }   
    }
}

void sender(uint16_t rawData[RAW_DATA_LEN]) {
  memset(inRAM, 0, RAW_DATA_LEN*2);
  memcpy_P(inRAM, &rawData, RAW_DATA_LEN*2);
  mySender.send(inRAM,RAW_DATA_LEN*2,36);
}
