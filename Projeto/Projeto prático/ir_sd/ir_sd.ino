/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>
#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>

File myFile;
IRsendRaw mySender;

uint16_t rawData[350];

void readSD(uint16_t *rawData[350], String entrada);

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
  
//  readSD(rawData[350], "KOMECCO,OFF,196");/

  for (int j = 0; j < 350; j++) {
    Serial.println(rawData[j]);
  }
}

void loop() {
  // nothing happens after setup
  delay(2000);
  mySender.send(rawData,350,36);
}

void readSD(uint16_t *rawData[350], String entrada){
//  for (int i = 0; i < 350; i++) rawData[i] = 0;///
  String string = "";
//  String entrada = "KOMECCO,OFF,196";/
  bool chave = false;
  bool id = true;
  bool copy = false;
  int i = 0;
  myFile = SD.open("comandos.txt");
  if (myFile) {
    Serial.println("comandos.txt:");
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
}
