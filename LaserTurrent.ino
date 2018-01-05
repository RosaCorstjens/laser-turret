#include <Servo.h>

#define LASER_PIN 12
#define BUTTON_PIN 5
#define SERVO_PIN_1 9
#define SERVO_PIN_2 10
#define POT_PIN_1 A0
#define POT_PIN_2 A1

Servo servo1, servo2;

int varPot1 = 0;
int varPot2 = 0;
int lastVarPot1 = 0;
int lastVarPot2 = 0;

void setup() {
  pinMode(LASER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
}

void loop() {
  varPot1 = analogRead(POT_PIN_1);
  varPot2 = analogRead(POT_PIN_2);
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
  digitalWrite(LASER_PIN, digitalRead(BUTTON_PIN));
  delay(15);
}
