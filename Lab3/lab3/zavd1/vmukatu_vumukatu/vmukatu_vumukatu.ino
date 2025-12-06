// Піни, які ми використовуємо
const int ledPin = 21;
const int buttonPin = 18;

// Змінна для збереження поточного стану світлодіода (false = OFF, true = ON)
bool ledIsOn = false;

// Змінна для відстеження попереднього стану "кнопки"
int lastButtonState = HIGH; 

void setup() {
  // Запускаємо монітор порту
  Serial.begin(115200); 

  // Налаштовуємо піни
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Переконуємося, що світлодіод вимкнений на старті
  digitalWrite(ledPin, LOW);
  
  Serial.println("Система готова. Дотик вмикає/вимикає світлодіод.");
}

void loop() {
  // 1. Зчитуємо поточний стан конекторів
  int currentButtonState = digitalRead(buttonPin);

  // 2. Перевіряємо, чи відбувся момент дотику (перехід з HIGH на LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    
    // 3. Інвертуємо (змінюємо на протилежний) стан світлодіода
    ledIsOn = !ledIsOn; // Якщо було false -> стане true, якщо було true -> стане false

    // 4. Діємо відповідно до нового стану
    if (ledIsOn == true) {
      // Якщо світлодіод тепер має бути УВІМКНЕНИМ
      digitalWrite(ledPin, HIGH);
      Serial.println("ON");
    } else {
      // Якщо світлодіод тепер має бути ВИМКНЕНИМ
      digitalWrite(ledPin, LOW);
      Serial.println("OFF");
    }
  }

  // 5. Оновлюємо попередній стан для наступної перевірки
  lastButtonState = currentButtonState;
  
  // Невелика затримка для стабільності
  delay(10); 
}