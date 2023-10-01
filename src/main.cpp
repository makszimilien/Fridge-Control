#include <Arduino.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>

// Variables
float tempValue = 0.0;
float tempHyst = 0.7;
bool cooolingOn = false;

int potValue = 0;
int potValueCurr = 0;
int potValueHyst = 5;
float setValue = 0.0;

unsigned long prevChangeMillis = 0;
const long backlightTimeout = 5000;

int minTemp = 20;
int maxTemp = 30;
// int minTemp = -20;
// int maxTemp = -16;

const int tempPin = 4;
const int potPin = A0;
const int ssrPin = 5;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(tempPin);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Create LCD instance
LiquidCrystal_I2C lcd(0x27, 16, 2);

void writeLcd() {

  setValue = map(potValueCurr, 0, 1024, maxTemp * 100, minTemp * 100) / 100.0;

  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Valos:");
  lcd.print(tempValue, 1);
  lcd.print("C");
  lcd.print((char)223);
  lcd.setCursor(0, 1);
  lcd.print("Cel:");
  lcd.print(setValue, 1);
  lcd.print("C");
  lcd.print((char)223);

  Serial.print("Celsius temperature: ");
  Serial.println(tempValue);
};

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);
  // Start up the library
  sensors.begin();
  // Set IOs
  pinMode(A0, INPUT);

  // Set up LCD
  lcd.init(); // initialize the lcd
}

void loop() {
  // Get measured temp value
  sensors.requestTemperatures();
  tempValue = sensors.getTempCByIndex(0);

  potValue = analogRead(potPin);

  unsigned long currentMillis = millis();

  // Handle potmeter change
  if (abs(potValue - potValueCurr) > potValueHyst) {
    potValueCurr = potValue;

    prevChangeMillis = currentMillis;

    writeLcd();
  };

  // Turn off LCD on timeout
  if (currentMillis - prevChangeMillis > backlightTimeout) {
    lcd.noBacklight();
  }

  // Temp control
  if (tempValue + tempHyst < setValue && cooolingOn) {
    cooolingOn = false;
  } else if (tempValue - tempHyst > setValue && !cooolingOn) {
    cooolingOn = true;
  }

  digitalWrite(ssrPin, cooolingOn);
}
