#include <SPI.h>
#include <SD.h>
#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>

File myFile;
IRsendRaw mySender;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");  
}

void loop() {
  uint16_t rawData[350];
  for (int j = 0; j < 350; j++) {
    rawData[j] = 0;
  }
  String string = "";
  String entrada = "KOMECCO,OFF,196";
  bool chave = false;
  bool id = true;
  bool copy = false;
  int i = 0;
  myFile = SD.open("comandos.txt");
  if (myFile) {
    while (myFile.available()) {
      char c = myFile.read();
      if (c == '{') {
        chave = true;
        continue;
      }
      if (c == '}') chave = false;

      if (chave == false && copy == true) break;
      
      if (chave && id) {
        if (c != '\n') string += c;
        else {
          if (string == entrada) {
            copy = true;
            id = false;
          } else {
            chave = false;
          }
          string = "";
        }
      }

      if (chave && copy) {
        if (c != ',') string += c;
        else {
          rawData[i] = string.toInt();
          Serial.println(string);
          string = "";
          i++;
        }
      }

    }
    i = 0;
    myFile.close();
  } else {
    Serial.println("error opening comandos.txt");
  }  
  
  for (int j = 0; j < 350; j++) {
    Serial.println(rawData[j]);
  }
  delay(9999999);
}
