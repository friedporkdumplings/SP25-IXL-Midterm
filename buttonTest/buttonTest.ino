int val;
int prevVal;
long startTime; 

// class millis code
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
}

void loop() {
  val = digitalRead(2);

  if (prevVal == LOW && val == HIGH) {
    Serial.println("Pressed");
    startTime = millis(); //starts to record time
  }
  if (prevVal == HIGH && val == LOW) {
    Serial.print("pressed for");
    Serial.println(millis()-startTime); //will calculate actual time it's pressed for 
  }
  prevVal = val;
  delay(10);
}

// const int buttonPin = 2; 

// void setup() {
//   Serial.begin(9600);      
//   pinMode(buttonPin, INPUT_PULLUP);  
// }

// void loop() {
//   int buttonState = digitalRead(buttonPin);

//   if (buttonState == LOW) {
//     Serial.println("pressed");  
//   } else {
//     Serial.println("not pressed");  
//   }

//   delay(100);  
// }