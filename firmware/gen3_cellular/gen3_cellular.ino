#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SDA_PIN      8
#define SCL_PIN      9
#define TOUCH_PIN    2
#define BUZZER_PIN   3

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 failed"));
    for(;;);
  }
  
  // High tech Jingle
  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 1500, 80); delay(100);
  tone(BUZZER_PIN, 2000, 80); delay(100);
  tone(BUZZER_PIN, 3000, 200);
}

void loop() {
  display.clearDisplay();
  
  // Draw App Menu Grid
  display.drawRoundRect(0, 0, 128, 64, 4, SSD1306_WHITE);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(6, 6);
  display.print("ZYLO Gen 3 - CELLULAR");
  display.drawLine(0, 16, 128, 16, SSD1306_WHITE);
  
  // Signal Bars
  display.fillRect(112, 6, 2, 4, SSD1306_WHITE);
  display.fillRect(116, 4, 2, 6, SSD1306_WHITE);
  display.fillRect(120, 2, 2, 8, SSD1306_WHITE);
  
  display.setCursor(10, 22); display.print("[>] TikTok App");
  display.setCursor(10, 34); display.print("[ ] WhatsApp");
  display.setCursor(10, 46); display.print("[ ] Web Browser");
  
  display.display();
  delay(100);
}
