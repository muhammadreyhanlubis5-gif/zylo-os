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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Boot Jingle
int bootMelody[] = { 1047, 1319, 1568, 2093 }; 
int noteDurations[] = { 150, 150, 150, 300 };

// ==========================================
// EYE STATE MACHINE
// ==========================================
enum EyeState {
  STATE_NORMAL,
  STATE_BLINK,
  STATE_HAPPY,
  STATE_SURPRISED,
  STATE_SAD,
  STATE_ANGRY,
  STATE_SLEEP,
  STATE_COOL,
  STATE_LOVE,
  STATE_DIZZY,
  STATE_SKEPTICAL
};

EyeState currentState = STATE_NORMAL;
unsigned long lastStateChange = 0;
bool isTouched = false;
bool wasTouched = false;

// ==========================================
// BOOT SCREEN ANIMATION
// ==========================================
void showBootScreen() {
  display.clearDisplay();
  // Frame
  display.drawRoundRect(2, 2, 124, 60, 4, SSD1306_WHITE);
  
  display.setTextColor(SSD1306_WHITE);
  int16_t x1, y1; uint16_t w, h;
  
  // Z Y L O
  display.setTextSize(2);
  display.getTextBounds("Z Y L O", 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 12);
  display.print("Z Y L O");
  
  // x A R A F A H
  display.setTextSize(1);
  display.getTextBounds("x A R A F A H", 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 32);
  display.print("x A R A F A H");
  display.display();
  
  // Bracket for loading bar [       ]
  display.setCursor(34, 45); display.print("[");
  display.setCursor(88, 45); display.print("]");
  display.display();
  
  // Animate dots and play jingle
  int startX = 44; 
  int dotSpacing = 12;
  
  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, bootMelody[i], noteDurations[i]);
    display.fillCircle(startX + (i * dotSpacing), 48, 3, SSD1306_WHITE);
    display.display();
    delay(noteDurations[i] * 1.5);
  }
  noTone(BUZZER_PIN);
  delay(1000); 
}

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Wire.begin(SDA_PIN, SCL_PIN);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 failed"));
    for(;;);
  }
  
  showBootScreen();
  // Random seed for expressions
  randomSeed(analogRead(0));
  lastStateChange = millis();
}

// ==========================================
// EYE RENDER FUNCTIONS
// ==========================================
void drawNormalEyes() {
  display.clearDisplay();
  display.fillRoundRect(24, 20, 24, 24, 4, SSD1306_WHITE);
  display.fillRoundRect(80, 20, 24, 24, 4, SSD1306_WHITE);
  display.display();
}

void drawBlinkEyes() {
  display.clearDisplay();
  display.fillRoundRect(24, 30, 24, 4, 2, SSD1306_WHITE);
  display.fillRoundRect(80, 30, 24, 4, 2, SSD1306_WHITE);
  display.display();
}

void drawSleepEyes() {
  display.clearDisplay();
  display.fillRoundRect(24, 30, 24, 6, 3, SSD1306_WHITE);
  display.fillRoundRect(80, 30, 24, 6, 3, SSD1306_WHITE);
  // Add some zZZ
  display.setTextSize(1);
  display.setCursor(100, 10); display.print("z");
  display.setCursor(110, 5); display.print("Z");
  display.display();
}

void drawHappyEyes() {
  display.clearDisplay();
  display.fillCircle(36, 32, 12, SSD1306_WHITE);
  display.fillCircle(36, 36, 12, SSD1306_BLACK); 
  display.fillCircle(92, 32, 12, SSD1306_WHITE);
  display.fillCircle(92, 36, 12, SSD1306_BLACK);
  display.display();
}

void drawSurprisedEyes() {
  display.clearDisplay();
  display.fillCircle(36, 32, 14, SSD1306_WHITE);
  display.fillCircle(36, 32, 4, SSD1306_BLACK); 
  display.fillCircle(92, 32, 14, SSD1306_WHITE);
  display.fillCircle(92, 32, 4, SSD1306_BLACK);
  
  // Open Mouth
  display.fillCircle(64, 48, 6, SSD1306_WHITE);
  display.display();
}

void drawSadEyes() {
  display.clearDisplay();
  // Sad eyes (angled inwards)
  display.fillRoundRect(24, 24, 24, 16, 8, SSD1306_WHITE);
  display.fillTriangle(24, 24, 48, 24, 24, 12, SSD1306_BLACK); // cut corner
  
  display.fillRoundRect(80, 24, 24, 16, 8, SSD1306_WHITE);
  display.fillTriangle(80, 24, 104, 24, 104, 12, SSD1306_BLACK); // cut corner
  
  // Sad mouth
  display.drawFastHLine(54, 50, 20, SSD1306_WHITE);
  display.drawFastHLine(55, 51, 18, SSD1306_WHITE);
  display.display();
}

void drawAngryEyes() {
  display.clearDisplay();
  display.fillRoundRect(24, 24, 24, 16, 4, SSD1306_WHITE);
  display.fillTriangle(24, 24, 48, 24, 48, 12, SSD1306_BLACK); 
  
  display.fillRoundRect(80, 24, 24, 16, 4, SSD1306_WHITE);
  display.fillTriangle(80, 24, 104, 24, 80, 12, SSD1306_BLACK); 
  display.display();
}

void drawCoolEyes() {
  display.clearDisplay();
  // Sunglasses frame
  display.fillRoundRect(20, 24, 32, 16, 4, SSD1306_WHITE);
  display.fillRoundRect(76, 24, 32, 16, 4, SSD1306_WHITE);
  // Bridge
  display.drawFastHLine(52, 28, 24, SSD1306_WHITE);
  display.drawFastHLine(52, 29, 24, SSD1306_WHITE);
  // Smirk
  display.drawFastHLine(58, 48, 12, SSD1306_WHITE);
  display.fillTriangle(70, 48, 74, 44, 74, 48, SSD1306_WHITE);
  display.display();
}

void drawLoveEyes() {
  display.clearDisplay();
  // Left Heart
  display.fillCircle(30, 24, 8, SSD1306_WHITE);
  display.fillCircle(42, 24, 8, SSD1306_WHITE);
  display.fillTriangle(22, 28, 50, 28, 36, 42, SSD1306_WHITE);
  // Right Heart
  display.fillCircle(86, 24, 8, SSD1306_WHITE);
  display.fillCircle(98, 24, 8, SSD1306_WHITE);
  display.fillTriangle(78, 28, 106, 28, 92, 42, SSD1306_WHITE);
  display.display();
}

void drawDizzyEyes() {
  display.clearDisplay();
  // X eyes
  display.drawLine(24, 20, 48, 44, SSD1306_WHITE);
  display.drawLine(24, 44, 48, 20, SSD1306_WHITE);
  display.drawLine(25, 20, 49, 44, SSD1306_WHITE);
  display.drawLine(25, 44, 49, 20, SSD1306_WHITE);
  
  display.drawLine(80, 20, 104, 44, SSD1306_WHITE);
  display.drawLine(80, 44, 104, 20, SSD1306_WHITE);
  display.drawLine(81, 20, 105, 44, SSD1306_WHITE);
  display.drawLine(81, 44, 105, 20, SSD1306_WHITE);
  
  // Wavy mouth
  display.drawFastHLine(54, 52, 6, SSD1306_WHITE);
  display.drawFastHLine(60, 50, 8, SSD1306_WHITE);
  display.drawFastHLine(68, 52, 6, SSD1306_WHITE);
  display.display();
}

void drawSkepticalEyes() {
  display.clearDisplay();
  // Left eye squinting
  display.fillRoundRect(24, 30, 24, 8, 2, SSD1306_WHITE);
  // Right eye wide
  display.fillCircle(92, 30, 14, SSD1306_WHITE);
  display.fillCircle(92, 30, 4, SSD1306_BLACK);
  // Mouth
  display.drawLine(58, 50, 70, 46, SSD1306_WHITE);
  display.drawLine(58, 51, 70, 47, SSD1306_WHITE);
  display.display();
}

void playChatter(int baseFreq, int duration, int count) {
  int step = duration / count;
  for(int i=0; i<count; i++) {
    tone(BUZZER_PIN, baseFreq + random(-300, 300), step - 10);
    delay(step);
  }
  noTone(BUZZER_PIN);
}

void playEmotionSound(EyeState emotion) {
  if(emotion == STATE_HAPPY) {
    playChatter(2200, 400, 5);
  } else if (emotion == STATE_SURPRISED) {
    tone(BUZZER_PIN, 1200, 50); delay(80); tone(BUZZER_PIN, 1800, 50); delay(80); tone(BUZZER_PIN, 2400, 100);
  } else if (emotion == STATE_SAD) {
    tone(BUZZER_PIN, 800, 200); delay(220); tone(BUZZER_PIN, 600, 300);
  } else if (emotion == STATE_ANGRY) {
    playChatter(400, 500, 8); // low angry chatter
  } else if (emotion == STATE_SLEEP) {
    tone(BUZZER_PIN, 400, 400); delay(450); tone(BUZZER_PIN, 350, 500);
  } else if (emotion == STATE_COOL) {
    tone(BUZZER_PIN, 1500, 100); delay(150); tone(BUZZER_PIN, 1200, 150);
  } else if (emotion == STATE_LOVE) {
    playChatter(2500, 600, 6); // cute high chatter
  } else if (emotion == STATE_DIZZY) {
    tone(BUZZER_PIN, 1800, 100); delay(120); tone(BUZZER_PIN, 1400, 100); delay(120); tone(BUZZER_PIN, 1000, 200);
  } else if (emotion == STATE_SKEPTICAL) {
    tone(BUZZER_PIN, 1200, 300);
  }
}

void loop() {
  isTouched = digitalRead(TOUCH_PIN) == HIGH;
  
  // Trigger new emotion on fresh touch
  if (isTouched && !wasTouched) {
    // Pick random emotion from Happy (2) to Skeptical (10)
    int randEmotion = random(2, 11);
    currentState = (EyeState)randEmotion;
    
    // Switch case to render immediately before delay
    switch (currentState) {
      case STATE_HAPPY: drawHappyEyes(); break;
      case STATE_SURPRISED: drawSurprisedEyes(); break;
      case STATE_SAD: drawSadEyes(); break;
      case STATE_ANGRY: drawAngryEyes(); break;
      case STATE_SLEEP: drawSleepEyes(); break;
      case STATE_COOL: drawCoolEyes(); break;
      case STATE_LOVE: drawLoveEyes(); break;
      case STATE_DIZZY: drawDizzyEyes(); break;
      case STATE_SKEPTICAL: drawSkepticalEyes(); break;
    }
    
    playEmotionSound(currentState);
    lastStateChange = millis();
  } 

  else if (!isTouched) {
    // Return to normal after 3 seconds of an emotion
    if (currentState >= STATE_HAPPY && (millis() - lastStateChange > 3000)) {
      currentState = STATE_NORMAL;
      lastStateChange = millis();
    }
    
    // Normal blink logic
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
  
  wasTouched = isTouched;
  
  // Continuous render for normal states
  if(currentState == STATE_NORMAL) drawNormalEyes();
  else if(currentState == STATE_BLINK) drawBlinkEyes();
  
  delay(50);
}
