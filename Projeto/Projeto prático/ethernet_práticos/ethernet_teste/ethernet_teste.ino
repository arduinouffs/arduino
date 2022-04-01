#include <SPI.h>
#include <Ethernet.h>

unsigned long tempoDisplay = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetServer server(80);     // Cria um servidor WEB

void setup() {
  Serial.begin(9600);
    if (Ethernet.begin(mac) == 0) {
       Serial.println("DHCP FAILED");
    } else {
      Serial.println("DHCP DONE");
    }
    Serial.println(Ethernet.localIP());
    Serial.println("Ethernet");
}

void loop() {
  if (millis() > tempoDisplay) {
      tempoDisplay = millis() + 60000;
      Ethernet.maintain();
  }

  EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)
  if (client) {  // Existe um cliente em conexão ?       
      Serial.println("Existe"); 
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

//                  duration1 = pulseIn(pin1, LOW);
//                  duration2 = pulseIn(pin2, LOW);
//                  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
//                  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;

//                  ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
//                  concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve
//                
//                  ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
//                  concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

//                  emon1.calcVI(17,2000); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS, TEMPO LIMITE PARA FAZER A MEDIÇÃO)    /

                  float concentration1 = 10001;

                  String request = "";
                  request += "{\"temperatura\": \"";
//                  client.print("{\"temperatura\": \"");
                  request += "27.0";
//                  client.print("25.0");
                  request += "\",\"umidade\": \"";
//                  client.print("\",\"umidade\": \"");
                  request += "40.0";
//                  client.print("40.0");
                  request += "\",\"gas inflamavel\": \"";
//                  client.print("\",\"gas inflamavel\": \"");
                  request += "16555615";
//                  client.print("16555615");
                  request += "\",\"CO2\": \"";
//                  client.print("\",\"CO2\": \"");
                  request += "1154545";
//                  client.print("1154545");
                  request += "\",\"fumaca\": \"";
//                  client.print("\",\"fumaca\": \"");
                  request += "5454";
//                  client.print("5454");
                  request += "\",\"ar\": {\"concentracao1\": \"";
//                  client.print("\",\"ar\": {\"concentracao1\": \"");
                  request += "878";
//                  client.print("878");
                  request += "\",\"concentracao2\": \"";
//                  client.print(" pcs/0.01cf\",\"concentracao2\": \"");
                  request += "77";
//                  client.print("77");
                  request += "\",\"consideracao_ar\": \"";
//                  client.print(" pcs/0.01cf\",\"consideracao de ar\": \"");
                  request += "CLEAN";
//                  if (concentration1 < 1000) {
//                   client.print("CLEAN");
//                  } else if (concentration1 > 1000 && concentration1 < 10000) {
//                   client.print("GOOD");
//                  } else if (concentration1 > 10000 && concentration1 < 20000) {      
//                   client.print("ACCEPTABLE");
//                  } else if (concentration1 > 20000 && concentration1 < 50000) {
//                   client.print("HEAVY");
//                  } else {   // (concentration1 > 50000 )
//                   client.print("HAZARD");  
//                  }
                  request += "\"},\"voltagem\": \"";
//                  client.print("\"},\"voltagem\": \"");
                  request += "220";
//                  client.print("220");
                  request += "\"}";
//                  client.print("\"}");
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
      
      delay(1);      // da um tempo para o WEB Browser receber o texto
      client.stop(); // termina a conexão
  } // fim do if (client)
}
