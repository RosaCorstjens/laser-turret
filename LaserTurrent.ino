#include <Servo.h>

Servo servo1;
Servo servo2;

#define laserPin 12
#define buttonPin 5
#define servoPin1 9
#define servoPin2 10
#define potPin1 A0
#define potPin2 A1

int varPot1 = 0;
int varPot2 = 0;
int lastVarPot1 = 0;
int lastVarPot2 = 0;

void setup() {
  pinMode(laserPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
}

void loop() {
  varPot1 = analogRead(potPin1);
  varPot2 = analogRead(potPin2);
  varPot1 = map(varPot1, 0, 1023, 0, 180);
  varPot2 = map(varPot2, 0, 1023, 0, 180);
  if(varPot1 == lastVarPot1){
    servo1.write(varPot1);
  }
  if(varPot2 == lastVarPot2){
    servo2.write(varPot2);
  }
  lastVarPot1 = varPot1;
  lastVarPot2 = varPot2;
  digitalWrite(laserPin, digitalRead(buttonPin));
  delay(15);
}
