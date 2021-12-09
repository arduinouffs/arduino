int semaforoVermelho = 10;
int semaforoAmarelo = 9;
int semaforoVerde = 8;
int pedestreVerde = 1;
int pedestreVermelo = 0;
int botao = 7;

int modo = 0;

int estadoAnterior;

int tempo = 0;
int estadoLed = HIGH;

void setup()
{
  pinMode(semaforoVerde, OUTPUT);
  pinMode(semaforoAmarelo, OUTPUT);
  pinMode(semaforoVerde, OUTPUT);
  pinMode(pedestreVerde, OUTPUT);
  pinMode(pedestreVermelo, OUTPUT);
  pinMode(botao, INPUT);

  estadoAnterior = digitalRead(botao);
}

void loop()
{
  int estadoBotao = digitalRead(botao);
  
  // (estadoBotao == HIGH) && (estadoAnterior == LOW)
  if ((estadoBotao == HIGH) && (estadoAnterior == LOW)) {
    if (modo < 3) {
      modo++;
    } else {
      modo = 0;
    }
  }

  estadoAnterior = estadoBotao;

  if (modo == 0) {
    digitalWrite(semaforoVermelho, HIGH);
    digitalWrite(semaforoAmarelo, LOW);
    digitalWrite(semaforoVerde, LOW);

    digitalWrite(pedestreVermelo, LOW);
    digitalWrite(pedestreVerde, HIGH);
  }
  else if (modo == 1) {
    digitalWrite(semaforoVermelho, LOW);
    digitalWrite(semaforoAmarelo, HIGH);
    digitalWrite(semaforoVerde, LOW);

    digitalWrite(pedestreVermelo, HIGH);
    digitalWrite(pedestreVerde, LOW);
  }
  else if (modo == 2) {
    digitalWrite(semaforoVermelho, LOW);
    digitalWrite(semaforoAmarelo, LOW);
    digitalWrite(semaforoVerde, HIGH);
  }
  else {
    digitalWrite(pedestreVerde, estadoLed);
    
    if (tempo == 400) {
      tempo = 0;
      estadoLed = !estadoLed;
    } else {
      tempo++;
    }
    delay(1);
  }
}