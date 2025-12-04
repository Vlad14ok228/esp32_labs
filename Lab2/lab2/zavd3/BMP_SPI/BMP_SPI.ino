#include <SPI.h>              // Обов'язкова бібліотека для протоколу SPI
#include <Adafruit_Sensor.h>  // Базова бібліотека для сенсорів Adafruit
#include <Adafruit_BMP280.h>  // Бібліотека для датчика BMP280

// --- ВИЗНАЧЕННЯ ПІНІВ ---
// Визначаємо пін Chip Select (CS) - потрібен для вибору датчика в SPI мережі
#define BMP_CS 5

// Варіант 1: Апаратний SPI (використовує стандартні піни ESP32: GPIO 18, 19, 23)
Adafruit_BMP280 bmp(BMP_CS); // Ініціалізація об'єкта BMP280, передаємо лише CS-пін

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Тест BMP280 через SPI");

  // --- Ініціалізація датчика ---
  // Для SPI не потрібна адреса (0x76), оскільки CS-пін вибирає датчик
  if (!bmp.begin()) {
    Serial.println("Помилка: Не знайдено BMP280!");
    // while(1); // Коментуємо, щоб уникнути зависання
  }

  // --- Налаштування датчика (Oversampling, Фільтрація, Режими) ---
  // Ці налаштування зменшують шум (завдання: фільтрація)
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,     // Режим роботи: нормальний
    Adafruit_BMP280::SAMPLING_X2,     // Oversampling температури (зменшує шум)
    Adafruit_BMP280::SAMPLING_X16,    // Oversampling тиску (збільшує точність)
    Adafruit_BMP280::FILTER_X16,      // Вмикаємо внутрішній фільтр IIR (Filter=16)
    Adafruit_BMP280::STANDBY_MS_500   // Час очікування між замірами
  );

  Serial.println("BMP280 успішно ініціалізовано по SPI!");
}

void loop() {
  // --- Зчитування даних ---
  float temperature = bmp.readTemperature(); // Зчитує температуру в °C
  float pressure = bmp.readPressure() / 100.0; // Зчитує тиск і переводить у гПа (hPa)

  // --- Виведення в термінал ---
  Serial.print("Температура: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Тиск: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.println("----------------------");
  delay(2000); // Чекаємо 2 секунди між замірами
}