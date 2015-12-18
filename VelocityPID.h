#ifndef VELOCITYPID_H_INCLUDED
#define VELOCITYPID_H_INCLUDED

//This scale constant relates mesured ticks per second to motor power
#define PID_SCALE 2.67

//This scale relates degrees per millisecond to rpm
#define DEGPMS_TO_RPM 166.7

//A velocity PID controller
typedef struct vel_PID
{
	//PID constants
	float kP;
	float kI;
	float kD;
	float upperBound;
	float lowerBound;

	//PID calculations
	float currentVelocity;
	int prevPosition;
	int error;
	int prevError;
	int integral;
	int derivative;

	//Timestep
	float dt;
	int prevTime;

	//PID limits
	int errorSumThreshold;
	int integralCap;

	//Input
	tSensors sensor;
	int currentPosition;
	float targetVelocity;

	//Output
	float outVal;
} vel_PID;

#endif
