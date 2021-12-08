int delay_ = 20;

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(7, INPUT);
}

void loop()
{
  int estadoBotao = digitalRead(7);
  
  digitalWrite(13, estadoBotao);
  digitalWrite(12, estadoBotao);
 
}