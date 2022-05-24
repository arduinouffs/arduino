#include <SPI.h>
#include <Ethernet.h>

// caso precise, defina cabeçalho de função aqui
extern void json_generator(EthernetClient client);

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x04 };
const EthernetServer server(80);     // Cria um servidor WEB
EthernetClient client;

//#define RAW_DATA_LEN 198

bool ar_condicionado = false;
bool dehumidify = false;

void client_verify() {
    Ethernet.maintain();
    client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)
    if (client) {  // Existe um cliente em conexão ?       
        Serial.println(F("Send json")); 
        boolean currentLineIsBlank = true;
        while (client.connected()) {          
            if (client.available()) {
                char c = client.read(); // lê 1 byte (character) do cliente
                                
                if (c == '\n' && currentLineIsBlank) {  
                    // ENVIA A PÁGINA WEB
                    json_generator(client);

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
}
