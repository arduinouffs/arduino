void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  float novo = analogRead(A5);
  float velho = analogRead(A4);
  Serial.print("Novo: "); Serial.print(novo); Serial.print(", Velho: "); Serial.println(velho);
  delay(1000);
}
