#include <Servo.h>
#include <TM1637Display.h>

#define CLK_PIN 3
#define DIO_PIN 4
const int buzzerPin = 5; 
const int buttonPin = 2;

// servos
Servo servo1;
Servo servo2;
Servo servo3;

// ultrasonic distance pins
int triggerPin = 9;
int echoPin = 7;
long distance;

// led timer display
int count = 0;
bool isCounting = false;
TM1637Display display(CLK_PIN, DIO_PIN);

// buzzer + button states
bool buzzerActive = false;
int buttonState;
int prevButtonState;
long buttonPressStartTime = 0;

// four different states
bool isDefault = true;
bool isCountingState = false;
bool isServosSpinning = false;
bool isBuzzerActive = false;

void setup() {
  Serial.begin(9600);

  // distance sensor
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // servo
  servo1.attach(6);
  servo2.attach(8);
  servo3.attach(10);

  // timer
  display.setBrightness(7);         // Set brightness (0-7)
  display.showNumberDec(0, false);  // Display initial count as 0

  // buzzer
  pinMode(buzzerPin, OUTPUT);

  // button
  pinMode(buttonPin, INPUT);
  prevButtonState = digitalRead(buttonPin);
}

void loop() {
  // functions for what to do in each state
  if (isDefault == true) {
    Default();
  } else if (isCountingState == true) {
    Counting();
  } else if (isServosSpinning == true) {
    ServosSpinning();
    // buzzer state was removed from here because if isBuzzerActive was deleted
  }

  // ultrasonic distance checking
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 17400);
  distance = duration / 29 / 2;
  Serial.println(distance);

  // check to see if button pressed 5 seconds
  if (digitalRead(buttonPin) == LOW) {
    buttonState = digitalRead(buttonPin);

    if (prevButtonState == LOW && buttonState == HIGH) {
      buttonPressStartTime = millis();
    }

    if (prevButtonState == HIGH && buttonState == LOW) {
      // check button pressed least 5 seconds
      if (millis() - buttonPressStartTime >= 5000) {
        resetToDefault();  // reset to default end everything!
      }
    }

    prevButtonState = buttonState;  // update previous button state
  }

  // regardless of state after timer starts it should not stop until grass touched
  if (isCounting == true) {
    count++;
    display.showNumberDec(count, false);
    delay(50);
  }

// replaces isButterState function because keeping it in the loop function ensures it keeps playing sound
// no conflicts with other functions
  if (isBuzzerActive == true) {
    Serial.println("i make sound");
    tone(buzzerPin, 1000);  // high freq tone
  }

  delay(100);
}

void Default() {
  if (distance > 1 && distance < 20) {
    isDefault = false;
    isCounting = true;
    isCountingState = true;
  }
}

void Counting() {
  if (isCounting == true) {
    count++;
    Serial.println("counting works");
    display.showNumberDec(count, false);
    delay(50);

    if (count >= 5) {
      isCountingState = false;
      isServosSpinning = true;
    }
  }
}

void ServosSpinning() {
  // servo spinning angles
  servo1.write(45);
  servo2.write(135);
  servo3.write(45);

  if (count >= 20) {
    Serial.println("servo works");
    isBuzzerActive = true;
  }
}
void resetToDefault() {
  isDefault = true;
  isCountingState = false;
  isServosSpinning = false;
  isBuzzerActive = false;
  isCounting = false;
  count = 0;
  display.showNumberDec(0, false);
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  noTone(buzzerPin);
  tone(buzzerPin, 500, 1000);  // soft tone (return to nature reminder)
}