#include <Keyboard.h>
#include <KeyboardLayout.h>


char KEY[7] = { NULL, NULL, 'A', 'B', 'C', 'D', 'E' };
unsigned long lastTimeButtonStateChanged[7];
unsigned long debounceDuration = 300;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();

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
      Keyboard.press(key);
    }

    delay(10);

    va_start(args, n);
    for (uint8_t i = 0; i < n; i++) {
      uint8_t key = va_arg(args, int);
      Keyboard.release(key);
    }

    va_end(args);
  }
}


void loop() {

  keyMap(2, 3, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_LEFT_ARROW);
  keyMap(3, 3, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_RIGHT_ARROW);
  keyMap(4, 2, KEY_LEFT_GUI, KEY_TAB);
  keyMap(5, 1, KEY_RETURN);

  Serial.println(digitalRead(4));
}