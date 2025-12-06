/*
 * Код: Запустити 3-секундний зворотний відлік
 * за допомогою GPIO-переривання та апаратного таймера.
 */

// --- Піни ---
#define LED_PIN 23
#define BUTTON_PIN 18

// --- Глобальні змінні ---
hw_timer_t *timer = NULL; // Вказівник на наш таймер

// 'volatile' - обов'язково для змінних,
// які використовуються всередині переривань (ISR)

// Прапорець, який піднімає ISR кнопки
volatile bool startCountdown = false; 
// Прапорець, який піднімає ISR таймера
volatile bool timerTick = false;      

// Змінна для відліку
volatile int countdown = 3;
// Змінна стану
volatile bool isCountingDown = false;

// "Антибрязкіт" для переривання
volatile unsigned long lastInterruptTime = 0;
unsigned long debounceDelay = 200; // 200 мс

// ----------------------------------------------------
// Переривання від ТАЙМЕРА (спрацьовує кожну 1 сек)
// ----------------------------------------------------
void IRAM_ATTR onTimer() {
  timerTick = true; // Просто піднімаємо прапорець
}

// ----------------------------------------------------
// Переривання від КОНЕКТОРІВ (дотик, FALLING)
// ----------------------------------------------------
void IRAM_ATTR handleButtonInterrupt() {
  unsigned long currentTime = millis();
  // Антибрязкіт + перевірка, чи не йде вже відлік
  if (currentTime - lastInterruptTime > debounceDelay && !isCountingDown) {
    startCountdown = true; // Піднімаємо прапорець для loop()
    lastInterruptTime = currentTime;
  }
}

// ----------------------------------------------------
// SETUP
// ----------------------------------------------------
void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED вимкнений
  
  // Налаштовуємо пін кнопки з PULLUP (на ньому 3.3V)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // --- Налаштування GPIO Переривання ---
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);

  // --- Налаштування Таймера (але поки не запускаємо) ---
  // 1 тік = 1 мікросекунда
  timer = timerBegin(1000000); 
  timerAttachInterrupt(timer, &onTimer);
  // 1,000,000 мкс = 1 секунда, 'true' = авто-перезапуск
  timerAlarm(timer, 1000000, true, 0); 
  // ВАЖЛИВО: Таймер ще не запущений, він чекає команди

  Serial.println("Система готова.");
  Serial.println("Доторкніться конекторами (GPIO 18), щоб почати відлік...");
}

// ----------------------------------------------------
// LOOP
// ----------------------------------------------------
void loop() {
  
  // 1. Чи спрацювало переривання кнопки?
  if (startCountdown) {
    startCountdown = false;  // Опускаємо прапорець
    isCountingDown = true;   // Вмикаємо режим відліку
    countdown = 3;           // Скидаємо лічильник
    
    Serial.println("СТАРТ!");
    Serial.println(countdown);
    digitalWrite(LED_PIN, HIGH); // Вмикаємо LED
    
    timerStart(timer);       // <--- ЗАПУСКАЄМО ТАЙМЕР!
  }

  // 2. Чи спрацював "тік" таймера?
  if (timerTick && isCountingDown) {
    timerTick = false; // Опускаємо прапорець
    countdown--;       // Зменшуємо лічильник

    if (countdown > 0) {
      Serial.println(countdown);
    } else {
      Serial.println("ПУСК!");
      digitalWrite(LED_PIN, LOW); // Вимикаємо LED
      isCountingDown = false;     // Зупиняємо відлік
      timerStop(timer);         // <--- ЗУПИНЯЄМО ТАЙМЕР!
    }
  }
  
  // Головний цикл вільний
}