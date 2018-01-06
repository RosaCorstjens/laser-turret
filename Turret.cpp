#include "Turret.h"

Turret::Turret(Servo servoHor, Servo servoVert) {
	Serial.begin(115200);

	this->servoHor = &servoHor;
	this->servoVert = &servoVert;

	// read current orientations
	orienHor = servoHor.read(); 
	orienVert = servoVert.read();

	goingToStationairy = true;
	digitalWrite(LASER_PIN, false);
}

Turret::~Turret(){}

// TODO: werkt nog niet helemaal okee?
bool Turret::GoToStationairy(float deltatime) {
	if (orienHor < 90) {
		orienHor += rotationPerSec * deltatime;
		servoHor->write(orienHor);
		return false;
	}
	if (orienHor > 91) {
		orienHor -= rotationPerSec * deltatime;
		servoHor->write(orienHor);
		return false;
	}

	if (orienVert < 90) {
		orienVert += rotationPerSec * deltatime;
		servoVert->write(orienVert);
		return false;
	}
	if (orienVert > 91) {
		orienVert -= rotationPerSec * deltatime;
		servoVert->write(orienVert);
		return false;
	}

	return true;
}

void Turret::UpdateAutomatic(float deltatime) {
	orienHor = servoHor->read();
	orienVert = servoVert->read();

	//while (goingToStationairy) {
	//	if (GoToStationairy(deltatime)) goingToStationairy = false;
	//}

	if(forward)
		orienVert -= rotationPerSec * deltatime;
	else
		orienVert += rotationPerSec * deltatime;

	if (orienVert <= 0 && forward) {
		forward = false;
		orienVert = 0;
	}
	if (orienVert >= 180 && !forward) {
		forward = true;
		orienVert = 180;
	}
	servoVert->write(orienVert);
}

bool Turret::UpdateOrientation(float newOrientation, Axis axis) {
	// check for value out of range
	if (ValidOrientation(newOrientation, axis)) return false;

	if (axis == Horizontal) servoHor->write(newOrientation);
	else if (axis == Vertical) servoVert->write(newOrientation);

	orienHor = servoHor->read();
	orienVert = servoVert->read();

	return true;
}

void Turret::Shoot(float secondsEffect) {
	if (shooting) return;
	
	shooting = true;
	shootingTime = secondsEffect;
	digitalWrite(LASER_PIN, true);
}

void Turret::EndCycle(float deltatime) {
	Serial.println(shootingTime);

	if (shootingTime > 0.0f) {
		shootingTime -= deltatime;
	}

	if (shootingTime <= 0.0f) {
		shooting = false;
		digitalWrite(LASER_PIN, false);
	}
}

int Turret::GetValidOrientation(int orientation, Axis axis) {
	return map(orientation, 0, 1023, 
		axis == Horizontal ? minHor : minVert, 
		axis == Horizontal ? maxHor : maxVert);
}

bool Turret::ValidOrientation(int orientation, Axis axis) {
	if (axis == Horizontal) {
		if (orientation < minHor || orientation > maxHor) {
			return false;
		}
		return true;
	}

	if (axis == Vertical) {
		if (orientation < minVert || orientation > maxVert) {
			return false;
		}
		return true;
	}
}