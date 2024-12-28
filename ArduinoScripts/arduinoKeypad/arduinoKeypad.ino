#define HID_CUSTOM_LAYOUT
#define LAYOUT_US_ENGLISH
#include <HID-Project.h>
#include <HID-Settings.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


unsigned long lastTimeButtonStateChanged[16];
unsigned long debounceDuration = 300;
unsigned long lastVolumeUpdateTime = millis();

const int BUTTON_MAPPED[3][3] = {
  { 10, 9, 8 },
  { 7, 6, 5 },
  { 4, 16, 14 }
};
#define BUZZER 15

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  NKROKeyboard.begin();
  Consumer.begin();


  lcd.begin();
  lcd.backlight();
  lcd.print("      NEMO");
  lcd.setCursor(0, 1);
  lcd.print("    Keyboard");

  pinMode(BUZZER, OUTPUT);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      lastTimeButtonStateChanged[BUTTON_MAPPED[i][j]] = millis();
    }
  }
}


void keyMap(uint8_t mappedPin, uint8_t n, ...) {
  uint8_t buttonState = digitalRead(mappedPin);

  if (buttonState && millis() - lastTimeButtonStateChanged[mappedPin] >= debounceDuration) {
    lastTimeButtonStateChanged[mappedPin] = millis();

    va_list args;
    va_start(args, n);

    for (uint8_t i = 0; i < n; i++) {
      uint8_t key = va_arg(args, int);
      NKROKeyboard.press(KeyboardKeycode(key));
    }

    digitalWrite(BUZZER, HIGH);
    delay(10);
    digitalWrite(BUZZER, LOW);

    va_start(args, n);
    for (uint8_t i = 0; i < n; i++) {
      uint8_t key = va_arg(args, int);
      NKROKeyboard.release(KeyboardKeycode(key));
    }

    va_end(args);
  }
}

void loop() {

  keyMap(BUTTON_MAPPED[1][0], 3, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_LEFT_ARROW);
  keyMap(BUTTON_MAPPED[1][2], 3, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_RIGHT_ARROW);
  keyMap(BUTTON_MAPPED[1][1], 2, KEY_LEFT_GUI, KEY_TAB);
  keyMap(BUTTON_MAPPED[0][1], 1, KEY_RETURN);

  if (millis() - lastVolumeUpdateTime >= 150) {
    if (digitalRead(BUTTON_MAPPED[2][2])) {
      Consumer.write(MEDIA_VOLUME_UP);
      lastVolumeUpdateTime = millis();

    } else if (digitalRead(BUTTON_MAPPED[2][1])) {
      Consumer.write(MEDIA_VOLUME_DOWN);
      lastVolumeUpdateTime = millis();
    }
  }
}