/* send.ino Example sketch for IRLib2
 *  Illustrates how to send a code.
 */
#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>    
#include <IRLib_P02_Sony.h>   
#include <IRLibCombo.h>

#define off_ 0xff00ff00ff00df205ba454ab // a mao
#define on_ 0xff00ff00ff009f605ba454ab // a mao
#define dehumidify_ 0xff00ff00ff009f605da254ab // a mao
#define khz 36
#define off 0xff00ff00ff00db245ba45
#define on 0xff00ff00ff009b645ba45
#define dehumidify 0xff00ff00ff00a15e6d925

IRsend mySender;

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  Serial.println(F("Every time you press a key is a serial monitor we will send."));
}

void loop() {
//  if (Serial.read() != -1) {
//    //send a code every time a character is received from the 
//    // serial port. You could modify this sketch to send when you
//    // push a button connected to an digital input pin.
//    //Substitute values and protocols in the following statement
//    // for device you have available.
//    mySender.send(UNKNOWN,0xff00ff00ff00a15e6d925, 36);//Sony DVD power A8BCA, 20 bits
//    //mySender.send(NEC,0x61a0f00f,0);//NEC TV power button=0x61a0f00f
//    Serial.println(F("Sent signal."));
//  }
  mySender.send(NEC,off_, 36); // 
  delay(3000);
}
