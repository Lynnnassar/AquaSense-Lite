# 💧 AquaSense Lite

### Arduino-Based Water Quality Monitoring System

**AquaSense Lite** is a compact Arduino-based water quality monitoring system designed to measure and display three important water parameters:

* 🧪 **pH level**
* 🌡️ **Water temperature**
* 💧 **Water turbidity**

The measurements are collected using analog and digital sensors and displayed in real time on a **16×2 I2C LCD**.

---

## 📸 Project Overview

AquaSense Lite uses an **Arduino Uno** as the main controller. The system continuously reads the connected sensors and updates the LCD every 1.5 seconds.

### System Architecture

```text
                  ┌─────────────────┐
                  │   Arduino Uno   │
                  │                 │
                  │                 │
        ┌─────────┤ A0          A1  ├─────────┐
        │         │                 │         │
        │         │ D2              │         │
        │         │                 │         │
        ▼         └────────┬────────┘         ▼
   ┌─────────┐             │            ┌─────────────┐
   │ pH      │             │            │  Turbidity  │
   │ Sensor  │             │            │   Sensor    │
   └─────────┘             │            └─────────────┘
                           ▼
                    ┌─────────────┐
                    │   DS18B20   │
                    │ Temperature │
                    └─────────────┘

                           │
                           │ I2C
                           ▼
                    ┌─────────────┐
                    │   16×2 LCD  │
                    │   I2C 0x27  │
                    └─────────────┘
```

---

## 🧰 Components

| Component                  |    Quantity |
| -------------------------- | ----------: |
| Arduino Uno                |           1 |
| 16×2 I2C LCD               |           1 |
| DS18B20 Temperature Sensor |           1 |
| pH Sensor Module + Probe   |           1 |
| Turbidity Sensor           |           1 |
| 4.7kΩ Resistor             |           1 |
| Jumper Wires               | As required |
| Breadboard                 |           1 |

---

## 🔌 Pin Connections

| Component            | Pin           | Arduino |
| -------------------- | ------------- | ------- |
| **DS18B20**          | Data          | D2      |
| **pH Sensor**        | Analog Output | A0      |
| **Turbidity Sensor** | Analog Output | A1      |
| **LCD**              | SDA           | A4      |
| **LCD**              | SCL           | A5      |
| **LCD**              | VCC           | 5V      |
| **LCD**              | GND           | GND     |

### DS18B20

The DS18B20 uses the **OneWire protocol**.

A **4.7kΩ pull-up resistor** should be connected between the DS18B20's **DATA** line and **5V**.

```text
Arduino 5V ───────┐
                  │
                4.7kΩ
                  │
Arduino D2 ───────┴──── DS18B20 DATA
Arduino 5V ──────────── DS18B20 VCC
Arduino GND ─────────── DS18B20 GND
```

### I2C LCD

The LCD communicates with the Arduino through I2C.

For an Arduino Uno:

```text
LCD VCC → 5V
LCD GND → GND
LCD SDA → A4
LCD SCL → A5
```

The current code assumes the LCD's I2C address is:

```text
0x27
```

If your LCD uses a different address, the address in the code must be changed.

---

## 📚 Required Libraries

The project requires the following Arduino libraries:

### Wire

Used for I2C communication with the LCD.

```cpp
#include <Wire.h>
```

### LiquidCrystal_I2C

Used to control the 16×2 I2C LCD.

```cpp
#include <LiquidCrystal_I2C.h>
```

### OneWire

Used for communication with the DS18B20 temperature sensor.

```cpp
#include <OneWire.h>
```

### DallasTemperature

Provides an easier interface for reading the DS18B20 temperature.

```cpp
#include <DallasTemperature.h>
```

### Installing Libraries

In the Arduino IDE:

**Sketch → Include Library → Manage Libraries**

Search for and install:

* `LiquidCrystal I2C`
* `OneWire`
* `DallasTemperature`

`Wire` is normally included with the Arduino IDE.

---

## ⚙️ How It Works

The Arduino continuously performs three measurements.

### 🌡️ 1. Temperature

The DS18B20 is connected to digital pin **D2**.

The program requests a temperature measurement:

```cpp
sensors.requestTemperatures();
```

It then retrieves the temperature in Celsius:

```cpp
float tempC = sensors.getTempCByIndex(0);
```

The result is displayed with one decimal place.

Example:

```text
T:25.3C
```

---

### 🧪 2. pH

The pH sensor is connected to analog pin **A0**.

The Arduino reads the sensor's analog output:

```cpp
int phRaw = analogRead(PH_PIN);
```

The 10-bit Arduino ADC produces a value between:

```text
0 → 1023
```

The raw value is converted to voltage:

```cpp
float phVoltage = phRaw * 5.0 / 1023.0;
```

The current prototype then estimates the pH using the following calibration formula:

```cpp
float pHValue = 7.0 + ((phVoltage - 2.63) / -0.18);
```

This formula is a **basic calibration approximation** and should be adjusted using known pH buffer solutions for better accuracy.

---

### 💧 3. Turbidity

The turbidity sensor is connected to analog pin **A1**.

The Arduino reads its analog output:

```cpp
int turbRaw = analogRead(TURBIDITY_PIN);
```

The reading is then mapped to an estimated turbidity value:

```cpp
int turbidity = map(turbRaw, 0, 1023, 1000, 0);
```

The mapping assumes:

```text
Higher sensor reading → clearer water
Lower sensor reading  → more turbid water
```

The resulting value is displayed as an estimated NTU value.

> **Important:** The current turbidity calculation is a simple linear mapping, not a laboratory-accurate NTU calibration. Actual turbidity sensors generally require calibration against known standards.

---

## 🖥️ LCD Output

The 16×2 LCD displays all three measurements.

Example:

```text
T:25.3C pH:7.1
Turb:450 NTU
```

The display is refreshed every **1.5 seconds**.

```cpp
delay(1500);
```

---

## 📁 Project Structure

```text
AquaSense-Lite/
│
├── AquaSense-Lite.ino
├── README.md
│
└── images/
    └── circuit-diagram.png
```

The `images` folder can contain the project's circuit diagram, prototype photos, and other documentation.

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR-USERNAME/AquaSense-Lite.git
```

### 2. Open the Project

Open:

```text
AquaSense-Lite.ino
```

using the **Arduino IDE**.

### 3. Install the Required Libraries

Install:

* LiquidCrystal_I2C
* OneWire
* DallasTemperature

### 4. Connect the Hardware

Connect the sensors and LCD according to the pin configuration described above.

### 5. Select the Arduino Board

In Arduino IDE:

```text
Tools → Board → Arduino AVR Boards → Arduino Uno
```

Select the appropriate COM port under:

```text
Tools → Port
```

### 6. Upload

Click:

```text
Upload
```

After uploading, the LCD should begin displaying the sensor readings.

---

## 🔧 Calibration

### pH Calibration

The current system uses a basic calibration equation:

```cpp
pH = 7.0 + ((voltage - 2.63) / -0.18)
```

This should **not be considered a universal pH calibration formula**.

For improved accuracy, the sensor should be calibrated using known pH buffer solutions.

A **two-point calibration**, such as pH 4 and pH 7, can provide a significantly better result than relying on a fixed formula.

### Turbidity Calibration

The current turbidity calculation uses a simple 0–1023 mapping.

For more accurate measurements, the sensor should be calibrated using water samples or standards with known turbidity values.

---

## ⚠️ Current Limitations

AquaSense Lite is currently a **prototype / educational project**.

The following limitations should be considered:

* pH readings depend on sensor calibration.
* Turbidity values are estimated using a simple linear mapping.
* Sensor accuracy depends on the quality of the hardware.
* The system currently does not store measurements.
* The system does not provide remote monitoring.
* The LCD can only display a limited amount of information at once.
* The system is not intended to replace laboratory water-quality analysis.

---

## 🔮 Future Improvements

Possible future versions could include:

* [ ] Two-point pH calibration
* [ ] More accurate turbidity calibration
* [ ] Automatic sensor calibration
* [ ] Water-quality status indicator
* [ ] Buzzer/LED warning system
* [ ] SD card data logging
* [ ] Real-time graphs
* [ ] Bluetooth connectivity
* [ ] Wi-Fi connectivity
* [ ] Web-based monitoring dashboard
* [ ] Mobile application
* [ ] Historical measurement storage

---

## 🎯 Project Goals

The main goal of AquaSense Lite is to demonstrate how an Arduino can combine multiple sensors into a single embedded monitoring system.

The project focuses on:

* Sensor integration
* Analog signal reading
* Digital sensor communication
* I2C communication
* Real-time data processing
* LCD-based user interfaces
* Basic sensor calibration

---

## 📄 License

This project is intended for educational and personal use.

You are free to modify and improve the project for learning and experimentation.

---

## 👤 Author

**Lynn Nassar**

*AquaSense Lite — Arduino Water Quality Monitoring System*
