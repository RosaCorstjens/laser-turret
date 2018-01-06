#include "Turret.h"

Turret::Turret() {}

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

	digitalWrite(LASER_PIN, 0);
}

Turret::~Turret(){}

void Turret::Update() {
	orienHor = servoHor->read();
	orienVert = servoVert->read();
}

bool Turret::GoToStationairy(float deltatime) {
	static float waitTime = 10.0f;

	// first reach vert stationairy
	if (reachingVertStationairy) {
		// check forwards/backwards
		forward = stationairyTarget.ForwardOnAxis(orienVert, 0);

		// update orientation
		orienVert += (0.01f * deltatime) * forward ? 1 : -1;

		RestrictOrientations();
		SetOrientation(orienVert, 0);

		// reached
		if (Equals(orienVert, stationairyTarget.GetOrienVer(), 1))
			reachingVertStationairy = false;
		else return false; 
	}

	if (reachingHorStationairy) {
		// check forwards/backwards
		forward = stationairyTarget.ForwardOnAxis(orienHor, 1);
		
		// update orientation
		orienHor += (0.01f * deltatime) * forward ? 1 : -1;

		RestrictOrientations();
		SetOrientation(orienHor, 1);

		// reached
		if (Equals(orienHor, stationairyTarget.GetOrienHor(), 5))
			reachingHorStationairy = false;
		else return false;
	}

	if (waitTime > 0.0f) {
		waitTime -= deltatime;
		return false;
	}
	else {
		waitTime = 10.0f;
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
	orienVert += (rotationPerSec * deltatime) * forward ? 1 : -1;

	RestrictOrientations();
	SetOrientation(orienVert, 0);
}

bool Turret::SetOrientation(float newOrientation, bool axis) {
	// check for value out of range
	if (!ValidOrientation(newOrientation, axis)) return false;

	if (axis) servoHor->write(newOrientation);
	else servoVert->write(newOrientation);

	return true;
}

void Turret::Shoot(float secondsEffect) {
	if (shooting) return;
	
	shooting = true;
	shootingTime = secondsEffect;
	digitalWrite(LASER_PIN, 1);
}

void Turret::EndCycle(float deltatime) {
	if (shootingTime > 0.0f) {
		shootingTime -= deltatime;

		if (shootingTime <= 0.0f) {
			shooting = false;
			digitalWrite(LASER_PIN, 0);
		}
	}
}

int Turret::MapOrientation(int orientation, bool axis) {
	return map(orientation, 0, 1023, 
		axis ? minHor : minVert, 
		axis ? maxHor : maxVert);
}

bool Turret::Equals(int n1, int n2, int diviation) {
	int lowBound = n2 - diviation;
	int highBound = n2 + diviation;

	if (n1 >= lowBound && n1 <= highBound) return true;
	else return false;
}

bool Turret::ValidOrientation(int orientation, bool axis) {
	if (axis) {
		if (orientation < minHor || orientation > maxHor) {
			return false;
		}
		return true;
	}
	else {
		if (orientation < minVert || orientation > maxVert) {
			return false;
		}
		return true;
	}
}

void Turret::RestrictOrientations() {
	if (orienVert <= 0 && !forward) {
		forward = true;
		orienVert = 0;
	}
	if (orienVert >= 180 && forward) {
		forward = false;
		orienVert = 180;
	}
}