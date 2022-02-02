#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 40); // Endereço IP que a Ethernet Shield terá. Deve ser alterado para um endereço livre da sua rede.
EthernetServer server(80);     // Cria um servidor WEB

void setup() {
    Ethernet.begin(mac, ip);  // Inicializa a Ethernet Shield
    server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
} // fim do setup

void loop() {

    EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)

    if (client) {  // Existe um cliente em conexão ?
      
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // os dados do cliente estão disponiveis para serem lidos
                char c = client.read(); // lê 1 byte (character) do cliente
                
                // a ultima linha da requisição do cliente é branca e termina com o caractere \n
                // responde para o cliente apenas após a última linha recebida
                if (c == '\n' && currentLineIsBlank) {
                  
                    // envia o cabeçalho de uma resposta http padrão
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    
                    // ENVIA A PÁGINA WEB
                    client.println("<!DOCTYPE html>");
                        client.println("<html>");                        
                        client.println("<head>");
                          client.println("<title>Página Arduino</title>");
                          client.println("<meta charset='UTF-8'>");
                          client.println("<meta http-equiv='X-UA-Compatible' content='IE=edge'>");
                          client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
                          client.println("<style>.temp_umid {font-size: 50px;}</style>");
                          client.println("</head><body>");
                            client.println("<h1>Controle de temperatura e umidade</h1>");
                            client.println("<div><div>");
                              client.println("<h3>Temperatura</h3><div class='temp_umid'><span id='temperatura'>10</span>°C</div>");
                              client.println("</div><div>");
                              client.println("<h3>Umidade</h3><div class='temp_umid'><span id='umidade'>80</span>%</div>");
                          client.println("</div></div></body></html>");  
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
