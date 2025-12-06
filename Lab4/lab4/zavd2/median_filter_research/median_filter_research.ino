/*
 * Завдання 2: Медіанний фільтр для датчика DS18B20
 */

#include <OneWire.h>
#include <DallasTemperature.h>

// --- Налаштування ---
#define ONE_WIRE_BUS 4        // Пін, до якого підключено DATA (DQ)
#define NUM_READINGS 100      // Кількість значень для зчитування
#define READ_INTERVAL 1000    // Інтервал зчитування (1000 мс = 1 сек)
#define MEDIAN_WINDOW_SIZE 5  // Розмір вікна фільтра (обов'язково непарне число: 3, 5, 7...)

// Ініціалізація 1-Wire
OneWire oneWire(ONE_WIRE_BUS);
// Передача посилання на 1-Wire в Dallas Temperature
DallasTemperature sensors(&oneWire);

// Масиви для зберігання даних
float rawTemps[NUM_READINGS];
float filteredTemps[NUM_READINGS];

// -----------------------------------------------------------------
// ДОПОМІЖНА ФУНКЦІЯ: Знайти медіану в малому масиві (вікні)
// -----------------------------------------------------------------
float findMedian(float window[], int windowSize) {
  // Ми сортуємо тимчасову копію вікна,
  // щоб знайти середній елемент.
  float tempWindow[windowSize];
  for(int i=0; i < windowSize; i++) {
    tempWindow[i] = window[i];
  }

  // Простий сортувальний алгоритм (сортування бульбашкою)
  // Для 5 елементів це працює миттєво.
  for (int i = 0; i < windowSize - 1; i++) {
    for (int j = 0; j < windowSize - i - 1; j++) {
      if (tempWindow[j] > tempWindow[j + 1]) {
        // обмін
        float temp = tempWindow[j];
        tempWindow[j] = tempWindow[j + 1];
        tempWindow[j + 1] = temp;
      }
    }
  }
  
  // Медіана - це середній елемент у відсортованому масиві
  return tempWindow[windowSize / 2];
}


// -----------------------------------------------------------------
// SETUP: Виконується один раз
// -----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- Тест медіанного фільтра з DS18B20 ---");

  // Запуск бібліотеки
  sensors.begin();
  if (sensors.getDeviceCount() == 0) {
    Serial.println("ПОМИЛКА: Датчик DS18B20 не знайдено!");
    while(true) delay(100); // Зупинка
  }
  Serial.println("Датчик знайдено.");
  
  // --- 1. ЕТАП: Зчитування 100 значень ---
  Serial.println("Зчитую 100 значень...");
  for (int i = 0; i < NUM_READINGS; i++) {
    sensors.requestTemperatures(); // Відправляємо запит на вимір
    
    float temp = sensors.getTempCByIndex(0);
    // Перевірка на помилку зчитування
    if (temp == DEVICE_DISCONNECTED_C) {
      Serial.println("Помилка зчитування!");
      rawTemps[i] = 0; // Записуємо 0 у разі помилки
    } else {
      rawTemps[i] = temp; // Отримуємо температуру
    }
    
    Serial.print("Зчитування ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(rawTemps[i]);
    
    delay(READ_INTERVAL);
  }
  Serial.println("Зчитування завершено.");

  // --- 2. ЕТАП: Реалізація медіанного фільтру ---
  Serial.println("Застосовую медіанний фільтр (розмір вікна = 5)...");
  
  float window[MEDIAN_WINDOW_SIZE]; // Масив для "ковзного вікна"
  int halfWindow = MEDIAN_WINDOW_SIZE / 2; // = 2 (для вікна 5)

  for (int i = 0; i < NUM_READINGS; i++) {
    // Крайові випадки: якщо ми на самому початку або в самому кінці
    // масиву, ми не можемо побудувати повне вікно.
    // Тому ми просто копіюємо "сире" значення.
    if (i < halfWindow || i >= NUM_READINGS - halfWindow) {
      filteredTemps[i] = rawTemps[i];
    } 
    else {
      // Формуємо "вікно" з 5 значень
      // Наприклад, для i=10 (вікно 5), ми беремо індекси: 8, 9, 10, 11, 12
      int k = 0;
      for (int j = i - halfWindow; j <= i + halfWindow; j++) {
        window[k] = rawTemps[j];
        k++;
      }
      
      // Знаходимо медіану цього вікна
      filteredTemps[i] = findMedian(window, MEDIAN_WINDOW_SIZE);
    }
  }
  Serial.println("Фільтрація завершена.");

  // --- 3. ЕТАП: Виведення результатів ---
  Serial.println("\n--- РЕЗУЛЬТАТИ ---");
  Serial.println("Індекс, \tСирі дані (Raw), \tВідфільтровано (Filtered)");
  Serial.println("-----------------------------------------------------");
  
  for (int i = 0; i < NUM_READINGS; i++) {
    Serial.print(i);
    Serial.print(", \t\t");
    Serial.print(rawTemps[i]);
    Serial.print(", \t\t");
    Serial.println(filteredTemps[i]);
  }
}

// -----------------------------------------------------------------
// LOOP: Порожній, оскільки все завдання виконується один раз
// -----------------------------------------------------------------
void loop() {
  // Нічого не робимо
}