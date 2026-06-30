# ESP32 OLED Manager (SSD1306 I2C)

Projeto simples e robusto para controle de display OLED 128x64 via I2C com ESP32.

## 📌 Características

- Driver SSD1306 (0x3C)
- I2C nos pinos personalizados:
  - SDA = GPIO 13
  - SCL = GPIO 14
- Classe encapsulada (`OLEDManager`)
- Reconexão automática do I2C
- Reinitialização do display em falhas
- Funções básicas de desenho
- Proteção contra uso antes da inicialização

---

## 🔌 Hardware

| OLED Pin | ESP32 |
|----------|-------|
| VCC      | 3.3V  |
| GND      | GND   |
| SDA      | GPIO 13 |
| SCL      | GPIO 14 |

---

## 🧠 Funcionalidades da Classe

```cpp
bool begin();              // inicializa display
void update();            // monitora falhas I2C
void clear();             // limpa buffer
void setText(x,y,text);   // escreve texto
void drawLine(...);       // desenha linha
void drawRect(...);       // retângulo
void fillRect(...);       // retângulo preenchido
void invert(bool);       // inverte cores
void dim(bool);          // reduz brilho
void show();             // envia buffer ao display
bool isReady();          // status do display