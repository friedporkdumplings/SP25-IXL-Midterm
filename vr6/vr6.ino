// several debugging print statements
// servos all work hitting and spinning
// end song and default state triggered
// only once at the end 

#include <Servo.h>
#include <TM1637Display.h>

#define CLK_PIN 3
#define DIO_PIN 4
const int buzzerPin = 13;
const int buttonPin = 2;

// servos
Servo servo1;
Servo servo2;
Servo servo3;

// hits hand
Servo servo4;
Servo servo5;

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

bool resetTriggered = false;

// four different states
bool isDefault = true;
bool isCountingState = false;
bool isServosSpinning = false;
bool isBuzzerActive = false;

void setup() {
  Serial.begin(9600);
  Serial.println("intialize program");

  // distance sensor
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // servo
  servo1.attach(6);
  servo2.attach(8);
  servo3.attach(10);
  servo4.attach(11);
  servo5.attach(12);

  // timer
  display.setBrightness(7);         // Set brightness (0-7)
  display.showNumberDec(0, false);  // Display initial count as 0

  // buzzer
  pinMode(buzzerPin, OUTPUT);

  // button
  pinMode(buttonPin, INPUT);
  prevButtonState = digitalRead(buttonPin);

  servo4.write(180);
  servo5.write(0);
}

void loop() {
  // functions for what to do in each state
  if (isDefault == true) {
    Serial.println("default mode");
    Default();
  } else if (isCountingState == true) {
    Serial.println("counting mode");
    Counting();
  } else if (isServosSpinning == true) {
    ServosSpinning();
    Serial.println("spinning mode");
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

  // press for 2 seconds, 5 was too long
  // press only 1 second because too long otherwise
  // there is a really bad delay
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && prevButtonState == HIGH) {
    buttonPressStartTime = millis();
    resetTriggered = false;  // reset trigger after button release
  }

  if (buttonState == LOW && millis() - buttonPressStartTime >= 1000 && !resetTriggered) {
    resetToDefault();
    resetTriggered = true;  // prevent multiple resets regardless of how long button is held down
  }

  Serial.print("initial button state: ");
  prevButtonState = buttonState;

  // regardless of state after timer starts it should not stop until grass touched
  if (isCounting == true) {
    count++;
    display.showNumberDec(count, false);
  }

  // replaces isBuzzerState function because keeping it in the loop function ensures it keeps playing sound
  // no conflicts with other functions
  if (isBuzzerActive == true) {
    Serial.println("i make sound");
    tone(buzzerPin, 3000);  // high freq tone
  }

  delay(20);
}

void Default() {
  if (distance > 1 && distance < 40) {
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
    delay(10);

    if (count >= 10) {
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

  // for the hand hitting servos
  bool increasing = true;  // direction of movement
  int angle = 0;           // servo angle position

  if (count >= 40) {
  if (increasing) {
    for (angle = 0; angle <= 180; angle += 10) {  // 0 to 180
      servo4.write(angle);
      servo5.write(180 - angle);
      delay(20);
    }
  } else {
    for (angle = 180; angle >= 0; angle -= 10) {  // 180 to 0
      servo4.write(angle);
      servo5.write(180 - angle);
      delay(20);
    }
  }

  increasing = !increasing;  // swtich between the directions
  }

  if (count >= 30) {
    isBuzzerActive = true;
  }
}

void resetToDefault() {
  Serial.println("reset is being called");
  Serial.println("reset is being called");
  Serial.println("reset is being called");
  Serial.println("reset is being called");
  Serial.println("reset is being called");

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
  servo4.write(180);
  servo5.write(0);
  // https://www.mintmusic.co.uk/2018/02/symphony-clean-bandit-ft-zara-larsson.html
  // tone(buzzerPin, 500, 1000);  // keep just in case meme song doesn't work
  // "I just wanna be part of your symphony"
  // deleted playSong fuction because it was being
  // activated randomly despite set bool so
  // here's to the more primative way of playing
  // the song at the end
  Serial.println("song mode");
  tone(buzzerPin, 311, 120);  // D#
  delay(220);
  tone(buzzerPin, 349, 120);  // F
  delay(220);
  tone(buzzerPin, 392, 120);  // G
  delay(220);
  tone(buzzerPin, 466, 120);  // A#
  delay(220);
  tone(buzzerPin, 392, 120);  // G
  delay(220);
  tone(buzzerPin, 466, 120);  // A#
  delay(220);
  tone(buzzerPin, 523, 120);  // C
  delay(220);
  tone(buzzerPin, 622, 120);  // D#
  delay(220);
  tone(buzzerPin, 698, 200);  // F
  delay(220);
  tone(buzzerPin, 698, 200);  // F
  delay(220);
  tone(buzzerPin, 622, 1000);  // D#
  delay(1000);
  noTone(buzzerPin);
}
