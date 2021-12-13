#define ledA 0
#define ledB 1
#define ledC 2
#define ledD 3
#define ledE 4
#define ledF 5
#define ledG 6
#define ponto 7
#define botao 8
int entrada = 0;
bool botaoAtu = false;
bool botaoAnt = false;

void setup()
{
  Serial.begin(9600);
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledC, OUTPUT);
  pinMode(ledD, OUTPUT);
  pinMode(ledE, OUTPUT);
  pinMode(ledF, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ponto, OUTPUT);
  pinMode(botao, INPUT);
  digitalWrite(ponto, HIGH);
}

void loop()
{
  if (Serial.available()) {
    entrada = Serial.parseInt();
    Serial.print(entrada);
    Serial.println("\n");
  }

  botaoAtu = digitalRead(botao);
  
  if (botaoAtu && !botaoAnt) {
  	entrada++;
    Serial.println("Somou: " + entrada);
  }
  botaoAnt = botaoAtu;
  
  if (entrada > 9) {
  	entrada = 0;
  }
  
  switch(entrada) {
  	case 0:
    	digitalWrite(ledA, HIGH);
    	digitalWrite(ledB, HIGH);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, HIGH);
    	digitalWrite(ledE, HIGH);
    	digitalWrite(ledF, HIGH);
    	digitalWrite(ledG, LOW);
    break;
      
    case 1:
    	digitalWrite(ledA, LOW);
    	digitalWrite(ledB, HIGH);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, LOW);
    	digitalWrite(ledE, LOW);
    	digitalWrite(ledF, LOW);
    	digitalWrite(ledG, LOW);
    break;
      
    case 2:
    	digitalWrite(ledA, HIGH);
    	digitalWrite(ledB, HIGH);
    	digitalWrite(ledC, LOW);
    	digitalWrite(ledD, HIGH);
    	digitalWrite(ledE, HIGH);
    	digitalWrite(ledF, LOW);
    	digitalWrite(ledG, HIGH);
    break;
      
    case 3:
    	digitalWrite(ledA, HIGH);
    	digitalWrite(ledB, HIGH);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, HIGH);
    	digitalWrite(ledE, LOW);
    	digitalWrite(ledF, LOW);
    	digitalWrite(ledG, HIGH);
    break;
      
    case 4:
    // F G B C
    	digitalWrite(ledA, LOW);
    	digitalWrite(ledB, HIGH);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, LOW);
    	digitalWrite(ledE, LOW);
    	digitalWrite(ledF, HIGH);
    	digitalWrite(ledG, HIGH);
    break;
      
    case 5:
    // A f g c d
    	digitalWrite(ledA, HIGH);
    	digitalWrite(ledB, LOW);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, HIGH);
    	digitalWrite(ledE, LOW);
    	digitalWrite(ledF, HIGH);
    	digitalWrite(ledG, HIGH);
    break;
      
    case 6:
    //a f g e c d
    	digitalWrite(ledA, HIGH);
    	digitalWrite(ledB, LOW);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, HIGH);
    	digitalWrite(ledE, HIGH);
    	digitalWrite(ledF, HIGH);
    	digitalWrite(ledG, HIGH);
    break;
      
    case 7:
    //a b c
    	digitalWrite(ledA, HIGH);
    	digitalWrite(ledB, HIGH);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, LOW);
    	digitalWrite(ledE, LOW);
    	digitalWrite(ledF, LOW);
    	digitalWrite(ledG, LOW);
    break;
      
    case 8:
    //
    	digitalWrite(ledA, HIGH);
    	digitalWrite(ledB, HIGH);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, HIGH);
    	digitalWrite(ledE, HIGH);
    	digitalWrite(ledF, HIGH);
    	digitalWrite(ledG, HIGH);
    break;
      
    case 9:
    //
    	digitalWrite(ledA, HIGH);
    	digitalWrite(ledB, HIGH);
    	digitalWrite(ledC, HIGH);
    	digitalWrite(ledD, HIGH);
    	digitalWrite(ledE, LOW);
    	digitalWrite(ledF, HIGH);
    	digitalWrite(ledG, HIGH);
    break;
      
    default:
    	digitalWrite(ledA, LOW);
    	digitalWrite(ledB, LOW);
    	digitalWrite(ledC, LOW);
    	digitalWrite(ledD, LOW);
    	digitalWrite(ledE, LOW);
    	digitalWrite(ledF, LOW);
    	digitalWrite(ledG, LOW);
    break;
  }
}