// https://www.ardumotive.com/how-to-use-a-buzzer-en.html
const int buzzerPin = 5; 

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  tone(buzzerPin, 1000); // Play a 1000 Hz tone
  delay(1000); // Wait for 1 second
  noTone(buzzerPin); // Stop the tone
  delay(1000); // Wait for 1 second
}
