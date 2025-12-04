#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Адреса BMP280 (залежить від SDO):
// 0x76 (SDO -> GND) або 0x77 (SDO -> 3.3V)
#define BMP280_ADDRESS 0x76  

Adafruit_BMP280 bmp; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println("Не знайдено BMP280! Перевірте підключення.");
    while (1);
  }

  // Налаштування датчика
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,     // нормальний режим
    Adafruit_BMP280::SAMPLING_X2,     // oversampling температури
    Adafruit_BMP280::SAMPLING_X16,    // oversampling тиску
    Adafruit_BMP280::FILTER_X16,      // фільтрація
    Adafruit_BMP280::STANDBY_MS_500   // час очікування
  );

  Serial.println("BMP280 успішно ініціалізовано!");
}

void loop() {
  float temperature = bmp.readTemperature();   // °C
  float pressure = bmp.readPressure() / 100.0; // Гектопаскалі (hPa)

  Serial.print("Температура: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Тиск: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.println("----------------------");
  delay(2000); // зчитування кожні 2 секунди
}