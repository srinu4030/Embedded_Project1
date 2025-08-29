#include <LiquidCrystal.h>

// LCD pins: RS=7, E=6, D4=5, D5=4, D6=3, D7=2
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Pin definitions
const int btnLeft = 8;
const int btnRight = 9;
const int btnMode = 10;
const int buzzer = 12;
const int led = 11;

int position = 0;          // Character position
bool autoMode = false;     // Mode flag
int direction = 1;         // For auto movement
unsigned long lastDebounce = 0;
const int debounceDelay = 200;

byte customChar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
void setup() {
  lcd.begin(16, 2);             // Initialize 16x2 LCD
  lcd.createChar(0, customChar);
  lcd.setCursor(position, 0);
  lcd.write((byte)0);

  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnMode, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  if (!autoMode) {
    // Manual Mode
    if (digitalRead(btnLeft) == LOW && millis() - lastDebounce > debounceDelay) {
      moveChar(-1);
      lastDebounce = millis();
    }
    if (digitalRead(btnRight) == LOW && millis() - lastDebounce > debounceDelay) {
      moveChar(1);
      lastDebounce = millis();
    }
  } else {
    // Automatic Mode
    static unsigned long lastMove = 0;
    if (millis() - lastMove > 400) {
      moveChar(direction);
      if (position == 0 || position == 15) direction = -direction;
      lastMove = millis();
    }
  }

  // Mode toggle
  if (digitalRead(btnMode) == LOW && millis() - lastDebounce > debounceDelay) {
    autoMode = !autoMode;
    if (autoMode) enterAutoMode();
    lastDebounce = millis();
  }
}

void moveChar(int dir) {
  lcd.clear();
  position += dir;
  if (position < 0) position = 0;
  if (position > 15) position = 15;
  lcd.setCursor(position, 0);
  lcd.write((byte)0);
}

void enterAutoMode() {
  // 3 beeps and 3 blinks
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
    delay(200);
  }
}
