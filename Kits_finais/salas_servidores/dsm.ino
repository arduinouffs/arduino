#define pin2 A4 // Vout2 PM10
#define pin1 2 // Vout1 PM2

#define sampletime_ms 3000 //sampe 1s ;
unsigned long duration1;
unsigned long duration2;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float pm2p5 = 0;
float pm10 = 0;

String read_ppm() {
  duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;
  
  ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
  pm2p5 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve
  
  ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
  pm10 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62;

  String request = "\",\"a\":{\"p\":\"";
  request += pm2p5; // partículas PM2.5
  request += "\",\"q\":\"";
  request += pm10; // partículas PM10ho
  request += "\"}";

  return request;
}
