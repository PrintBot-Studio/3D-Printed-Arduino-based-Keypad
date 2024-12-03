
#define BUTTON_PIN 2
#define BUTTON_LIGHT 3
#define METER A0

bool toggle = 0;

void setup()
{
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUTTON_LIGHT, OUTPUT);

    Serial.begin(9600);
}

void loop()
{
    int meterValue = analogRead(METER);
    int normalValue = meterValue / (1023 / 255);

    int buttonClicked = digitalRead(BUTTON_PIN);
    if (buttonClicked && toggle)
    {
        toggle = 0;
    }
    else if (buttonClicked && !toggle)
    {
        toggle = 1;
    }

    if (toggle)
    {
        analogWrite(BUTTON_LIGHT, normalValue);
    }
    else
        analogWrite(BUTTON_LIGHT, 0);

    delay(10);
}