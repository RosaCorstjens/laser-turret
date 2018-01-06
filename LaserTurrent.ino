#include <Servo.h>
#include "Turret.h"

#define MANUAL_PIN 13			// take over control!! MWOEHAOAHAHA
#define BUTTON_PIN 5			// push button
#define SHOOT_PIN 4		// push button

#define POT_PIN_HORIZONTAL A0			// manual, horizontal movement turret
#define POT_PIN_VERTICAL A1				// vertical movement turret

#define SERVO_PIN_HORIZONTAL 9			// signal servo, horizontal movement
#define SERVO_PIN_VERTICAL 10			// vertical movement

Turret turret;

long startTime, currTime, prevTime, deltaTime;

// true: manual controlled, false: automatic
bool manualControlOn;

// input for pot meters
int potHor, potVert, potHorLast, potVertLast = 0;

// servo instances
Servo servoHor, servoVert;

/// setup: set all pins to set connections
void setup() {
	startTime = millis();

	pinMode(BUTTON_PIN, INPUT);
	pinMode(LASER_PIN, OUTPUT);

	servoHor.attach(SERVO_PIN_HORIZONTAL);
	servoVert.attach(SERVO_PIN_VERTICAL);

	turret = Turret(servoHor, servoVert);
}

/// loop: executed every cycle
void loop() {
	currTime = millis() - startTime;
	deltaTime = (currTime - prevTime);
	prevTime = currTime;

	/*digitalRead(MANUAL_PIN)*/
	if (true) handleAutomatic();
	else handleManual();

	// wait to make chip gappy :)
	delay(15);

	if (digitalRead(SHOOT_PIN)) {
		turret.Shoot(20.0f);
	}
	
	turret.EndCycle(deltaTime);
}

void handleManual() {
	turret.Update();

	// get values from pot meters
	potHor = analogRead(POT_PIN_HORIZONTAL);
	potVert = analogRead(POT_PIN_VERTICAL);

	// map to write values
	// old range: 0-1023 to 0-180 (degree)
	potHor = turret.MapOrientation(potHor, Turret::Horizontal);
	potVert = turret.MapOrientation(potVert, Turret::Vertical);

	// only overwrite if value changed
	// NOT THE ROTATION, DIRECTLY THE ANGLE
	if (potHor == potHorLast) turret.SetOrientation(potHor, Turret::Horizontal);
	if (potVert == potVertLast) turret.SetOrientation(potVert, Turret::Vertical);

	// capture old values
	potHorLast = potHor;
	potVertLast = potVert;

	// handle laser
	turret.Shoot();
}

void handleAutomatic() {
	turret.UpdateAutomatic(deltaTime);
}
