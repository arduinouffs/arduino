#define on    "111111110000000011111111000000001111111100000000100110110110010001011011101001000101"
#define dehum "111111110000000011111111000000001111111100000000101000010101111001101101100100100101"
#define off   "111111110000000011111111000000001111111100000000110110110010010001011011101001000101"
#define test  "111111110000000011111111000000001111111100000000110111110010000001011011101001000101010010101011"

#define pin 9

void NEC (String command);

void setup() {
  pinMode(pin, OUTPUT);
  Serial.begin(115200);
  NEC(on);
  delay(4000);
  NEC(dehum);
  delay(4000);
  NEC(off);
}

void loop() {

}

void NEC (String command) {
  digitalWrite(pin, false);
//   cabeçalho 
  digitalWrite(pin, true);
  delay(9);
  digitalWrite(pin, false);
  delayMicroseconds(4500);
//   comandos
  for (short i = 0; i < command.length(); i++) {
    digitalWrite(pin, true);
    delayMicroseconds(560);
    if (command[i] == '1') {
      digitalWrite(pin, false);
      delayMicroseconds(1690);
    } else { // == 0
      digitalWrite(pin, false);
      delayMicroseconds(565);
    }
  }
}
