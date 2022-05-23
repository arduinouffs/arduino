#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <EmonLib.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

IRsend irsend;

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
//#define RAW_DATA_LEN 198

bool ar_condicionado = false;
bool dehumidify = false;
unsigned long timeDisplay = 0;
unsigned long timeAC = 0;

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
//EnergyMonitor tensao_entrada_nobreak;
//EnergyMonitor tensao_saida_nobreak;
EnergyMonitor tensao;
const LiquidCrystal_I2C lcd(0x27, 16, 2);

void (*Reset)() = 0;
void controleDeAr(bool force = false);
void att_lcd(bool force = false);
void sendRAW_Flash(const unsigned int * signalArray, unsigned int signalLength, unsigned char carrierFreq);

byte grau[8] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

const PROGMEM short rawDataOff[]={
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
#define off 0xff00ff00ff00df205ba454ab

const PROGMEM short rawDataOn[]={
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
#define on 0xff00ff00ff009f605ba454ab

//const PROGMEM short rawDataDehumidify[]={
//  6062, 7422, 458, 1750, 510, 1678, 506, 1702, 
//  506, 1678, 478, 1730, 506, 1682, 474, 1730, 
//  510, 1682, 414, 722, 506, 614, 446, 690, 
//  510, 614, 474, 662, 510, 610, 510, 630, 
//  510, 610, 510, 1698, 506, 1682, 534, 1670, 
//  506, 1682, 442, 1758, 514, 1682, 446, 1754, 
//  510, 1698, 474, 642, 510, 634, 470, 646, 
//  510, 614, 462, 674, 510, 614, 414, 718, 
//  510, 638, 394, 1782, 510, 1702, 470, 1714, 
//  506, 1702, 506, 1678, 478, 1726, 510, 1682, 
//  414, 1786, 510, 634, 442, 674, 510, 614, 
//  490, 646, 506, 634, 398, 718, 510, 614, 
//  490, 642, 510, 1702, 470, 646, 510, 1698, 
//  474, 642, 510, 630, 478, 638, 510, 634, 
//  474, 1710, 510, 630, 502, 1686, 498, 638, 
//  506, 1682, 474, 1726, 482, 1726, 446, 1738, 
//  482, 662, 446, 670, 482, 1726, 482, 1702, 
//  454, 686, 506, 1678, 474, 1730, 514, 610, 
//  470, 1730, 514, 1694, 478, 638, 518, 626, 
//  474, 1710, 482, 658, 506, 610, 550, 1658, 
//  510, 610, 450, 686, 510, 1678, 466, 670, 
//  510, 1682, 462, 670, 510, 1698, 474, 642, 
//  510, 634, 470, 1714, 502, 638, 506, 1678, 
//  470, 670, 502, 1690, 462, 670, 502, 1706, 
//  442, 1738, 502, 7478, 470, 1000};
#define dehumidifyy 0xff00ff00ff009f605da254ab

//const PROGMEM char cabecalho[] = "HTTP/1.1 200 OK\nContent-Type:aplication/json\nContent-Disposition:attachment;filename=\"data.json\"\nConnection:close\n";

#define khz 36

void setup() {
  pinMode(MQ2PIN_DIGITAL, INPUT);
  Serial.begin(9600);
  Serial.println(F("Ligado"));
  sendRAW_Flash(rawDataOff, sizeof(rawDataOff)/sizeof(int),khz);
  if (Ethernet.begin(mac) == 0) {
     Serial.println(F("DHCP FAILED"));
     delay(30000);
     Reset();
  } else {
    Serial.println(F("DHCP DONE"));
  }
  server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
  dht.begin();
//  tensao_entrada_nobreak.voltage(NOBREAK_ENTRADA_PIN, VOLT_CAL_ENTRADA, 1.7);
//  tensao_saida_nobreak.voltage(NOBREAK_SAIDA_PIN, VOLT_CAL_SAIDA, 1.7);
  Serial.println(Ethernet.localIP());
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, grau);
  controleDeAr(true);
  att_lcd(true);
}

void loop() {
    controleDeAr();
//    att_lcd();
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
                    tensao.voltage(NOBREAK_ENTRADA_PIN, VOLT_CAL_ENTRADA, 1.7);
                    tensao.calcVI(17,2000);
                    request += tensao.Vrms;
                    request += "\",\"noout\":\"";
                    tensao.voltage(NOBREAK_SAIDA_PIN, VOLT_CAL_SAIDA, 1.7);
                    tensao.calcVI(17,2000);
                    request += tensao.Vrms;
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
} // fim do loop

void controleDeAr(bool force) {
    if ((millis() % 60000) == 0 || force) {
//    if (millis() > timeAC) {
      float temperatura = dht.readTemperature();
      float umidade = dht.readHumidity();

      if ((millis() % 3600000) == 0) { // 1h
        sendRAW_Flash(rawDataOff, sizeof(rawDataOff)/sizeof(int),khz);
        ar_condicionado = false;
        dehumidify = false;
        Serial.println(F("Ar desligado1"));
      }
      Serial.print(F("Temperatura: ")); Serial.println(temperatura);
            
      if (temperatura > 26 && ar_condicionado == false) {
        sendRAW_Flash(rawDataOn, sizeof(rawDataOn)/sizeof(int),khz);
        ar_condicionado = true;
        dehumidify = false;
        Serial.println(F("Ar ligado"));
      }

      if (temperatura > 28){
        sendRAW_Flash(rawDataOn, sizeof(rawDataOn)/sizeof(int),khz);
        ar_condicionado = true;
        dehumidify = false;
        Serial.println(F("Forçando Ar ligado"));
      }

      if (temperatura <= 23 && ar_condicionado) {
        sendRAW_Flash(rawDataOff, sizeof(rawDataOff)/sizeof(int),khz);
        ar_condicionado = false;
        dehumidify = false;
        Serial.println(F("Ar desligado2")); //normal
      }
        

      if (umidade > 60 && dehumidify == false && ar_condicionado == false) {
//        sendRAW_Flash(rawDataDehumidify, sizeof(rawDataDehumidify)/sizeof(int),khz);
        Serial.println(F("Desumidificação em ação"));
        dehumidify = true;
      } else {
        if (umidade < 55 && dehumidify) {
          sendRAW_Flash(rawDataOff, sizeof(rawDataOff)/sizeof(int),khz);
          Serial.println(F("Ar desligado3"));
          dehumidify = false;
        }
      }   
    }
    timeAC = millis() + 60000;
}

void sendRAW_Flash(const unsigned int * signalArray, unsigned int signalLength, unsigned char carrierFreq) {
  irsend.enableIROut(carrierFreq); //initialise the carrier frequency for each signal to be sent
  for (unsigned int i=0; i < signalLength; i++) {
    if (i & 1) irsend.space(pgm_read_word_near(&signalArray[i]));
    else irsend.mark(pgm_read_word_near(&signalArray[i]));
  }
  irsend.space(1);//make sure IR is turned off at end of signal
}

void att_lcd(bool force) {
  static bool mode = true;
  if ((millis() % 10000) == 0 || force) {
//  if (millis() > timeDisplay) {
    if (mode) {
      lcd.clear();
      lcd.print("IP local:");
      lcd.setCursor(0,1);
      lcd.print(Ethernet.localIP());
    } else {
      lcd.clear();
      lcd.print("Temperatura: ");
      lcd.print(int(dht.readTemperature()));
      lcd.write((byte)0);
      lcd.setCursor(0, 1);
      lcd.print("Umidade:     ");
      lcd.print(int(dht.readHumidity()));
      lcd.print("%");
    }
    mode = !mode;
    timeDisplay = millis() + 10000;
  }
}
