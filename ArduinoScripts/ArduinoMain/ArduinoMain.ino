#include "HID-Project.h"

const int pinLed = LED_BUILTIN;
const int pinButton = 2;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);

  // Sends a clean report to the host. This is important on any Arduino type.
  NKROKeyboard.begin();
}

void typeKey(KeyboardKeycode key){
    NKROKeyboard.press(key);
    delay(50);
    NKROKeyboard.release(key);
}

void loop() {
  delay(5000);
  typeKey(KEY_RIGHT_WINDOWS);
}