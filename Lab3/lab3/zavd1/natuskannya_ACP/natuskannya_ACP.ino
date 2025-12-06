// --- Піни компонентів ---
const int ledPin = 25;      // <--- ВАЖЛИВА ЗМІНА! Це пін ЦАП (DAC1)
const int buttonPin = 18;   // Пін для конекторів-"кнопки"

// --- Логіка лічильника яскравості ---
int pressCount = 0;         // Поточний рівень яскравості (0 = OFF)
int numLevels = 6;          // Загальна кількість рівнів (0, 1, 2, 3, 4, 5)

// Масив, що зберігає рівні яскравості (0 = вимкнено, 255 = макс.)
// ЦАП (DAC) 8-бітний, тому значення від 0 до 255
int brightnessLevels[] = {0, 50, 100, 150, 200, 255};

// Змінна для відстеження стану кнопки
int lastButtonState = HIGH; 

void setup() {
  Serial.begin(115200);

  // Налаштовуємо пін кнопки
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Для dacWrite() не потрібен pinMode(ledPin, OUTPUT)
  
  Serial.println("Система готова. Збільшуйте яскравість дотиком.");
  
  // Починаємо з вимкненого світлодіода (0)
  dacWrite(ledPin, 0);
}

void loop() {
  // Зчитуємо поточний стан конекторів
  int currentButtonState = digitalRead(buttonPin);

  // "Ловимо" тільки МОМЕНТ дотику (перехід з HIGH на LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    
    // Збільшуємо лічильник і "зациклюємо" його
    // 0->1, 1->2, 2->3, 3->4, 4->5, 5->0
    pressCount = (pressCount + 1) % numLevels; 
    
    // Отримуємо нове значення яскравості з нашого масиву
    int currentBrightness = brightnessLevels[pressCount];
    
    // Встановлюємо нову яскравість через ЦАП (DAC)
    dacWrite(ledPin, currentBrightness);
    
    // Виводимо інформацію в термінал
    Serial.print("Рівень: ");
    Serial.print(pressCount);
    Serial.print(", Яскравість ЦАП (0-255): ");
    Serial.println(currentBrightness);
  }

  // Оновлюємо попередній стан
  lastButtonState = currentButtonState;
  
  delay(10); // Невелика затримка для стабільності
}