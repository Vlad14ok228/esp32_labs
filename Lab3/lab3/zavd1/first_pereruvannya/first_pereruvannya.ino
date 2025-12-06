// Піни, які ми використовуємо згідно вашої схеми
const int ledPin = 21;
const int buttonPin = 18; // Пін для вашого конектора

void setup() {
  // Налаштовуємо пін світлодіода як вихід
  pinMode(ledPin, OUTPUT);
  
  // Дуже важливий рядок!
  // Налаштовуємо пін "кнопки" як вхід з увімкненим 
  // внутрішнім підтягуючим резистором.
  // Це означає, що пін 18 завжди буде HIGH (3.3V), 
  // поки ви не з'єднаєте його з GND.
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Зчитуємо стан піна 18
  int buttonState = digitalRead(buttonPin);

  // Коли ви з'єднуєте конектори, пін 18 з'єднується з GND, 
  // і його стан стає LOW (низький).
  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH); // Вмикаємо світлодіод
  } else {
    // Якщо конектори не з'єднані, стан буде HIGH
    digitalWrite(ledPin, LOW);  // Вимикаємо світлодіод
  }
}