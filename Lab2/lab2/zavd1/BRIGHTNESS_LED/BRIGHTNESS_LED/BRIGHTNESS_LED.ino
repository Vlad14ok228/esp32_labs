int LDR_Val = 0;       // Змінна для значення фоторезистора
int sensor = 34;       // ADC пін для фоторезистора (GPIO34)
int led = 26;          // Пін для LED

void setup() {
  Serial.begin(9600);   // Швидкість Serial Monitor
  pinMode(led, OUTPUT); // LED як вихід
}

void loop() {
  LDR_Val = analogRead(sensor); // Зчитування значення LDR
  Serial.print("LDR Output Value: ");
  Serial.println(LDR_Val);

  // Якщо темно (значення LDR близько до 4000)
  if (LDR_Val > 3000) {  
    digitalWrite(led, HIGH);  // LED горить
  }
  // Якщо яскраво (значення LDR близько до 0–200)
  else if (LDR_Val < 1500) {  
    digitalWrite(led, LOW);   // LED вимкнено
  }

  delay(200); // Оновлення кожні 0.2 секунди
}