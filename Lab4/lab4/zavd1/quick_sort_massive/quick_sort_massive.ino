/*
 * Код для Завдання 1:
 * Дослідження "Швидкого сортування" (qsort) на ESP32
 * для масивів різного розміру та типу.
 */

#include <stdlib.h> // Потрібно для функції qsort()
#include <time.h>   // Потрібно для функції srand() (ініціалізація випадкових чисел)

// ==========================================================
// 1. КОМПАРАТОРИ (Функції порівняння) для qsort()
// qsort() вимагає, щоб ми самі сказали, як порівнювати елементи.
// ==========================================================

// Компаратор для INT
int compareInt(const void *a, const void *b) {
  return (*(int*)a - *(int*)b);
}

// Компаратор для CHAR
int compareChar(const void *a, const void *b) {
  return (*(char*)a - *(char*)b);
}

// Компаратор для FLOAT
int compareFloat(const void *a, const void *b) {
  float fa = *(const float*)a;
  float fb = *(const float*)b;
  if (fa < fb) return -1;
  if (fa > fb) return 1;
  return 0;
}

// Компаратор для DOUBLE
int compareDouble(const void *a, const void *b) {
  double da = *(const double*)a;
  double db = *(const double*)b;
  if (da < db) return -1;
  if (da > db) return 1;
  return 0;
}

// ==========================================================
// 2. ФУНКЦІЇ ДЛЯ ЗАПОВНЕННЯ МАСИВІВ
// ==========================================================
void fillIntArray(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    arr[i] = rand() % 10000; // Випадкові числа від 0 до 9999
  }
}

void fillFloatArray(float arr[], int size) {
  for (int i = 0; i < size; i++) {
    arr[i] = (float)rand() / (float)(RAND_MAX / 1000.0); // Випадкові числа від 0.0 до 1000.0
  }
}

void fillDoubleArray(double arr[], int size) {
  for (int i = 0; i < size; i++) {
    arr[i] = (double)rand() / (double)(RAND_MAX / 1000.0); // Випадкові числа від 0.0 до 1000.0
  }
}

void fillCharArray(char arr[], int size) {
  for (int i = 0; i < size; i++) {
    arr[i] = (rand() % 94) + 33; // Друковані ASCII-символи (від '!' до '~')
  }
}

// ==========================================================
// 3. SETUP (Основна логіка)
// ==========================================================
void setup() {
  // Запускаємо Монітор порту на швидкості 115200
  Serial.begin(115200);
  delay(2000); // Даємо час Монітору порту підключитися

  Serial.println("================================");
  Serial.println("ПОЧАТОК ТЕСТУ ШВИДКОГО СОРТУВАННЯ (qsort)");
  Serial.print("Частота CPU: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");
  Serial.println("================================");

  // Ініціалізуємо генератор випадкових чисел
  srand(millis()); 

  int sizes[] = {50, 100, 500, 1000};
  
  // Проходимо по всім заданим розмірам
  for (int size : sizes) {
    Serial.print("--- Тестування: Розмір Масиву = ");
    Serial.print(size);
    Serial.println(" ---");
    
    unsigned long startTime, endTime;

    // --- 1. Тест INT ---
    // Використовуємо malloc() (динамічне виділення пам'яті),
    // щоб гарантовано вистачило місця для великих масивів
    int *intArray = (int*) malloc(size * sizeof(int));
    if (intArray == NULL) { Serial.println("Помилка виділення пам'яті для INT!"); continue; }
    
    fillIntArray(intArray, size); // Заповнюємо
    startTime = micros(); // Початок виміру часу
    qsort(intArray, size, sizeof(int), compareInt);
    endTime = micros(); // Кінець виміру часу
    Serial.print("  [INT] \tЧас сортування: ");
    Serial.print(endTime - startTime);
    Serial.println(" мкс");
    free(intArray); // Очищуємо пам'ять

    
    // --- 2. Тест FLOAT ---
    float *floatArray = (float*) malloc(size * sizeof(float));
    if (floatArray == NULL) { Serial.println("Помилка виділення пам'яті для FLOAT!"); continue; }
    
    fillFloatArray(floatArray, size);
    startTime = micros();
    qsort(floatArray, size, sizeof(float), compareFloat);
    endTime = micros();
    Serial.print("  [FLOAT] \tЧас сортування: ");
    Serial.print(endTime - startTime);
    Serial.println(" мкс");
    free(floatArray);

    
    // --- 3. Тест DOUBLE ---
    double *doubleArray = (double*) malloc(size * sizeof(double));
    if (doubleArray == NULL) { Serial.println("Помилка виділення пам'яті для DOUBLE!"); continue; }
    
    fillDoubleArray(doubleArray, size);
    startTime = micros();
    qsort(doubleArray, size, sizeof(double), compareDouble);
    endTime = micros();
    Serial.print("  [DOUBLE] \tЧас сортування: ");
    Serial.print(endTime - startTime);
    Serial.println(" мкс");
    free(doubleArray);

    
    // --- 4. Тест CHAR ---
    char *charArray = (char*) malloc(size * sizeof(char));
    if (charArray == NULL) { Serial.println("Помилка виділення пам'яті для CHAR!"); continue; }
    
    fillCharArray(charArray, size);
    startTime = micros();
    qsort(charArray, size, sizeof(char), compareChar);
    endTime = micros();
    Serial.print("  [CHAR] \tЧас сортування: ");
    Serial.print(endTime - startTime);
    Serial.println(" мкс");
    free(charArray);

    Serial.println("---------------------------------");
  }
  
  Serial.println("ТЕСТ ЗАВЕРШЕНО.");
}

// ==========================================================
// 4. LOOP (порожній)
// ==========================================================
void loop() {
  // Нічого не робимо, оскільки тест виконується один раз у setup()
}