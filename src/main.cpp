#include <Arduino.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>

// Variables
float tempValue = 0.0;
int potValue = 0;
int minTemp = -20;
int maxTemp = -16;
float setValue = 0.0;
// Data wire is conntec to the Arduino digital pin 4
const int tempPin = 4;
const int potPin = A0;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(tempPin);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Create LCD instance
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);
  // Start up the library
  sensors.begin();
  // Set IOs
  pinMode(A0, INPUT);

  // Set up LCD
  lcd.init(); // initialize the lcd
  lcd.backlight();
}

void loop() {
  // Call sensors.requestTemperatures() to issue a global temperature and
  // Requests to all devices on the bus
  sensors.requestTemperatures();
  tempValue = sensors.getTempCByIndex(0);
  potValue = analogRead(potPin);
  setValue = map(potValue, 0, 1024, maxTemp * 100, minTemp * 100) / 100.0;
  Serial.print("Celsius temperature: ");
  Serial.println(tempValue);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Valos:");
  lcd.print(tempValue);
  lcd.print("C");
  lcd.print((char)223);
  lcd.setCursor(0, 1);
  lcd.print("Cel:");
  lcd.print(setValue);
  lcd.print("C");
  lcd.print((char)223);

  delay(100);
}
