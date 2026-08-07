#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// ==========================================
// ZYLO ESP32-C3 PINOUT CONFIGURATION
// ==========================================
#define SDA_PIN      8
#define SCL_PIN      9
#define TOUCH_PIN    2
#define BUZZER_PIN   3

// OLED Display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==========================================
// ZYLO BOOT JINGLE
// ==========================================
int bootMelody[] = { 1047, 1319, 1568, 2093 }; // C6, E6, G6, C7
int noteDurations[] = { 150, 150, 150, 300 };

// ==========================================
// EYE STATE MACHINE
// ==========================================
enum EyeState {
  STATE_NORMAL,
  STATE_BLINK,
  STATE_HAPPY,
  STATE_SURPRISED
};

EyeState currentState = STATE_NORMAL;
unsigned long lastStateChange = 0;
bool isTouched = false;

// Function to play the boot jingle
void playBootJingle() {
  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, bootMelody[i], noteDurations[i]);
    delay(noteDurations[i] * 1.3); 
  }
  noTone(BUZZER_PIN);
}

// Function to show ZYLO boot screen
void showBootScreen() {
  display.clearDisplay();
  
  // Outer Box Frame
  display.drawRoundRect(10, 5, 108, 54, 5, SSD1306_WHITE);
  
  // ZYLO Logo Text
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 15);
  display.print("ZYLO");
  
  // Subtext
  display.setTextSize(1);
  display.setCursor(25, 45);
  display.print("MINI ROBOT OS");
  
  display.display();
  
  // Play the jingle while showing the logo
  playBootJingle();
  delay(1500); // Hold boot screen for a moment
}

void setup() {
  Serial.begin(115200);
  
  // Initialize Pins
  pinMode(TOUCH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize I2C for OLED
  Wire.begin(SDA_PIN, SCL_PIN);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Display ZYLO Boot Sequence
  showBootScreen();
  lastStateChange = millis();
}

// ==========================================
// EYE RENDER FUNCTIONS
// ==========================================
void drawNormalEyes() {
  display.clearDisplay();
  // Left eye
  display.fillRoundRect(24, 20, 24, 24, 4, SSD1306_WHITE);
  // Right eye
  display.fillRoundRect(80, 20, 24, 24, 4, SSD1306_WHITE);
  display.display();
}

void drawBlinkEyes() {
  display.clearDisplay();
  // Left eye (closed/squinted)
  display.fillRoundRect(24, 30, 24, 4, 2, SSD1306_WHITE);
  // Right eye (closed/squinted)
  display.fillRoundRect(80, 30, 24, 4, 2, SSD1306_WHITE);
  display.display();
}

void drawHappyEyes() {
  display.clearDisplay();
  // Left eye happy curve
  display.fillCircle(36, 32, 12, SSD1306_WHITE);
  display.fillCircle(36, 36, 12, SSD1306_BLACK); // Mask bottom part
  
  // Right eye happy curve
  display.fillCircle(92, 32, 12, SSD1306_WHITE);
  display.fillCircle(92, 36, 12, SSD1306_BLACK);
  
  display.display();
}

void drawSurprisedEyes() {
  display.clearDisplay();
  // Left eye wide
  display.fillCircle(36, 32, 14, SSD1306_WHITE);
  display.fillCircle(36, 32, 4, SSD1306_BLACK); // Pupil
  
  // Right eye wide
  display.fillCircle(92, 32, 14, SSD1306_WHITE);
  display.fillCircle(92, 32, 4, SSD1306_BLACK);
  
  display.display();
}

void loop() {
  // 1. Read touch sensor
  isTouched = digitalRead(TOUCH_PIN) == HIGH;
  
  // 2. Logic & State Transitions
  if (isTouched) {
    if (currentState != STATE_HAPPY) {
      currentState = STATE_HAPPY;
      // Happy chirp sound
      tone(BUZZER_PIN, 2000, 100);
      delay(150);
      tone(BUZZER_PIN, 2500, 150);
    }
  } else {
    // Revert to normal if previously happy
    if (currentState == STATE_HAPPY) {
      currentState = STATE_NORMAL;
      lastStateChange = millis();
    }
    
    // Blink logic (blink every ~3 seconds)
    if (millis() - lastStateChange > 3000) {
      if (currentState == STATE_NORMAL) {
        currentState = STATE_BLINK;
        lastStateChange = millis();
      } else if (currentState == STATE_BLINK && (millis() - lastStateChange > 200)) {
        currentState = STATE_NORMAL;
        lastStateChange = millis();
      }
    }
  }
  
  // 3. Render appropriate state
  switch (currentState) {
    case STATE_NORMAL:
      drawNormalEyes();
      break;
    case STATE_BLINK:
      drawBlinkEyes();
      break;
    case STATE_HAPPY:
      drawHappyEyes();
      break;
    case STATE_SURPRISED:
      drawSurprisedEyes();
      break;
  }
  
  // 4. Short delay to debounce and save CPU
  delay(50);
}
