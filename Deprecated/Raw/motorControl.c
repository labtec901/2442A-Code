#ifndef MOTORCONTROL_C_INCLUDED
#define MOTORCONTROL_C_INCLUDED

#ifndef MOTORCONTROL_H_INCLUDED
#include "motorControl.h"
#endif

//Array for motors
static driveMotor driveMotors[MOTOR_NUM];

/*
* Adds and initializes a motor (by name) to the motor array
*/
driveMotor* addMotor(const tMotor name, float slewRate)
{
	//Grab a spot in the motor array which
	//corresponds with which port the
	//physical motor is plugged into
	driveMotor *m = &(driveMotors[name]);

	m->reqSpeed = 0;
	m->artSpeed = 0;
	m->slew = slewRate;
	writeDebugStreamLine("%d", MOTOR_NUM);
	return m;
}

/*
* Updates the power of each motor to meet the requested power
*/
task motorSlewRateTask()
{
	//Index of current motor
	unsigned int motorIndex;
	//Requested speed of current motor
	int motorTmpReq;
	//Artificial speed of current motor
	float motorTmpArtSpd;
	//Current motor
	driveMotor *currentMotor;

	while (true)
	{
		for (motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
		{
			//Keep internal memory access to a minimum
			currentMotor = &(driveMotors[motorIndex]);
			motorTmpArtSpd = currentMotor->artSpeed;
			writeDebugStreamLine("%2.6f", motorTmpArtSpd);
			motorTmpReq = currentMotor->reqSpeed;

			//If the motor value needs to change
			if (motorTmpArtSpd != motorTmpReq)
			{
				//Increase motor value
				if (motorTmpReq > motorTmpArtSpd)
				{
					motorTmpArtSpd += currentMotor->slew;

					//Limit speed
					motorTmpArtSpd = motorTmpArtSpd > motorTmpReq ? motorTmpReq : motorTmpArtSpd;
				}

				//Decrease motor value
				if (motorTmpReq < motorTmpArtSpd)
				{
					motorTmpArtSpd -= currentMotor->slew;

					//Limit speed
					motorTmpArtSpd = motorTmpArtSpd < motorTmpReq ? motorTmpReq : motorTmpArtSpd;
				}

				//Bound speed
				motorTmpArtSpd = motorTmpArtSpd > MOTOR_MAX_VALUE ? MOTOR_MAX_VALUE : motorTmpArtSpd;
				motorTmpArtSpd = motorTmpArtSpd < MOTOR_MIN_VALUE ? MOTOR_MIN_VALUE : motorTmpArtSpd;

				//Send updated speed to motor
				motor[motorIndex] = (int)motorTmpArtSpd;

				//Send updated speed back to current motor
				currentMotor->artSpeed = motorTmpArtSpd;
			}
		}
		wait1Msec(MOTOR_TASK_DELAY);
	}
}

#endif //#ifndef MOTORCONTROL_C_INCLUDED
