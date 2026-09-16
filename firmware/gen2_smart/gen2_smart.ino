/*
 * ZYLO OS - GEN 2 (SMART AI & COLOR LCD)
 * License: ZYLO x ARAFAH
 * 
 * This firmware is designed for ESP32 with a Full Color LCD (e.g., ST7789 / GC9A01).
 * It uses TFT_eSPI for high-performance graphics.
 */

#include <SPI.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#define BUZZER_PIN 3
#define TOUCH_PIN 2

int currentState = 0; // 0 = Boot, 1 = Watchface, 2 = Menu

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  
  tft.init();
  tft.setRotation(1); // Set orientation
  tft.fillScreen(TFT_BLACK);
  
  // 1. BOOT ANIMATION
  drawBootScreen();
  delay(2000);
  
  currentState = 1;
}

void drawBootScreen() {
  tft.fillScreen(TFT_BLACK);
  
  // Draw futuristic circle
  tft.drawCircle(120, 120, 80, TFT_GREEN);
  tft.drawCircle(120, 120, 78, TFT_DARKGREEN);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM); // Middle center
  
  tft.drawString("ZYLO OS", 120, 100, 4); // Font 4
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("x ARAFAH", 120, 130, 2); // Font 2
  
  tft.setTextColor(TFT_DARKGREEN, TFT_BLACK);
  tft.drawString("SECURE BOOT V2", 120, 160, 1);
  
  // Startup sound
  tone(BUZZER_PIN, 1200, 100); delay(150);
  tone(BUZZER_PIN, 1800, 150); delay(200);
  tone(BUZZER_PIN, 2400, 300);
}

void drawWatchFace() {
  tft.fillScreen(TFT_BLACK);
  
  // Draw Spider/Hero theme background elements
  tft.fillTriangle(0, 0, 240, 0, 120, 120, tft.color565(40, 0, 0)); // Dark red top
  tft.drawLine(0, 0, 240, 240, TFT_RED);
  tft.drawLine(240, 0, 0, 240, TFT_RED);
  
  // Time Box
  tft.fillRoundRect(40, 80, 160, 80, 10, TFT_BLACK);
  tft.drawRoundRect(40, 80, 160, 80, 10, TFT_RED);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("12:30", 120, 110, 7); // Large 7-segment font
  
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString("AM", 120, 145, 2);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("BAT: 80%", 120, 200, 2);
}

void drawAppMenu() {
  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM); // Top Center
  tft.drawString("APPS", 120, 10, 2);
  tft.drawLine(20, 30, 220, 30, TFT_DARKGREY);
  
  // App 1: Grok AI
  tft.fillRoundRect(30, 50, 60, 60, 12, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawString("AI", 60, 80, 2);
  
  // App 2: Wallpaper
  tft.fillRoundRect(150, 50, 60, 60, 12, TFT_MAGENTA);
  tft.setTextColor(TFT_WHITE, TFT_MAGENTA);
  tft.drawString("WLP", 180, 80, 2);
  
  // App 3: Health
  tft.fillRoundRect(30, 140, 60, 60, 12, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.drawString("BPM", 60, 170, 2);
  
  // App 4: Settings
  tft.fillRoundRect(150, 140, 60, 60, 12, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.drawString("CFG", 180, 170, 2);
}

void loop() {
  bool touched = digitalRead(TOUCH_PIN) == HIGH;
  
  if (touched) {
    tone(BUZZER_PIN, 2000, 50);
    // Toggle state
    if (currentState == 1) {
      currentState = 2;
      drawAppMenu();
    } else {
      currentState = 1;
      drawWatchFace();
    }
    delay(500); // debounce
  }
  
  // Initial draw after boot
  static bool firstRun = true;
  if (firstRun && currentState == 1) {
    drawWatchFace();
    firstRun = false;
  }
}
