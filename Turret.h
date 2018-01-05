#ifndef TURRET
#define TURRET

#include <Servo.h>
#include <Arduino.h>

#define SERVO_PIN_HORIZONTAL 9			// signal servo, horizontal movement
#define SERVO_PIN_VERTICAL 10			// vertical movement
#define LASER_PIN 12					// laser on/off shooooot!

class Turret {
public:
	enum Axis {
		Horizontal = 0,
		Vertical
	};

	Turret();
	~Turret();

	bool UpdateOrientation(float newOrientation, Axis axis);
	void Shoot();

	void EndCycle();
	
	Servo servoHor;
	Servo servoVert;

private:
	// servo's are for movement (rotations)

	// current orientation
	int orienHor = 90;
	int orienVert = 90;

	bool shooting = false;
	bool targetDetected = false;
};
#endif TURRET
