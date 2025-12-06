/*
 * Код для Завдання 2:
 * Зчитування даних з фоторезистора за допомогою ДВОХ апаратних таймерів
 * з різними інтервалами.
 * Адаптовано для ESP32 Core v3.x.x.
 */

// Пін, до якого підключений фоторезистор (АЦП)
#define PHOTORESISTOR_PIN 34

// Вказівники на апаратні таймери
hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;

// "Прапорці", які будуть встановлюватися перериваннями таймерів
volatile bool flag_ReadTimer1 = false;
volatile bool flag_ReadTimer2 = false;

// ----------------------------------------------------
// Функція переривання для ТАЙМЕРА 1
void IRAM_ATTR onTimer1() {
  flag_ReadTimer1 = true;
}

// ----------------------------------------------------
// Функція переривання для ТАЙМЕРА 2
void IRAM_ATTR onTimer2() {
  flag_ReadTimer2 = true;
}
// ----------------------------------------------------

void setup() {
  // ВАЖЛИВО: Переконайтеся, що в моніторі порту обрано 115200
  Serial.begin(115200);
  pinMode(PHOTORESISTOR_PIN, INPUT);

  Serial.println("Налаштування двох апаратних таймерів...");

  // --- Налаштування Таймера 1 (спрацьовує кожну 1 секунду) ---
  
  // 1. Ініціалізуємо таймер з частотою 1 МГц (1,000,000 тіків в секунду)
  // 1 тік = 1 мікросекунда
  timer1 = timerBegin(1000000); 
  
  // 2. Прив'язуємо функцію-переривання
  timerAttachInterrupt(timer1, &onTimer1); 
  
  // 3. Встановлюємо спрацювання (1,000,000 тіків = 1 секунда)
  timerAlarm(timer1, 1000000, true, 0); 

  // --- Налаштування Таймера 2 (спрацьовує кожні 3 секунди) ---
  
  // Використовуємо наступний апаратний таймер
  timer2 = timerBegin(1000000); // Та сама частота 1 МГц
  
  // Прив'язуємо переривання
  timerAttachInterrupt(timer2, &onTimer2); 
  
  // 3,000,000 тіків = 3 секунди
  timerAlarm(timer2, 3000000, true, 0); 
}

void loop() {
  
  // --- Обробник для Таймера 1 ---
  if (flag_ReadTimer1) {
    flag_ReadTimer1 = false; // Опускаємо прапорець
    
    int value = analogRead(PHOTORESISTOR_PIN);
    Serial.print("Таймер 1 (1 сек): Значення фоторезистора = ");
    Serial.println(value);
  }

  // --- Обробник для Таймера 2 ---
  if (flag_ReadTimer2) {
    flag_ReadTimer2 = false; // Опускаємо прапорець
    
    int value = analogRead(PHOTORESISTOR_PIN);
    Serial.print("Таймер 2 (3 сек): Значення фоторезистора = ");
    Serial.println(value);
  }

  // Головний цикл вільний
}