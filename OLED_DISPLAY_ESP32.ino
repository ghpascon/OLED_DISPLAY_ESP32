#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 13
#define SCL_PIN 14
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

class OLEDManager {
private:
  Adafruit_SSD1306 display;
  bool initialized = false;
  unsigned long lastCheck = 0;
  const unsigned long checkInterval = 3000;

public:
  OLEDManager()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

  bool begin() {
    Wire.begin(SDA_PIN, SCL_PIN);

    if (!checkDevice()) {
      Serial.println("[OLED] Device not found");
      return false;
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
      Serial.println("[OLED] Init failed");
      return false;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();

    initialized = true;
    Serial.println("[OLED] Initialized");
    return true;
  }

  bool checkDevice() {
    Wire.beginTransmission(OLED_ADDR);
    return (Wire.endTransmission() == 0);
  }

  bool reconnect() {
    Serial.println("[OLED] Reconnecting I2C...");

    Wire.end();
    delay(50);
    Wire.begin(SDA_PIN, SCL_PIN);

    if (!checkDevice()) {
      return false;
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
      return false;
    }

    display.clearDisplay();
    display.display();

    initialized = true;
    Serial.println("[OLED] Reconnected OK");
    return true;
  }

  void update() {
    if (millis() - lastCheck < checkInterval)
      return;
    lastCheck = millis();

    if (!checkDevice()) {
      initialized = false;
      reconnect();
    }
  }

  void clear() {
    if (!initialized)
      return;
    display.clearDisplay();
  }

  void setText(int x, int y, String text, int size = 1) {
    if (!initialized)
      return;
    display.setTextSize(size);
    display.setCursor(x, y);
    display.print(text);
  }

  void drawLine(int x1, int y1, int x2, int y2) {
    if (!initialized)
      return;
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }

  void drawRect(int x, int y, int w, int h) {
    if (!initialized)
      return;
    display.drawRect(x, y, w, h, SSD1306_WHITE);
  }

  void fillRect(int x, int y, int w, int h) {
    if (!initialized)
      return;
    display.fillRect(x, y, w, h, SSD1306_WHITE);
  }

  void invert(bool inv) {
    if (!initialized)
      return;
    display.invertDisplay(inv);
  }

  void dim(bool dimOn) {
    if (!initialized)
      return;
    display.ssd1306_command(dimOn ? SSD1306_SETCONTRAST : 0x81);
  }

  void show() {
    if (!initialized)
      return;
    display.display();
  }

  bool isReady() {
    return initialized;
  }
};

OLEDManager oled;

void setup() {
  Serial.begin(115200);

  if (!oled.begin()) {
    Serial.println("OLED not detected, retrying...");
    delay(1000);
  }
}

void loop() {
  oled.update();

  static int counter = 0;
  counter++;

  String epc = "E28012341234123412341234";

  oled.clear();
  oled.setText(30, 0, "SMARTX", 2);
  oled.setText(0, 20, "TAGS - " + String(counter), 2);
  // show epc in 2 lines
  oled.setText(30, 45, epc.substring(0, epc.length() / 2), 1);
  oled.setText(30, 55, epc.substring(epc.length() / 2), 1);
  oled.show();

  delay(500);
}