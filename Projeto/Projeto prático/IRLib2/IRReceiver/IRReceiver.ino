#include <IRLibRecv.h>
IRrecv MyReceiver(2); //Create the receiver. Use pin 2
#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLibCombo.h>
IRdecode MyDecoder;

void setup() {
 Serial.begin(9600);
 MyReceiver.enableIRIn();//start receiving
}

void loop() {
 if (MyReceiver.getResults()) {//wait till it returns true
 MyDecoder.decode();
 MyDecoder.dumpResults();
 MyReceiver.enableIRIn(); //restart the receiver
 }
}
