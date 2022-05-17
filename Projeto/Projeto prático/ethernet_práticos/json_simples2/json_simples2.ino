#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

#define DHTPIN A0 // pino que estamos conectado
#define DHTTYPE DHT22 // DHT

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 };
const EthernetServer server(80);     // Cria um servidor WEB
const DHT dht(DHTPIN, DHTTYPE);

void (*Reset)() = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Ligado"));
  if (Ethernet.begin(mac) == 0) {
     Serial.println(F("DHCP FAILED"));
     delay(30000);
     Reset();
  } else {
    Serial.println(F("DHCP DONE"));
  }
  server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
  dht.begin();
  Serial.println(Ethernet.localIP());
}

void loop() {
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
                    
                    String request = "";
                    request += "{\"temperatura\": \"";
                    request += dht.readTemperature();
                    request += "\",\"umidade\": \"";
                    request += dht.readHumidity();
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
