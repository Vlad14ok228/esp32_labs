// Піни, які ми використовуємо
const int ledPin = 21;
const int buttonPin = 18;

// Змінна для збереження кількості натискань
int pressCount = 0;

// Змінна для відстеження попереднього стану "кнопки"
// Це потрібно, щоб зареєструвати натискання лише один раз
int lastButtonState = HIGH; 

void setup() {
  // Запускаємо монітор порту для виведення результатів
  Serial.begin(115200); 

  // Налаштовуємо піни
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  Serial.println("Лічильник готовий. З'єднайте конектори.");
}

void loop() {
  // 1. Зчитуємо поточний стан конекторів
  int currentButtonState = digitalRead(buttonPin);

  // 2. Перевіряємо, чи відбулося натискання
  // Ми шукаємо перехід від стану HIGH (роз'єднано) до LOW (з'єднано)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    
    // Збільшуємо лічильник
    pressCount++;
    
    // Виводимо результат в консоль
    Serial.print("Кількість натискань: ");
    Serial.println(pressCount);
    
    // Коротко мигаємо світлодіодом для підтвердження
    digitalWrite(ledPin, HIGH);
    delay(50); // Коротка затримка, щоб спалах був помітний
    digitalWrite(ledPin, LOW);
  }

  // 3. Оновлюємо попередній стан для наступної перевірки
  lastButtonState = currentButtonState;
  
  // Невелика затримка для стабільності (антибрязкіт)
  delay(10); 
}