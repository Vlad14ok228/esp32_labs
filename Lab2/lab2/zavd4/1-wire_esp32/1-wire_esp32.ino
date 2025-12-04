#include <OneWire.h>          // Бібліотека для реалізації протоколу 1-Wire
#include <DallasTemperature.h> // Бібліотека для роботи з сімейством датчиків Dallas (DS18B20)

// --- ВИЗНАЧЕННЯ ПІНІВ ТА КОНСТАНТ ---
#define ONE_WIRE_BUS 4       // Пін, до якого підключена лінія даних (DQ) 1-Wire (GPIO 4)
#define TEMPERATURE_PRECISION 9 // Встановлюємо точність вимірювання (9 біт = 0.5 °C)

// Ініціалізація об'єкта OneWire, передаємо номер піна
OneWire oneWire(ONE_WIRE_BUS);

// Передача посилання на 1-Wire об'єкт в Dallas Temperature
DallasTemperature sensors(&oneWire);

// --- Глобальна змінна для зберігання адреси датчика ---
// Цей масив буде зберігати унікальну 64-бітну адресу першого знайденого датчика
DeviceAddress tempDeviceAddress; 

void setup() {
  Serial.begin(115200);
  Serial.println("--- Тест протоколу 1-Wire з одним DS18B20 ---");

  // Запуск бібліотеки DallasTemperature
  sensors.begin(); 
  
  // Перевірка кількості знайдених датчиків
  int deviceCount = sensors.getDeviceCount();
  Serial.print("Знайдено пристроїв: ");
  Serial.println(deviceCount);

  // Отримання адреси першого знайденого датчика
  if (deviceCount > 0) {
    sensors.getAddress(tempDeviceAddress, 0); // Зберігаємо адресу пристрою з індексом 0
    Serial.println("Адреса першого сенсора збережена.");
    // Встановлення бажаної точності (необов'язково)
    sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
  } else {
    Serial.println("ПОМИЛКА: Жоден датчик не знайдений. Перевірте резистор 4.7 кОм!");
    while(1); // Зупинка програми
  }
}

void loop() {
  // --- 1. Відправлення команди на вимірювання ---
  // Ця команда надсилається ВСІМ пристроям на шині
  sensors.requestTemperatures(); 

  // --- 2. Отримання результату ---
  // Отримуємо температуру, використовуючи збережену АДРЕСУ датчика (найкращий метод)
  float tempC = sensors.getTempC(tempDeviceAddress); 

  // --- 3. Виведення ---
  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Температура: ");
    Serial.print(tempC);
    Serial.println(" °C");
  } else {
    Serial.println("Помилка зчитування температури.");
  }

  delay(2000); // Інтервал зчитування 2 секунди
}