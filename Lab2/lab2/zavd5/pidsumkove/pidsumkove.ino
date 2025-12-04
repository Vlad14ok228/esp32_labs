/*
 * Підсумкове завдання: Метеостанція на ESP32
 * Сенсори: DS18B20 (1-Wire), BMP280 (I2C), Фоторезистор (ADC)
 */

#include <Wire.h>               // Для протоколу I2C (BMP280)
#include <OneWire.h>            // Для протоколу 1-Wire (DS18B20)
#include <DallasTemperature.h>  // Для роботи з DS18B20
#include <Adafruit_BMP280.h>    // Для роботи з BMP280

// --- НАЛАШТУВАННЯ ПІНІВ ---
#define ONE_WIRE_BUS 4          // Пін даних для DS18B20
#define PHOTO_PIN 34            // Аналоговий пін для фоторезистора

// --- СТВОРЕННЯ ОБ'ЄКТІВ ---

// 1. Налаштування DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// 2. Налаштування BMP280 (через I2C)
Adafruit_BMP280 bmp; 

void setup() {
  // Ініціалізація серійного порту для виводу даних
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- Старт метеостанції ---");

  // --- Ініціалізація DS18B20 ---
  sensors.begin();
  if (sensors.getDeviceCount() == 0) {
    Serial.println("УВАГА: DS18B20 не знайдено! Перевірте підключення.");
  } else {
    Serial.println("DS18B20 готовий.");
  }

  // --- Ініціалізація BMP280 ---
  // Спроба підключитися за адресою 0x76 (стандарт для дешевих модулів)
  if (!bmp.begin(0x76)) {  
    Serial.println("УВАГА: BMP280 не знайдено! Перевірте адресу (0x76/0x77) або дроти.");
    // Ми не зупиняємо програму (while(1)), щоб інші сенсори могли працювати
  } else {
    Serial.println("BMP280 готовий.");
  }

  // --- Налаштування Фоторезистора ---
  // GPIO 34 працює тільки на вхід, тому pinMode можна не писати, але для ясності:
  pinMode(PHOTO_PIN, INPUT);
  
  Serial.println("Всі системи ініціалізовані. Починаємо вимірювання...");
  Serial.println("------------------------------------------------");
}

void loop() {
  Serial.println("\n--- Нові виміри ---");

  // ==========================================
  // 1. РОБОТА З ТЕМПЕРАТУРОЮ (DS18B20)
  // ==========================================
  // Відправляємо команду всім датчикам на лінії 1-Wire зробити замір
  sensors.requestTemperatures(); 
  float tempDS = sensors.getTempCByIndex(0); // Зчитуємо результат

  if (tempDS != DEVICE_DISCONNECTED_C) {
    Serial.print("DS18B20 Температура: ");
    Serial.print(tempDS);
    Serial.println(" °C");
  } else {
    Serial.println("DS18B20: Помилка зчитування");
  }

  // ==========================================
  // 2. РОБОТА З ТИСКОМ (BMP280)
  // ==========================================
  // Зчитуємо температуру з BMP (він теж вміє міряти)
  float tempBMP = bmp.readTemperature();
  // Зчитуємо тиск і ділимо на 100, щоб отримати гектопаскалі (hPa)
  float pressure = bmp.readPressure() / 100.0F; 

  Serial.print("BMP280  Температура: ");
  Serial.print(tempBMP);
  Serial.println(" °C");
  
  Serial.print("BMP280  Тиск:        ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // ==========================================
  // 3. РОБОТА ЗІ СВІТЛОМ (Фоторезистор)
  // ==========================================
  // Зчитуємо аналогове значення (0 - 4095 для ESP32)
  int lightRaw = analogRead(PHOTO_PIN);
  
  // Конвертуємо у відсотки (приблизно) для зручності
  // map(значення, мінімум_вхід, максимум_вхід, мінімум_вихід, максимум_вихід)
  int lightPercent = map(lightRaw, 0, 4095, 0, 100);

  Serial.print("Світло (Raw):        ");
  Serial.print(lightRaw);
  Serial.print(" (прибл. ");
  Serial.print(lightPercent);
  Serial.println("%)");

  // ==========================================
  
  Serial.println("------------------------------------------------");
  delay(2000); // Затримка 2 секунди перед наступним циклом
}