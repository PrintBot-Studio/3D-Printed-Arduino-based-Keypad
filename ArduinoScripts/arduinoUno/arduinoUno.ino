#define LIGHT_PIN 3
#define PUSH_BUTTON_PIN 2
#define POTENSIO_ALOG_METER A0

byte lastButtonState;
byte ledState = LOW;

unsigned long lastTimeButtonStateChanged = millis();
unsigned long debounceDuration = 50;


void setup() {
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(PUSH_BUTTON_PIN, INPUT);
  pinMode(POTENSIO_ALOG_METER, INPUT);
  lastButtonState = digitalRead(PUSH_BUTTON_PIN);
}

void loop() {
  int potensioInputVal = analogRead(POTENSIO_ALOG_METER);

  if (millis() - lastTimeButtonStateChanged >= debounceDuration) {
    byte buttonState = digitalRead(PUSH_BUTTON_PIN);
    if (buttonState != lastButtonState) {
      lastTimeButtonStateChanged = millis();
      lastButtonState = buttonState;
      if (buttonState == LOW) {
        if (ledState == HIGH) {
          ledState = LOW;
        } else {
          ledState = HIGH;
        }
      }

      Serial.println(buttonState);
    }
  }

  analogWrite(LIGHT_PIN, ledState == HIGH ? potensioInputVal / (1023 / 255) : 0);
  delay(50);

}