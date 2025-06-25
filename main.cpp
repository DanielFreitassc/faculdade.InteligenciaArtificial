#include <DHT.h>

#define DHTPIN 8
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int ventilador = 9;

float tempC = 0;
int umidade = 0;

float pertinenciaTriangular(float x, float a, float b, float c) {
  if (x <= a || x >= c) return 0;
  else if (x == b) return 1;
  else if (x > a && x < b) return (x - a) / (b - a);
  else return (c - x) / (c - b);
}

float pertinenciaTrapezoidal(float x, float a, float b, float c, float d) {
  if (x <= a || x >= d) return 0;
  else if (x >= b && x <= c) return 1;
  else if (x > a && x < b) return (x - a) / (b - a);
  else return (d - x) / (d - c);
}


void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(ventilador, OUTPUT);
  Serial.println("Sistema de controle de ventilador iniciado.");
}

void loop() {
  tempC = dht.readTemperature();
  umidade = dht.readHumidity();

  if (isnan(tempC) || isnan(umidade)) {
    Serial.println("Falha na leitura do DHT11!");
    delay(2000);
    return;
  }

  float tempMuitoBaixa = pertinenciaTrapezoidal(tempC, -15, -5, 10, 20); 
  float tempBaixa = pertinenciaTriangular(tempC, 15, 22, 30);
  float tempMedia = pertinenciaTriangular(tempC, 28, 35, 42);
  float tempAlta  = pertinenciaTriangular(tempC, 40, 48, 55);
  float tempMuitoAlta = pertinenciaTrapezoidal(tempC, 50, 60, 70, 80); 

  float umidMuitoBaixa = pertinenciaTrapezoidal(umidade, -15, -5, 15, 35);
  float umidBaixa = pertinenciaTriangular(umidade, 30, 45, 60);
  float umidMedia = pertinenciaTriangular(umidade, 55, 70, 85);
  float umidAlta  = pertinenciaTriangular(umidade, 80, 95, 100); 
  float umidMuitoAlta = pertinenciaTrapezoidal(umidade, 90, 100, 110, 120); 

  float regra1_fanOff = max(tempMuitoBaixa, umidMuitoBaixa);

  float regra2_fanMuitoBaixo = max(tempBaixa, umidBaixa);

  float regra3_fanBaixo = max(tempMedia, umidMedia);

  float regra4_fanMedio = max(tempAlta, umidAlta);

  float regra5_fanAlto = max(tempMuitoAlta, umidMuitoAlta);

  float numerador = (regra1_fanOff * 0) +
                    (regra2_fanMuitoBaixo * 30) +   
                    (regra3_fanBaixo * 80) +         
                    (regra4_fanMedio * 150) +        
                    (regra5_fanAlto * 255);

  float denominador = regra1_fanOff +
                      regra2_fanMuitoBaixo +
                      regra3_fanBaixo +
                      regra4_fanMedio +
                      regra5_fanAlto;

  int velocidadeVentilador = 0;
  if (denominador > 0) {
    velocidadeVentilador = numerador / denominador;
  }

  velocidadeVentilador = constrain(velocidadeVentilador, 0, 255);
  analogWrite(ventilador, velocidadeVentilador);

  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C | Umid: ");
  Serial.print(umidade);
  Serial.print("% | Velocidade do Ventilador: ");
  Serial.println(velocidadeVentilador);

  delay(200);
}