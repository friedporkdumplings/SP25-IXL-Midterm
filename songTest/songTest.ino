int buzzerPin = 5; 
int buttonPin = 2; 

void setup() {
  pinMode(buzzerPin, OUTPUT); 
  pinMode(buttonPin, INPUT_PULLUP); 
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    play(); 
    delay(500); 
  }
}

void play() {
  // "I just wanna be part of your symphony"
  tone(buzzerPin, 311, 120); // D#
  delay(220);
  tone(buzzerPin, 349, 120); // F
  delay(220);
  tone(buzzerPin, 392, 120); // G
  delay(220);
  tone(buzzerPin, 466, 120); // A#
  delay(220);
  tone(buzzerPin, 392, 120); // G
  delay(220);
  tone(buzzerPin, 466, 120); // A#
  delay(220);
  tone(buzzerPin, 523, 120); // C
  delay(220);
  tone(buzzerPin, 622, 120); // D#
  delay(220);
  tone(buzzerPin, 698, 200); // F
  delay(220);
  tone(buzzerPin, 698, 200); // F
  delay(220);
  tone(buzzerPin, 622, 700); // D#
  delay(700);
}