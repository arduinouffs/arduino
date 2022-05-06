#include <IRremote.h>

// DIRETIVAS DE COMPILAÇÃO
#define tempoTecla 350
#define frequencia 38 // kHz

#define pinReceptor 2

// INSTANCIANDO OBJETOS
IRrecv receptorIR(pinReceptor);
IRsend emissorIR;

unsigned int rawDataOff[]={
450,1700, 500,1700, 500,1650, 550,1650, 500,1650, 500,1700, 500,1650, 500,1700, 500,650, 450,650, 500,600, 500,650, 500,600, 500,600, 500,650, 500,600, 500,1650, 500,1700, 450,1700, 550,1650, 500,1650, 500,1700, 500,1700, 450,1700, 500,600, 500,650, 500,600, 500,650, 450,650, 500,600, 500,650, 500,600, 450,1750, 450,1700, 450,1750, 450,1700, 500,1700, 450,1700, 500,1700, 450,1700, 500,650, 450,650, 450,700, 450,650, 450,650, 500,650, 400,700, 450,650, 450,1750, 450,1700 
};

void setup() {
  Serial.begin(9600);

  // INICIANDO RECEPTOR IR
//  receptorIR.enableIRIn();
  Serial.print("Setup Concluído");
}

void loop() {
  emissorIR.sendRaw(rawDataOff, sizeof(rawDataOff) / sizeof(rawDataOff[0]), frequencia);
  delay(1000);
}
