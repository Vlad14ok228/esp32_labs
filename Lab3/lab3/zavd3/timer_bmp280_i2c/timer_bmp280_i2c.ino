#include <Wire.h>
#include <Adafruit_BMP280.h>

// --- Піни ---
#define LED_PIN 23        // Пін для LED (індикатор)
#define BUTTON_PIN 18     // Пін для конекторів
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// --- Об'єкти та глобальні змінні ---
Adafruit_BMP280 bmp;

volatile bool interruptFired = false; 
volatile bool isMeasuring = false;    // Змінна стану (ON/OFF)

unsigned long lastReadTime = 0;
const long readInterval = 2000; // 2000 мс = 2 секунди

volatile unsigned long lastInterruptTime = 0;
unsigned long debounceDelay = 200; // 200 мс

// ----------------------------------------------------
// ФУНКЦІЯ ПЕРЕРИВАННЯ (ISR)
// Спрацьовує при дотику (FALLING)
// ----------------------------------------------------
void IRAM_ATTR handleInterrupt() {
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > debounceDelay) {
    interruptFired = true; // Піднімаємо прапорець для loop()!
    lastInterruptTime = currentTime;
  }
}

// ----------------------------------------------------
// SETUP
// ----------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  // Починаємо з увімкненого LED (бо вимірювання ще не йде)
  digitalWrite(LED_PIN, HIGH); 
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  if (!bmp.begin(0x76)) {
    Serial.println("Не вдалося знайти BMP280!");
    while (1) delay(10);
  }
  Serial.println("BMP280 знайдено!");

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);

  Serial.println("Система готова. LED горить. Доторкніться, щоб почати вимір.");
}

// ----------------------------------------------------
// LOOP
// ----------------------------------------------------
void loop() {
  
  // 1. Перевіряємо, чи спрацювало переривання
  if (interruptFired) {
    interruptFired = false; 
    isMeasuring = !isMeasuring; // Перемикаємо стан

    if (isMeasuring) {
      Serial.println("--- СТАРТ: Вимірювання почалося (LED вимкнено) ---");
      lastReadTime = millis(); 
    } else {
      Serial.println("--- СТОП: Вимірювання зупинено (LED увімкнено) ---");
    }
  }

  // 2. Основна логіка, що залежить від стану
  if (isMeasuring) {
    // --- РЕЖИM УВІМКНЕНО ---
    
    // Світлодіод ВИМКНЕНИЙ, поки йде вимір
    digitalWrite(LED_PIN, LOW); // <--- ЗМІНА ТУТ

    // Зчитуємо дані кожні 2 секунди
    if (millis() - lastReadTime >= readInterval) {
      lastReadTime = millis(); 
      float temp = bmp.readTemperature();
      Serial.print("ВИМІР: Температура = ");
      Serial.print(temp);
      Serial.println(" *C");
    }
    
  } else {
    // --- РЕЖИМ ВИМКНЕНО ---
    
    // Світлодіод УВІМКНЕНИЙ, коли система чекає
    digitalWrite(LED_PIN, HIGH); // <--- ЗМІНА ТУТ
  }
}