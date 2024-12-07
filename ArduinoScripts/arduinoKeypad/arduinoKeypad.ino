#define HID_CUSTOM_LAYOUT
#define LAYOUT_US_ENGLISH
#include <HID-Project.h>
#include <HID-Settings.h>



unsigned long lastTimeButtonStateChanged[7];
unsigned long debounceDuration = 300;



#define volumePin A0
uint8_t prevVolume = 0;
unsigned long lastVolumeUpdateTime = millis();

#define XPin A1
#define YPin A2
unsigned long lastGUiUpdateTime = millis();



void setup() {
  Serial.begin(9600);
  NKROKeyboard.begin();
  Consumer.begin();

  pinMode(volumePin, INPUT);
  pinMode(XPin, INPUT);
  pinMode(YPin, INPUT);

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

void sliderVolume() {
  int potValue = analogRead(volumePin);
  int volume = map(potValue, 0, 1023, 0, 100);

  if (abs(volume - prevVolume) >= 3 && millis() - lastVolumeUpdateTime >= 20) {
    if (volume > prevVolume) {
      Consumer.write(MEDIA_VOLUME_UP);
      Consumer.write(MEDIA_VOLUME_UP);
    } else {
      Consumer.write(MEDIA_VOLUME_DOWN);
      Consumer.write(MEDIA_VOLUME_DOWN);
    }

    prevVolume = volume;
    lastVolumeUpdateTime = millis();
  }
}

void loop() {

  keyMap(2, 3, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_LEFT_ARROW);
  keyMap(3, 3, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_RIGHT_ARROW);
  keyMap(4, 2, KEY_LEFT_GUI, KEY_TAB);
  keyMap(5, 1, KEY_RETURN);
  sliderVolume();

  int x = map(analogRead(A1), 0, 1023, 2, 0);
  int y = map(analogRead(A2), 0, 1023, 2, 0);


  if ((x != 1 || y != 1) && millis() - lastGUiUpdateTime >= 50) {
    lastGUiUpdateTime = millis();
    Serial.println(x);
    Serial.println(y);
    if (x == 0) {
      NKROKeyboard.press(KeyboardKeycode(KEY_LEFT_GUI));
      NKROKeyboard.press(KeyboardKeycode(KEY_LEFT_CTRL));
      NKROKeyboard.press(KeyboardKeycode(KEY_LEFT_ARROW));
      delay(10);
      NKROKeyboard.release(KeyboardKeycode(KEY_LEFT_GUI));
      NKROKeyboard.release(KeyboardKeycode(KEY_LEFT_CTRL));
      NKROKeyboard.release(KeyboardKeycode(KEY_LEFT_ARROW));

    } else if (x == 2) {
      NKROKeyboard.press(KeyboardKeycode(KEY_LEFT_GUI));
      NKROKeyboard.press(KeyboardKeycode(KEY_LEFT_CTRL));
      NKROKeyboard.press(KeyboardKeycode(KEY_RIGHT_ARROW));
      delay(10);
      NKROKeyboard.release(KeyboardKeycode(KEY_LEFT_GUI));
      NKROKeyboard.release(KeyboardKeycode(KEY_LEFT_CTRL));
      NKROKeyboard.release(KeyboardKeycode(KEY_RIGHT_ARROW));
    }

    if (y == 1) {
      NKROKeyboard.press(KeyboardKeycode(KEY_LEFT_GUI));
      NKROKeyboard.press(KeyboardKeycode(KEY_TAB));
      delay(10);
      NKROKeyboard.release(KeyboardKeycode(KEY_LEFT_GUI));
      NKROKeyboard.release(KeyboardKeycode(KEY_TAB));
    }
  }

  // delay(200);
}