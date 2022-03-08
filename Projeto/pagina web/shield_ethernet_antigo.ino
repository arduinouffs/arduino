#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <DHT.h>

#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT  

String HTTP_req; 
String URLValue;

String getURLRequest(String *requisicao);
bool mainPageRequest(String *requisicao);
void tempUmid(EthernetClient cl);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 40); // Endereço IP que a Ethernet Shield terá. Deve ser alterado para um endereço livre da sua rede.
EthernetServer server(80);     // Cria um servidor WEB
File webFile;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Ethernet.begin(mac, ip);  // Inicializa a Ethernet Shield
    server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
    Serial.begin(9600);
    dht.begin();

    Serial.println("Iniciando o cartao SD...");
    if (!SD.begin(4)) {
        Serial.println("ERRO - a inicializacao do cartao SD falhou!");
        return;    //aborta a função SETUP caso exista erro
    }
    
    Serial.println("Cartao iniciado com SUCESSO!!");
    // verifica a existencia do arquivo 1~1.htm no cartao SD
    if (!SD.exists("1~1.htm")) {
        Serial.println("ERRO - O arquivo 1~1.htm nao foi localizado!");
        return;   //aborta a função SETUP caso exista erro
    }
    
    Serial.println("Arquivo 1~1.htm encontrado!");
} // fim do setup

void loop() {
    EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)

    if (client) {  // Existe um cliente em conexão ?
      
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // os dados do cliente estão disponiveis para serem lidos
                char c = client.read(); // lê 1 byte (character) do cliente
                HTTP_req += c;  
                
                // a ultima linha da requisição do cliente é branca e termina com o caractere \n
                // responde para o cliente apenas após a última linha recebida
                if (c == '\n' && currentLineIsBlank) {
                  tempUmid(client);
                  // ENVIA A PÁGINA WEB
                    webFile = SD.open("1~1.htm");        // abre o arquivo da pagina WEB
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read());  // envia a pagina WEB para o cliente (browser)
                        }
                        webFile.close();
                    }
                    Serial.println("Página carregada");
                    break;
                  if ( mainPageRequest(&HTTP_req) ) {
                    URLValue = getURLRequest(&HTTP_req);

                    // envia o cabeçalho de uma resposta http padrão
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: keep-alive");
                    client.println();

                    // ENVIA A PÁGINA WEB
                    webFile = SD.open("1~1.htm");        // abre o arquivo da pagina WEB
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read());  // envia a pagina WEB para o cliente (browser)
                        }
                        webFile.close();
                    }
                    Serial.println("Página carregada");
                    break;
                  } else if (HTTP_req.indexOf("solicitacao_ajax") > -1) {     //<----- NOVO
                        Serial.println(HTTP_req);

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");      
                        client.println();                      

                        tempUmid(client);//roda funcao   
                  } else {
                        Serial.println(HTTP_req);
                        client.println("HTTP/1.1 200 OK");
                  }
                  HTTP_req = "";    
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

String getURLRequest(String *requisicao) {
int inicio, fim;
String retorno;

  inicio = requisicao->indexOf("GET") + 3;
  fim = requisicao->indexOf("HTTP/") - 1;
  retorno = requisicao->substring(inicio, fim);
  retorno.trim();

  return retorno;
}

bool mainPageRequest(String *requisicao) {
String valor;
bool retorno = false;

  valor = getURLRequest(requisicao);
  valor.toLowerCase();

  if (valor == "/") {
     retorno = true;
  }

  if (valor.substring(0,2) == "/?") {
     retorno = true;
  }  

  if (valor.substring(0,10) == "/1.html") {
     retorno = true;
  }  

  return retorno;
}

void tempUmid(EthernetClient cl) {
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t)) {
    cl.print("t_ino#ERRO|");
  } else {
    cl.print("t_ino#");
    cl.print(t);
    cl.print("|");
  }

  if (isnan(h)) {
    cl.print("u_ino#ERRO|");
  } else {
    cl.print("u_ino#");
    cl.print(h);
    cl.print("|");
  }
}
