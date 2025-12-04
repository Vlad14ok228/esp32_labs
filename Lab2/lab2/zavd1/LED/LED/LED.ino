const int ledPin = 25; // GPIO25

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH); // LED ON
  delay(1000);
  digitalWrite(ledPin, LOW);  // LED OFF
  delay(1000);
}