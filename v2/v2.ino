// servos move in when user is in front of distance sensor
// timer starts when user detected
// when users leave servos stop 
// user returns servo spins again timer restarts from 1

#include <Servo.h>
#include <TM1637Display.h>


#define CLK_PIN 3
#define DIO_PIN 4


Servo servo1;
Servo servo2;
Servo servo3;
int triggerPin = 9;
int echoPin = 7;
long distance;
bool isCounting = false;


int count = 0;


TM1637Display display(CLK_PIN, DIO_PIN);


void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo1.attach(6);
  servo2.attach(8);
  servo3.attach(10);


  display.setBrightness(7); // Set brightness (0-7)
  display.showNumberDec(0, false); // Display initial count as 0
}


void loop() {
  // additional 2 microsecond delay to ensure pulse clarity
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);  // pulseIn waits for signal to go from HIGH to LOW,
  // timeout according to max range of sensor
  long duration = pulseIn(echoPin, HIGH, 17400);
  // sound travels roughly 29cm per microsecond so we divide by 29,
  // then by 2 since we recorded sound both going forth and back
  distance = duration / 29 / 2;
  Serial.println(distance);
 
 
  if (distance > 1 && distance < 50) {
    servo1.write(45);
    servo2.write(135);
    servo3.write(45);
  } else {
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    delay(500);
  }




  if (distance > 1 && distance < 20) {
    count = count + 1;
    display.showNumberDec(count, false);
    delay(50);
  } else {
    // Stop counting
    isCounting = false;
    count = 0;
  }


  delay(100); // Small delay to stabilize the loop