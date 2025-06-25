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
  Serial.println("Sistema de controle fuzzy com temperatura e umidade.");
}

void loop() {
  tempC = dht.readTemperature();
  umidade = dht.readHumidity();

  if (isnan(tempC) || isnan(umidade)) {
    Serial.println("Falha na leitura do DHT11!");
    delay(2000);
    return;
  }

  float pTempBaixa = pertinenciaTrapezoidal(tempC, 10, 15, 20, 25);
  float pTempMedia = pertinenciaTriangular(tempC, 22, 30, 38);
  float pTempAlta  = pertinenciaTrapezoidal(tempC, 38, 44, 52, 65);  

  float pUmidBaixa = pertinenciaTriangular(umidade, 30, 45, 60);
  float pUmidMedia = pertinenciaTriangular(umidade, 55, 70, 85);
  float pUmidAlta  = pertinenciaTrapezoidal(umidade, 80, 90, 100, 110);

  float numTemp = (pTempBaixa * 60) + (pTempMedia * 140) + (pTempAlta * 255);
  float denTemp = pTempBaixa + pTempMedia + pTempAlta;

  float numUmid = (pUmidBaixa * 80) + (pUmidMedia * 120) + (pUmidAlta * 200);
  float denUmid = pUmidBaixa + pUmidMedia + pUmidAlta;

  float velocidadeVentilador = 0;
  if (denTemp > 0 || denUmid > 0) {
    float tempResult = (denTemp > 0) ? (numTemp / denTemp) : 0;
    float umidResult = (denUmid > 0) ? (numUmid / denUmid) : 0;

    if (denUmid == 0) {
      velocidadeVentilador = tempResult;
    } else {
      velocidadeVentilador = (tempResult * 0.7 + umidResult * 0.3);
    }
  }

  if (velocidadeVentilador > 0 && velocidadeVentilador < 40) {
    velocidadeVentilador = 40;
  }

  // Segurança térmica
  if (tempC >= 50.0) {
    velocidadeVentilador = 255;
  }


  analogWrite(ventilador, (int)velocidadeVentilador);

  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C | Umid: ");
  Serial.print(umidade);
  Serial.print("% | PWM: ");
  Serial.println((int)velocidadeVentilador);

  delay(500);
}
