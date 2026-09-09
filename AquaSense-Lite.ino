// AquaSense Lite: Arduino-based Water Quality Monitoring System
// Measures pH, temperature, and turbidity, displaying results on a 16x2 I2C LCD

#include <Wire.h>                   // I2C communication library
#include <LiquidCrystal_I2C.h>      // LCD library for I2C 16x2 display
#include <OneWire.h>                // OneWire library for temperature sensor communication
#include <DallasTemperature.h>      // Dallas Temperature library for DS18B20 sensor

// Initialize the LCD object with I2C address 0x27, 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the digital pin connected to the OneWire bus for DS18B20 temperature sensor
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);        // Setup OneWire instance for DS18B20 communication
DallasTemperature sensors(&oneWire);  // Pass OneWire reference to DallasTemperature library

// Define analog input pins connected to the pH and turbidity sensors
#define PH_PIN A0
#define TURBIDITY_PIN A1

void setup() {
  Serial.begin(9600);       // Initialize serial communication for debugging at 9600 baud rate

  // Initialize the LCD display
  lcd.begin(16, 2);         // Set up LCD dimensions: 16 columns, 2 rows
  lcd.backlight();          // Turn on the LCD backlight for visibility

  // Initialize temperature sensor communication
  sensors.begin();          // Starts communication with DS18B20 temperature sensor
}

void loop() {
  // --- Temperature Measurement ---
  sensors.requestTemperatures();               // Send command to DS18B20 to perform temperature measurement
  float tempC = sensors.getTempCByIndex(0);   // Retrieve temperature in Celsius from the first sensor found

  // --- pH Measurement ---
  int phRaw = analogRead(PH_PIN);              // Read raw analog value from pH sensor (0-1023)
  float phVoltage = phRaw * 5.0 / 1023.0;      // Convert raw ADC value to voltage (5V reference)
  
  /* 
     Convert voltage to pH value:
     - Typical pH sensor outputs ~2.5V at pH 7 (neutral)
     - Calibration formula: pH = 7.0 + ((voltage - 2.63) / -0.18)
  */
  float pHValue = 7.0 + ((phVoltage - 2.63) / -0.18);

  // --- Turbidity Measurement ---
  int turbRaw = analogRead(TURBIDITY_PIN);         // Read raw analog value from turbidity sensor
  float turbVoltage = turbRaw * 5.0 / 1023.0;      // Convert raw ADC value to voltage

  /*
     Map the raw sensor value to turbidity in NTU (Nephelometric Turbidity Units):
     - Higher raw values correspond to clearer water (lower turbidity)
     - Mapping inversely: 0 (max voltage) -> 1000 NTU (max turbidity), 1023 (min voltage) -> 0 NTU (clear)
  */
  int turbidity = map(turbRaw, 0, 1023, 1000, 0);

  // --- Display Results on LCD ---
  lcd.clear();                          // Clear previous LCD data

  // Display temperature and pH on first line
  lcd.setCursor(0, 0);                  // Set cursor to first column, first row
  lcd.print("T:");                     // Label for temperature
  lcd.print(tempC, 1);                 // Print temperature with 1 decimal place
  lcd.print("C pH:");                  // Label for pH
  lcd.print(pHValue, 1);               // Print pH value with 1 decimal place

  // Display turbidity on second line
  lcd.setCursor(0, 1);                  // Set cursor to first column, second row
  lcd.print("Turb:");                  // Label for turbidity
  lcd.print(turbidity);                // Print turbidity value (NTU)
  lcd.print(" NTU");                   // Unit label for turbidity

  delay(1500);                        // Pause for 1.5 seconds before next reading update
}
