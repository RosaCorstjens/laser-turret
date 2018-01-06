#include "Turret.h"

Turret::Turret(Servo servoHor, Servo servoVert) {
	Serial.begin(115200);

	this->servoHor = &servoHor;
	this->servoVert = &servoVert;

	// read current orientations
	orienHor = servoHor.read(); 
	orienVert = servoVert.read();

	goingToStationairy = true;
	reachingHorStationairy = true;
	reachingVertStationairy = true;

	stationairyTarget = Target(90, 90);

	digitalWrite(LASER_PIN, false);
}

Turret::~Turret(){}

bool Turret::GoToStationairy(float deltatime) {
	// first reach vert stationairy
	if (reachingVertStationairy) {
		// check forwards/backwards
		forward = stationairyTarget.ForwardOnAxis(orienVert, Vertical);

		// update orientation
		orienVert += rotationPerSec * deltatime * forward ? 1 : -1;

		RestrictOrientations();
		SetOrientation(orienVert, Vertical);

		// reached
		if (Equals(orienVert, stationairyTarget.GetOrienVer(), 5))
			reachingVertStationairy = false;
		else return false; 
	}

	if (reachingHorStationairy) {
		// check forwards/backwards
		forward = stationairyTarget.ForwardOnAxis(orienHor, Horizontal);
		
		// update orientation
		orienHor += rotationPerSec * deltatime * forward ? 1 : -1;

		RestrictOrientations();
		SetOrientation(orienHor, Horizontal);

		// reached
		if (Equals(orienHor, stationairyTarget.GetOrienHor(), 5))
			reachingHorStationairy = false;
		else return false;
	}

	return true;
}

void Turret::UpdateAutomatic(float deltatime) {
	while (goingToStationairy) {
		if (GoToStationairy(deltatime)) goingToStationairy = false;
		return;
	}

	// while no target is detected 
	RotateOverTime(deltatime);
}

void Turret::RotateOverTime(float deltatime) {
	orienVert += rotationPerSec * deltatime * forward ? 1 : -1;
	
	RestrictOrientations();
	
	SetOrientation(orienVert, Vertical);
}

bool Turret::SetOrientation(float newOrientation, Axis axis) {
	// check for value out of range
	if (!ValidOrientation(newOrientation, axis)) return false;

	if (axis == Horizontal) servoHor->write(newOrientation);
	else if (axis == Vertical) servoVert->write(newOrientation);

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

		if (shootingTime <= 0.0f) {
			shooting = false;
			digitalWrite(LASER_PIN, false);
		}
	}
}

int Turret::MapOrientation(int orientation, Axis axis) {
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