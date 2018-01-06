#ifndef TURRET
#define TURRET

#include <Servo.h>
#include <Arduino.h>

#define LASER_PIN 12					// laser on/off shooooot!

class Turret {
public:
	enum Axis {
		Horizontal = 0,
		Vertical
	};

	Turret() {}
	Turret(Servo servoHor, Servo servoVert);
	~Turret();

	void Update() {
		orienHor = servoHor->read();
		orienVert = servoVert->read();
	}

	void UpdateAutomatic(float deltatime);
	bool SetOrientation(float newOrientation, Axis axis);

	void Shoot(float secondsEffect = 0.2f);

	void EndCycle(float deltatime);

	int MapOrientation(int orientation, Axis axis);

	bool Equals(int n1, int n2, int diviation) {
		int lowBound = n2 - diviation;
		int highBound = n2 + diviation;

		if (n1 >= lowBound && n1 <= highBound) return true;
		else return false;
	}

private:
	// servo's are for movement (rotations)
	Servo* servoHor;
	Servo* servoVert;

	// current orientation
	float orienHor = 90;
	int orienVert = 90;

	int minHor = 0;
	int maxHor = 180;
	int minVert = 0;
	int maxVert = 180;

	bool shooting = false;
	float shootingTime = 0.0f;

	bool forward = true;
	float rotationPerSec = 0.1f;

	Target stationairyTarget;
	bool goingToStationairy = false;
	bool reachingVertStationairy = false;
	bool reachingHorStationairy = false;

	bool GoToStationairy(float deltatime);
	bool ValidOrientation(int orientation, Axis axis);
	void RotateOverTime(float deltatime);
	void RestrictOrientations() {
		if (orienVert <= 0 && forward) {
			forward = true;
			orienVert = 0;
		}
		if (orienVert >= 180 && !forward) {
			forward = false;
			orienVert = 180;
		}
	}
};

class Target {
public:
	Target() {}
	Target(int orienHor, int orienVer) : orienHor(orienHor), orienVer(orienVer) {}
	~Target();

	int GetOrienHor() { return orienHor; }
	int GetOrienVer() { return orienVer; }

	bool ForwardOnAxis(int currOrien, Turret::Axis axis) {
		int targetOrien = axis == Turret::Horizontal ? GetOrienHor() : GetOrienVer();
		
		if (currOrien < targetOrien){
			return true;
		}
		if (currOrien > targetOrien) {
			return false;
		}
	}

private:
	int orienHor, orienVer = 0;
};
#endif TURRET
