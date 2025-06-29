# 🌾 Sistema de Controle de Clima para Silos – Protótipo com Arduino

## 📌 Objetivo

Este projeto tem como objetivo controlar **automaticamente a ventilação de um silo**, mantendo o ambiente **seco** e **fresco**, a fim de preservar a qualidade de grãos armazenados. O sistema responde dinamicamente à **temperatura** e **umidade relativa do ar** internas, atuando em um ventilador com velocidade variável.

---

## 🧠 Problema que o projeto resolve

Em silos de armazenamento, **excesso de calor** e **umidade** são fatores críticos que podem causar a deterioração de grãos, como milho e soja. Fungos, germinação indesejada e perda de qualidade podem ocorrer rapidamente se essas variáveis não forem bem controladas.

A climatização manual é imprecisa e ineficiente. Logo, este sistema propõe uma **automação inteligente**, capaz de ajustar a ventilação com base nas condições internas do silo.

---

## 🧪 Solução Proposta

- Foi desenvolvido um **protótipo funcional** em maquete com um **Arduino UNO**, sensores e um ventilador.
- Um **sensor DHT11** faz a leitura da temperatura e da umidade do ar dentro do silo.
- Um algoritmo de **lógica fuzzy** calcula a velocidade ideal para o ventilador.
- A saída PWM do Arduino controla a intensidade da ventilação conforme as leituras dos sensores.

---

## 🛠️ Materiais Utilizados

| Item                     | Quantidade |
|--------------------------|------------|
| Arduino UNO              | 1          |
| Sensor DHT11 (temp/umid) | 1          |
| Ventilador DC 12V      | 1          |
| Transistor TIP31C        | 1          |
| Resistores  (1kΩ)        | 1     |
| Diodo UF4007 | 1 |
| Fonte externa 9V         | 1          |
| Protoboard e jumpers     | Diversos   |
| Balde metálico Maquete | 1   |

---

## 🧪 Lógica de Controle

O controle fuzzy foi implementado com regras como:

- Se a **temperatura e umidade forem muito baixas**, o ventilador é desligado.
- Se forem **altas**, o ventilador atinge máxima potência.
- Em valores intermediários, a velocidade varia suavemente.

Essa abordagem evita picos abruptos, reduz ruído e consumo de energia, prolongando a vida útil do sistema.

---

## 🧰 Aplicações Reais

- Silos agrícolas (milho, soja, arroz, etc.)
- Estufas e galpões de secagem
- Ambientes que exigem controle climático passivo e de baixo custo

---

## 📸 Fotos da Maquete
![IMG_20250625_121524428](https://github.com/user-attachments/assets/02d4f704-343c-4039-9de3-f9c2b6e340ee)

![IMG_20250625_121444652](https://github.com/user-attachments/assets/b52a30b5-3f09-4e69-90c7-a5b7520565d6)

![IMG_20250625_121504157](https://github.com/user-attachments/assets/acd43b74-cac1-4c56-950e-874c733bb2ac)



---

## 🔄 Futuras Melhorias

- Substituição do sensor DHT11 por DHT22 (maior precisão)
- Adição de display OLED para visualização local
- Integração com ESP8266/ESP32 para monitoramento via Wi-Fi
- Atuação em bomba d’água em vez de ventilador (versão para estufas)
