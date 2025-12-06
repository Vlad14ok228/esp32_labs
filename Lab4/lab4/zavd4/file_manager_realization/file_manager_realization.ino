#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"
#include "SPIFFS.h"

// === Піни ===
#define ONE_WIRE_BUS 4     // DS18B20
#define LDR_PIN 34         // фоторезистор (ADC)
#define SDA_PIN 21
#define SCL_PIN 22

// === Ініціалізація ===
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
Adafruit_BMP280 bmp;

// === Таймінг ===
const unsigned long interval = 3000; // 3 секунди
unsigned long lastTime = 0;

// === Глобальні змінні для сенсорів ===
float tempDS = 0.0;
float tempBMP = 0.0;
float pressure = 0.0;
int light = 0;

// === Ініціалізація SPIFFS ===
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed!");
    while (1);
  }
  Serial.println("SPIFFS mounted successfully.");
}

// === Функція запису у файл ===
void writeToFile(String dataLine) {
  File file = SPIFFS.open("/data.txt", FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for writing!");
    return;
  }
  file.println(dataLine);
  file.close();
}

// === Функція читання файлу ===
void readFromFile() {
  File file = SPIFFS.open("/data.txt");
  if (!file) {
    Serial.println("Failed to open file for reading!");
    return;
  }

  Serial.println("\n=== File contents ===");
  while (file.available()) {
    Serial.write(file.read());
  }
  Serial.println("\n=====================\n");
  file.close();
}

// === SETUP ===
void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("\nESP32 Sensor Data Logger to SPIFFS");

  // 1-Wire (DS18B20)
  ds18b20.begin();

  // BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 not found!");
    while (1);
  }

  // SPIFFS
  initSPIFFS();

  // Створення нового файлу з заголовком
  File file = SPIFFS.open("/data.txt", FILE_WRITE);
  file.println("===== NEW SESSION STARTED =====");
  file.println("Time(ms), Temp_DS18B20(C), Temp_BMP280(C), Pressure(hPa), Light(ADC)");
  file.println("================================");
  file.close();
}

// === LOOP ===
void loop() {
  unsigned long now = millis();

  if (now - lastTime >= interval) {
    lastTime = now;

    // === 1) Температура DS18B20 ===
    ds18b20.requestTemperatures();
    tempDS = ds18b20.getTempCByIndex(0);

    // === 2) BMP280 ===
    tempBMP = bmp.readTemperature();
    pressure = bmp.readPressure() / 100.0F;

    // === 3) LDR ===
    light = analogRead(LDR_PIN);

    // === 4) Формування рядка ===
    String line = 
      "Time(ms): " + String(now) + "\n" +
      "Temp DS18B20 (C): " + String(tempDS, 2) + "\n" +
      "Temp BMP280  (C): " + String(tempBMP, 2) + "\n" +
      "Pressure (hPa): " + String(pressure, 2) + "\n" +
      "Light (ADC): " + String(light) + "\n" +
      "-----------------------------";

    // Вивести в Serial
    Serial.println(line);

    // Запис у файл
    writeToFile(line);
  }

  // Через 90 секунд — прочитаємо файл
  if (millis() > 90000) {
    readFromFile();
    while (1);
  }
}