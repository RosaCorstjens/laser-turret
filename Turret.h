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

	bool GoToStationairy(float deltatime);
	void UpdateAutomatic(float deltatime);

	bool UpdateOrientation(float newOrientation, Axis axis);
	void Shoot(float secondsEffect = 0.2f);

	void EndCycle(float deltatime);

	int GetValidOrientation(int orientation, Axis axis);

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
	bool targetDetected = false;

	bool forward = true;
	float rotationPerSec = 0.1f;
	bool goingToStationairy = false;

	bool ValidOrientation(int orientation, Axis axis);
	void RotateOverTime(float deltatime);
};
#endif TURRET
