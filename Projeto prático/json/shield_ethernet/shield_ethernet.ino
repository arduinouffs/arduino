#include <SPI.h>
#include <Ethernet.h>
//#include <SD.h>
#include <DHT.h>
//#include <ArduinoJson.h>
#include <MQ2.h>

#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT  
#define MQ2PIN A2

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 40); // Endereço IP que a Ethernet Shield terá. Deve ser alterado para um endereço livre da sua rede.
EthernetServer server(80);     // Cria um servidor WEB
DHT dht(DHTPIN, DHTTYPE);
MQ2 mq2(MQ2PIN);

void setup() {
    Serial.begin(9600);

    Ethernet.begin(mac, ip);  // Inicializa a Ethernet Shield
    server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
    dht.begin();
    mq2.begin();
} // fim do setup

void loop() {
    EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)
    if (client) {  // Existe um cliente em conexão ?        
        boolean currentLineIsBlank = true;
        while (client.connected()) {          
            if (client.available()) {
                char c = client.read(); // lê 1 byte (character) do cliente
                                
                if (c == '\n' && currentLineIsBlank) {  
                    // ENVIA A PÁGINA WEB
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
//                    client.println("Connection: keep-alive");
                    client.println("Connection: close");
                    client.println();    

                    client.print("{");
                    client.print('"');//
                    client.print("temperatura");
                    client.print('"');
                    client.print(':');
                    client.print(dht.readTemperature());
                    client.print(",");
                    client.print('"');//
                    client.print("Umidade");
                    client.print('"');
                    client.print(':');
                    client.print(dht.readHumidity());
                    client.print(",");
                    client.print('"');//
                    client.print("Gás Inflamável");
                    client.print('"');
                    client.print(':');
                    client.print(mq2.readLPG());
                    client.print(",");
                    client.print('"');//
                    client.print("CO2");
                    client.print('"');
                    client.print(':');
                    client.print(mq2.readCO());
                    client.print(",");
                    client.print('"');//
                    client.print("Fumaça");
                    client.print('"');
                    client.print(':');
                    client.print(mq2.readSmoke());
                    client.print("}");

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
