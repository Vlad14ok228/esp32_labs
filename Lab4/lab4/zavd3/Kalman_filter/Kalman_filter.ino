/*
 * Завдання 3: Фільтр Кальмана для датчика DS18B20
 * * Логіка заснована на 1D фільтрі Кальмана:
 * 1. Прогноз:
 * P_pred = P + Q
 * 2. Оновлення:
 * K = P_pred / (P_pred + R)
 * x = x + K * (вимір - x)
 * P = (1 - K) * P_pred
 */

#include <OneWire.h>
#include <DallasTemperature.h>

// --- Налаштування датчика ---
#define ONE_WIRE_BUS 4        // Пін, до якого підключено DATA (DQ)
#define NUM_READINGS 100      // Кількість значень для зчитування
#define READ_INTERVAL 1000    // Інтервал зчитування (1000 мс = 1 сек)

// Ініціалізація 1-Wire
OneWire oneWire(ONE_WIRE_BUS);
// Передача посилання на 1-Wire в Dallas Temperature
DallasTemperature sensors(&oneWire);

// Масиви для зберігання даних
float rawTemps[NUM_READINGS];
float filteredTemps[NUM_READINGS];

// -----------------------------------------------------------------
// Клас для 1D Фільтра Кальмана
// -----------------------------------------------------------------
class SimpleKalmanFilter {
private:
  double R; // Похибка вимірювання (Measurement Noise)
  double Q; // Похибка процесу (Process Noise)
  double P; // Похибка оцінки (Estimate Error)
  double x; // Оцінене (відфільтроване) значення

public:
  // Конструктор
  SimpleKalmanFilter(double r, double q, double p_initial, double x_initial) {
    R = r; // Наскільки ми "не довіряємо" датчику
    Q = q; // Наскільки швидко, на нашу думку, змінюється справжнє значення
    P = p_initial; // Початкова похибка оцінки
    x = x_initial; // Початкове значення
  }

  // Функція оновлення фільтра новим значенням
  double updateEstimate(double z) { // z - це нове вимірювання
    // 1. Етап Прогнозу
    // Ми не маємо моделі руху, тому x_pred = x
    double P_pred = P + Q;

    // 2. Етап Оновлення (Корекції)
    double K = P_pred / (P_pred + R); // Розрахунок коефіцієнта Кальмана
    x = x + K * (z - x);             // Нова оцінка значення
    P = (1.0 - K) * P_pred;          // Оновлення похибки оцінки

    return x;
  }
};

// -----------------------------------------------------------------
// Ініціалізація фільтра Кальмана
// -----------------------------------------------------------------
// Ми будемо ініціалізувати його в setup(), 
// коли отримаємо перше реальне значення.
// R = 0.125 (DS18B20 має дискретність 0.125C, це наш "шум")
// Q = 0.01  (Температура в кімнаті змінюється ДУЖЕ повільно)
// P = 1.0   (Починаємо з високої невпевненості)
SimpleKalmanFilter kalman(0.125, 0.01, 1.0, 0.0); // 0.0 - тимчасове значення

// -----------------------------------------------------------------
// SETUP: Виконується один раз
// -----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- Тест фільтра Кальмана з DS18B20 ---");

  // Запуск бібліотеки
  sensors.begin();
  if (sensors.getDeviceCount() == 0) {
    Serial.println("ПОМИЛКА: Датчик DS18B20 не знайдено!");
    while(true) delay(100); // Зупинка
  }
  Serial.println("Датчик знайдено.");

  // --- 1. ЕТАП: Зчитування 100 значень + Фільтрація ---
  Serial.println("Зчитую 100 значень...");

  // --- Зчитування ПЕРШОГО значення (для "запуску" фільтра) ---
  sensors.requestTemperatures();
  float firstTemp = sensors.getTempCByIndex(0);
  if (firstTemp == DEVICE_DISCONNECTED_C) {
    Serial.println("ПОМИЛКА при першому зчитуванні!");
    while(1);
  }
  
  // Тепер ініціалізуємо фільтр реальним першим значенням
  kalman = SimpleKalmanFilter(0.125, 0.01, 1.0, firstTemp);
  
  rawTemps[0] = firstTemp;
  filteredTemps[0] = firstTemp;
  Serial.print("Зчитування 1: Raw=");
  Serial.println(firstTemp);
  delay(READ_INTERVAL);

  // --- Зчитування решти 99 значень ---
  for (int i = 1; i < NUM_READINGS; i++) {
    sensors.requestTemperatures(); // Запит
    float rawValue = sensors.getTempCByIndex(0); // Отримання
    
    // Обробка можливої помилки
    if (rawValue == DEVICE_DISCONNECTED_C) {
       Serial.println("Помилка зчитування!");
       rawTemps[i] = rawTemps[i-1]; // Використовуємо останнє вдале значення
    } else {
       rawTemps[i] = rawValue;
    }

    // Застосовуємо фільтр
    Кальмана
    filteredTemps[i] = kalman.updateEstimate(rawTemps[i]);

    Serial.print("Зчитування "); Serial.print(i + 1);
    Serial.print(": Raw="); Serial.print(rawTemps[i]);
    Serial.print(", \tKalman="); Serial.println(filteredTemps[i]);
    
    delay(READ_INTERVAL);
  }
  Serial.println("Зчитування завершено.");

  // --- 2. ЕТАП: Виведення результатів ---
  Serial.println("\n--- РЕЗУЛЬТАТИ ---");
  Serial.println("Індекс, \tСирі дані (Raw), \tФільтр Кальмана (Kalman)");
  Serial.println("---------------------------------------------------------");
  
  for (int i = 0; i < NUM_READINGS; i++) {
    Serial.print(i);
    Serial.print(", \t\t");
    Serial.print(rawTemps[i]);
    Serial.print(", \t\t");
    Serial.println(filteredTemps[i]);
  }
}

// -----------------------------------------------------------------
// LOOP: Порожній
// -----------------------------------------------------------------
void loop() {
  // Нічого не робимо
}