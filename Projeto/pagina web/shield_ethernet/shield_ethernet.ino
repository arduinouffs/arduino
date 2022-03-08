#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <DHT.h>

#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT  
#define REQ_BUF_SZ   400

//char HTTP_req[REQ_BUF_SZ]; 
String HTTP_req;
int req_index = 0; 
//String URLValue;
bool connectedd = false;

//String getURLRequest(String *requisicao);
//bool mainPageRequest(String *requisicao);
void tempUmid(EthernetClient cl);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 40); // Endereço IP que a Ethernet Shield terá. Deve ser alterado para um endereço livre da sua rede.
EthernetServer server(80);     // Cria um servidor WEB
File webFile;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
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

    Ethernet.begin(mac, ip);  // Inicializa a Ethernet Shield
    server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
    dht.begin();

//    Serial.println(webFile = SD.open("1~1.htm"));
} // fim do setup

void loop() {
    EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)
//    Serial.println("a");
    if (client) {  // Existe um cliente em conexão ?
//        Serial.println("Entrou");
        if (!webFile) {
          webFile = SD.open("1~1.htm");  
        }
        
        boolean currentLineIsBlank = true;
        while (client.connected()) {          
            if (client.available()) {
                char c = client.read(); // lê 1 byte (character) do cliente
                HTTP_req.concat(c);
//                if (req_index < (400 - 1)) {
//                    HTTP_req[req_index] = c;          // save HTTP request character
//                    req_index++;
//                }
                                
                if (c == '\n' && currentLineIsBlank) {  
//                  if (StrContains(HTTP_req, "solicitacao_ajax")) {   
                  if (HTTP_req.indexOf("Ø") > -1 || HTTP_req.indexOf("%C3%98")) {
//                  if (connectedd) {                
                    tempUmid(client);//roda funcao
                  } else {
                    // ENVIA A PÁGINA WEB
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: keep-alive");
                    client.println();

                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read());  // envia a pagina WEB para o cliente (browser)
                        }
                        webFile.close();
                    }
                    Serial.println("Página carregada");
                    connectedd = true;
                  }


                  Serial.println("-----------------------------------------------------------");
                  Serial.println(HTTP_req);

//                  req_index = 0;
//                  StrClear(HTTP_req, REQ_BUF_SZ);
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

char StrContains(char *str, char *sfind) {
    char found = 0;
    char index = 0;
    char len;
 
    len = strlen(str);
   
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }
 
    return 0;
}

char StrClear(char *str, int width) {
    for (int i = 0; i < width; i++) {
      str[i] = "";
    }
    return str;
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
