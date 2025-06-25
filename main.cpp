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
  Serial.println("Sistema de controle fuzzy contínuo iniciado.");
}

void loop() {
  tempC = dht.readTemperature();
  umidade = dht.readHumidity();

  if (isnan(tempC) || isnan(umidade)) {
    Serial.println("Falha na leitura do DHT11!");
    delay(2000);
    return;
  }

  // Pertinências de temperatura
  float pTempBaixa = pertinenciaTrapezoidal(tempC, 10, 15, 20, 25);   
  float pTempMedia = pertinenciaTriangular(tempC, 22, 30, 38);       
  float pTempAlta  = pertinenciaTrapezoidal(tempC, 34, 40, 45, 50);  

  // Cálculo contínuo de velocidade ponderada
  float numerador = (pTempBaixa * 60) + (pTempMedia * 140) + (pTempAlta * 255);
  float denominador = pTempBaixa + pTempMedia + pTempAlta;

  int velocidadeVentilador = 0;
  if (denominador > 0) {
    velocidadeVentilador = numerador / denominador;
  }

  // Garante torque mínimo se tiver velocidade não nula
  if (velocidadeVentilador > 0 && velocidadeVentilador < 60) {
    velocidadeVentilador = 60;
  }

  analogWrite(ventilador, velocidadeVentilador);

  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C | Umid: ");
  Serial.print(umidade);
  Serial.print("% | Velocidade: ");
  Serial.println(velocidadeVentilador);

  delay(500);
}
