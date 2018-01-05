#define MANUAL_PIN 13			// take over control!! MWOEHAOAHAHA
#define BUTTON_PIN 5			// push button

#define POT_PIN_HORIZONTAL A0			// manual, horizontal movement turret
#define POT_PIN_VERTICAL A1				// vertical movement turret

#include <Servo.h>
#include "Turret.h"

Turret turret;

// true: manual controlled, false: automatic
bool manualControlOn;

// input for pot meters
float potHor = 0;
float potVert = 0;
float potHorLast = 0;
float potVertLast = 0;

/// setup: set all pins to set connections
void setup() {
	pinMode(BUTTON_PIN, INPUT);
	pinMode(LASER_PIN, OUTPUT);

	turret = Turret();
}

/// loop: executed every cycle
void loop() {
	// get values from pot meters
	potHor = analogRead(POT_PIN_HORIZONTAL);
	potVert = analogRead(POT_PIN_VERTICAL);

	// map to write values
	// old range: 0-1023 to 0-180 (degree)
	potHor = map(potHor, 0, 1023, 0, 180);
	potVert = map(potVert, 0, 1023, 0, 180);

	// only overwrite if value changed
	// NOT THE ROTATION, DIRECTLY THE ANGLE
	if (potHor == potHorLast) turret.UpdateOrientation(potHor, Turret::Horizontal);
	if (potVert == potVertLast) turret.UpdateOrientation(potVert, Turret::Vertical);

	// capture old values
	potHorLast = potHor;
	potVertLast = potVert;

	// handle laser
	//turret.Shoot();

	/*digitalRead(MANUAL_PIN)*/
	//if (true) handleManual();
	//else handleAutomatic();

	// wait to make chip gappy :)
	delay(15);

	turret.EndCycle();
}

void handleManual() {
	
}

void handleAutomatic() {

}
