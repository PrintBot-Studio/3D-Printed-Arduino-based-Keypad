#define HID_CUSTOM_LAYOUT
#define LAYOUT_US_ENGLISH
#include <HID-Project.h>
#include <HID-Settings.h>



unsigned long lastTimeButtonStateChanged[7];
unsigned long debounceDuration = 300;



#define volumePin A0
uint8_t prevVolume = 0; 
unsigned long lastVolumeUpdatedTime = millis();


void setup() {
  Serial.begin(9600);
  NKROKeyboard.begin();

  for (byte pinNum = 2; pinNum <= 6; pinNum++) {
    pinMode(pinNum, INPUT);
    lastTimeButtonStateChanged[pinNum] = millis();
  }
}


void keyMap(uint8_t mappedButton, uint8_t n, ...) {
  uint8_t buttonState = digitalRead(mappedButton);

  if (buttonState && millis() - lastTimeButtonStateChanged[mappedButton] >= debounceDuration) {
    lastTimeButtonStateChanged[mappedButton] = millis();

    va_list args;
    va_start(args, n);

    for (uint8_t i = 0; i < n; i++) {
      uint8_t key = va_arg(args, int);
      NKROKeyboard.press(KeyboardKeycode(key));
    }

    delay(10);

    va_start(args, n);
    for (uint8_t i = 0; i < n; i++) {
      uint8_t key = va_arg(args, int);
      NKROKeyboard.release(KeyboardKeycode(key));
    }

    va_end(args);
  }
}



void loop() {

  keyMap(2, 3, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_LEFT_ARROW);
  keyMap(3, 3, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_RIGHT_ARROW);
  keyMap(4, 2, KEY_LEFT_GUI, KEY_TAB);
  keyMap(5, 1, KEY_RETURN);

  // Serial.println(digitalRead(4));
}