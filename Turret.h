#ifndef TURRET
#define TURRET

#include <Servo.h>
#include <Arduino.h>
#include "Target.h"

#define LASER_PIN 12					// laser on/off shooooot!

class Turret {
public:
	Turret();
	Turret(Servo servoHor, Servo servoVert);
	~Turret();

	void Update();
	void UpdateAutomatic(float deltatime);
	void EndCycle(float deltatime);
	
	void Shoot(float secondsEffect = 0.2f);

	bool SetOrientation(float newOrientation, bool axis);
	int MapOrientation(int orientation, bool axis);

	bool Equals(int n1, int n2, int diviation);

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
	bool ValidOrientation(int orientation, bool axis);
	void RotateOverTime(float deltatime);
	void RestrictOrientations();
};
#endif TURRET
