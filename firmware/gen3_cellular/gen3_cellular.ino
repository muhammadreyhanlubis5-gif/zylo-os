/*
 * ZYLO OS - GEN 3 (CELLULAR / SIM ENABLED)
 * License: ZYLO x ARAFAH
 * 
 * This firmware is designed for ESP32 with a Full Color LCD.
 * It uses TFT_eSPI for graphics.
 */

#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); 

#define BUZZER_PIN 3
#define TOUCH_PIN 2

int currentState = 1;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  
  tft.init();
  tft.setRotation(1);
  
  // Cellular Boot Sound
  tone(BUZZER_PIN, 1500, 80); delay(100);
  tone(BUZZER_PIN, 2000, 80); delay(100);
  tone(BUZZER_PIN, 3000, 200);
  
  drawCellularHome();
}

void drawCellularHome() {
  tft.fillScreen(TFT_BLACK);
  
  // Top Status Bar
  tft.fillRect(0, 0, 240, 24, tft.color565(20, 20, 20));
  tft.setTextColor(TFT_WHITE, tft.color565(20, 20, 20));
  tft.setTextDatum(TL_DATUM);
  tft.drawString("4G LTE", 5, 4, 2);
  
  // Signal Bars
  tft.fillRect(215, 12, 4, 8, TFT_GREEN);
  tft.fillRect(222, 8, 4, 12, TFT_GREEN);
  tft.fillRect(229, 4, 4, 16, TFT_DARKGREY); // 2 of 3 bars
  
  // Time
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("12:30", 120, 80, 7);
  
  // Data widget
  tft.fillRoundRect(20, 150, 90, 60, 8, tft.color565(0, 30, 0));
  tft.setTextColor(TFT_GREEN, tft.color565(0, 30, 0));
  tft.drawString("DATA", 65, 165, 2);
  tft.drawString("850 MB", 65, 190, 2);
  
  // Social widget
  tft.fillRoundRect(130, 150, 90, 60, 8, tft.color565(30, 0, 30));
  tft.setTextColor(TFT_MAGENTA, tft.color565(30, 0, 30));
  tft.drawString("TIKTOK", 175, 165, 2);
  tft.drawString("3 msgs", 175, 190, 2);
}

void drawAppGrid() {
  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("ALL APPS", 120, 10, 2);
  
  // Grid 2x2
  tft.fillRoundRect(30, 40, 70, 70, 15, TFT_DARKCYAN);
  tft.setTextColor(TFT_WHITE, TFT_DARKCYAN);
  tft.drawString("CALL", 65, 75, 2);
  
  tft.fillRoundRect(140, 40, 70, 70, 15, tft.color565(0, 150, 0)); // WhatsApp green
  tft.setTextColor(TFT_WHITE, tft.color565(0, 150, 0));
  tft.drawString("CHAT", 175, 75, 2);
  
  tft.fillRoundRect(30, 130, 70, 70, 15, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawString("WEB", 65, 165, 2);
  
  tft.fillRoundRect(140, 130, 70, 70, 15, TFT_ORANGE);
  tft.setTextColor(TFT_WHITE, TFT_ORANGE);
  tft.drawString("IG", 175, 165, 2);
}

void loop() {
  bool touched = digitalRead(TOUCH_PIN) == HIGH;
  
  if (touched) {
    tone(BUZZER_PIN, 2000, 50);
    if (currentState == 1) {
      currentState = 2;
      drawAppGrid();
    } else {
      currentState = 1;
      drawCellularHome();
    }
    delay(500); 
  }
}
