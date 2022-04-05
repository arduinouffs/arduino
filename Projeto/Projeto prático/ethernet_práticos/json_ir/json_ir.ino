#include <SPI.h>
#include <Ethernet.h>
//#include <SD.h>
#include <DHT.h>
#include <MQ2.h>
#include <EmonLib.h>
//#include <LiquidCrystal.h>
//#include <LiquidCrystal_I2C.h>
#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>
//#include <EEPROM.h>

#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT  
#define MQ2PIN A2
#define pin2 4 // Vout2
#define pin1 2 // Vout1
#define NOBREAK_ENTRADA_PIN A3
#define NOBREAK_SAIDA_PIN A4
#define VOLT_CAL 440.7

//void readIntArrayFromEEPROM(int address, int numbers[], int arraySize){
//  int addressIndex = address;
//  for (int i = 0; i < arraySize; i++)
//  {
//    numbers[i] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
//    addressIndex += 2;
//  }
//}

#define RAW_DATA_LEN 350
const PROGMEM uint16_t rawDataOff[196]={ // variável constante salva na flash
  510, 1702, 474, 1730, 494, 1702, 422, 1762, 
  510, 1698, 514, 1694, 494, 1674, 514, 1718, 
  454, 666, 490, 634, 454, 682, 490, 630, 
  486, 658, 486, 630, 490, 638, 470, 666, 
  486, 1706, 478, 1706, 482, 1726, 454, 1754, 
  466, 1722, 490, 1718, 458, 1710, 514, 1698, 
  446, 670, 514, 614, 466, 670, 510, 614, 
  450, 686, 510, 630, 494, 630, 502, 614, 
  514, 1698, 470, 1714, 510, 1702, 442, 1742, 
  510, 1682, 466, 1738, 506, 1686, 438, 1766, 
  506, 618, 470, 666, 506, 614, 506, 634, 
  470, 650, 506, 622, 462, 670, 502, 626, 
  462, 1742, 498, 1690, 466, 670, 502, 1690, 
  466, 1738, 502, 618, 498, 1710, 502, 1686, 
  498, 646, 466, 650, 478, 1730, 470, 650, 
  474, 670, 442, 1766, 450, 666, 446, 698, 
  450, 670, 446, 1762, 450, 666, 446, 1766, 
  446, 1738, 446, 698, 446, 1738, 450, 1762, 
  446, 1738, 446, 674, 466, 1742, 446, 674, 
  442, 698, 446, 1742, 442, 698, 446, 674, 
  442, 698, 446, 1742, 438, 706, 442, 1742, 
  438, 706, 438, 1746, 438, 702, 442, 678, 
  438, 1770, 438, 682, 438, 1770, 438, 682, 
  438, 1770, 414, 706, 438, 1770, 414, 1774, 
  438, 7498, 434, 1000};
const PROGMEM uint16_t rawDataOn[198]={ // variável constante salva na flash
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
//uint16_t rawData[RAW_DATA_LEN];
unsigned long tempoDisplay = 0;
unsigned long sampletime_ms = 3000;//sampe 1s ;
unsigned long duration1;
unsigned long duration2;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float pm2p5 = 0;
float pm10 = 0;
const PROGMEM byte grau[8] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
const EthernetServer server(80);     // Cria um servidor WEB
const DHT dht(DHTPIN, DHTTYPE);
const MQ2 mq2(MQ2PIN);
EnergyMonitor tensao_entrada_nobreak;
EnergyMonitor tensao_saida_nobreak;
//                rs  e   d4  d5   d6   d7
// const PROGMEM LiquidCrystal lcd(5,  8,  9,  A4,  A5,  4);
//const LiquidCrystal_I2C lcd(0x27, 16, 2);
const IRsendRaw mySender;

void setup() {  
//  IPAddress ip(172, 20, 66, 10);
    Serial.begin(9600);
    if (Ethernet.begin(mac) == 0) {
       Serial.println(F("DHCP FAILED"));
    } else {
      Serial.println(F("DHCP DONE"));
    }
//    lcd.createChar(0, grau);
//    lcd.begin(16,2);
//    lcd.init();
//    lcd.setBacklight(HIGH);
    server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
    dht.begin();
    mq2.begin();
    tensao_entrada_nobreak.voltage(NOBREAK_ENTRADA_PIN, VOLT_CAL, 1.7);
    tensao_saida_nobreak.voltage(NOBREAK_SAIDA_PIN, VOLT_CAL, 1.7);
    Serial.println(F("Ethernet"));
    Serial.println(Ethernet.localIP());
//    Ethernet.maintain();
//    for (int i = 50; i > 0; i--) {
//      lcd.clear();
//      lcd.print("IP local:");
//      lcd.setCursor(0,1);
//      lcd.print(Ethernet.localIP());
//      lcd.setCursor(13,0);
//      lcd.print(i);
//      lcd.print("s");
//      delay(1000);
//    }
} // fim do setup

void loop() {
    if (millis() > tempoDisplay) {
//      lcd.clear();
//      lcd.print("Temperatura: ");
//      lcd.print(int(dht.readTemperature()));
//      lcd.write(byte(0));
//      lcd.setCursor(0, 1);
//      lcd.print("Umidade:     ");
//      lcd.print(int(dht.readHumidity()));
//      lcd.print("%");
      tempoDisplay = millis() + 60000;

      if (dht.readTemperature() > 26) {
//        uint16_t rawDataOn[198];
//        readIntArrayFromEEPROM(0, rawDataOn, 198);
        mySender.send(rawDataOn,RAW_DATA_LEN,36);
      }

      if (dht.readTemperature() <= 21) {
        mySender.send(rawDataOff,RAW_DATA_LEN,36);
      }

      Ethernet.maintain();
    }
    
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
  
//                    client.print(dht.readTemperature());
//                    client.print(dht.readHumidity());
//                    client.print(mq2.readLPG());
//                    client.print(mq2.readCO());
//                    client.print(mq2.readSmoke());
//                    client.print(pm2p5);
//                    client.print(pm10);
//                    client.print(tensao_entrada_nobreak.Vrms);

                    String request = "";
                    request.reserve(300);
                    request += "{\"temperatura\": \"";
                    request += dht.readTemperature();
                    request += "\",\"umidade\": \"";
                    request += dht.readHumidity();
                    request += "\",\"gas_inflamavel\": \"";
                    request += mq2.readLPG();
//                    request += "nan";
                    request += "\",\"CO2\": \"";
                    request += mq2.readCO();
//                    request += "nan";
                    request += "\",\"fumaca\": \"";
                    request += mq2.readSmoke();
//                    request += "nan";
                    request += "\",\"ar\": {\"concentracao_pm2p5\": \"";
                    request += pm2p5;
                    request += "\",\"concentracao_pm10\": \"";
                    request += pm10;
//                    request += "\",\"consideracao_ar\": \"";
//                    if (pm2p5 < 1000) {
//                     request += "CLEAN";
//                    } else if (pm2p5 > 1000 && pm2p5 < 10000) {
//                     request += "GOOD";
//                    } else if (pm2p5 > 10000 && pm2p5 < 20000) {      
//                     client.print("ACCEPTABLE");
//                    } else if (pm2p5 > 20000 && pm2p5 < 50000) {
//                     client.print("HEAVY");
//                    } else {   // (pm2p5 > 50000 )
//                     client.print("HAZARD");  
//                    }
                    request += "\"},\"tensao_entrada_nobreak\": \"";
                    request += tensao_entrada_nobreak.Vrms;
                    request += "\",\"tensao_saida_nobreak\": \"";
                    request += tensao_saida_nobreak.Vrms;
                    request += "\"}";
                    client.print(request);

                    lowpulseoccupancy1 = 0;
                    lowpulseoccupancy2 = 0;

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
