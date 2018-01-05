#include "Turret.h"

Turret::Turret() {
	Serial.begin(115200);
	Serial.println("init turret");

	servoHor.attach(SERVO_PIN_HORIZONTAL);
	servoVert.attach(SERVO_PIN_VERTICAL);
	
	// read current orientations
	orienHor = servoHor.read(); 
	orienVert = servoVert.read();

	Serial.println(orienHor);
	Serial.println(orienVert);
}

Turret::~Turret(){}

bool Turret::UpdateOrientation(float newOrientation, Axis axis) {
	// check for value out of range
	if (newOrientation < 0 || newOrientation > 180) return false;

	Serial.println("updating orientation");

	if (axis == Horizontal) servoHor.write(newOrientation);
	else if (axis == Vertical) servoVert.write(newOrientation);

	return true;
}

void Turret::Shoot() {
	if (shooting) return;
	
	shooting = true;
	digitalWrite(LASER_PIN, true);
}

void Turret::EndCycle() {
	if (shooting) digitalWrite(LASER_PIN, false);
}