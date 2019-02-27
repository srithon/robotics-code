#include "robot-config.h"
#include <math.h>

/*
 *  Mass * Radius of Wheel * (CONSTANT) Velocity^2
 * ------------------------------------------------
 * 			8 * Torque of the Motor
 * 
 */

void auton(void);
void driver(void);
void pre_auton(void);
int getNumDirectionButtonsPressed(void);
bool checkOnlyDirectionButtonPressed(void);
void setUserControlBinds(void);
void moveBot(double, bool);
void moveBot(double, bool, double);
void turnBot(double, double);
void resetRotations(void);
void turnDeceleration(double, double);
void firstMoveAuton(double, double);
void torqueGetter(void);
void wait(double);
void simulateControllerInput(double, double, double, bool);
inline float frontMotorsValAverage(void);
inline float inchesToTicks(float);

enum COLOR
{
	RED, BLUE;
}

enum SIDE
{
	FRONT, BACK;
}

/*********CONFIGURE************/

	COLOR col = COLOR::RED;
	SIDE side = SIDE::FRONT;
	
/*********CONFIGURE************/

double torqueSum;
int torqueIterations;
double torqueAverage = 0;
bool stop = true;
double friction;

float averageEncoderValue = 0.0;

float kP;
float kI;
float kD;

const unsigned float integralMax = 38586.4; //experimental value (OPTIONAL)
//prevents integral from overpowering the rest

//float lastTime = time_t;

int main(void)
{
	pre_auton();
	comp.autonomous(auton);
	comp.usercontrol(driver);
}

void pidTurn(float deg)
{
	resetRotations();
	
	/*if(deg > 0)
	{
        deg += 25;
    }
    else
	{
        speed *= -1;
        deg -= 25;
    }*/
	
    double arcLength = (2.0*rad*M_PI)*(deg/360.0);
    double rot = 360.0*(arcLength/(4.0*M_PI));
    
	float desiredDegs = rot;
	float currentDegs = 0;
	
	float lastError = 0;
	float error = 0;
	float errorSum = 0;
	
	float rateErrorChange;
	float motorSpinVelocity;
	
	//lastTime = time_t;
	
	while (currentDegs < desiredDegs)
	{
		currentDegs += effectiveRotationValue(); // this or averageEncoderValue() - not sure
		
		//proportional
		error = (desiredDegs - currentDegs);
		
		//integral
		if (errorSum < integralMax)
			errorSum += error;
		else
			errorSum = integralMax;
		
		//derivative
		rateErrorChange = (error - lastError);// / 16;//(getChangeInTime());
		
		motorSpinVelocity = (error * kP) + (errorSum * kI) + (rateErrorChange * kD);
		
		rightMotorF.spin(motorSpinVelocity, directionType::fwd);
		rightMotorB.spin(motorSpinVelocity, directionType::fwd);
		leftMotorF.spin(motorSpinVelocity, directionType::rev);
		leftMotorB.spin(motorSpinVelocity, directionType::rev);
		
		lastError = error;
		
		task::sleep(15);
	}
}

float averageEncoderValue()
{
	float temp = 0.0;
	
	temp += rightMotorF.rotation(rotationUnits::deg);
	temp += rightMotorB.rotation(rotationUnits::deg);
	temp += leftMotorF.rotation(rotationUnits::deg);
	temp += leftMotorB.rotation(rotationUnits::deg);
	
	return temp / 4.0;
}

inline float getChangeInTime()
{
	float t = lastTime;
	lastTime = time_t;
	return time_t - t;
}

inline float effectiveRotationValue()
{
	return (0 - leftMotorF.rotation(rotationUnits::deg)
	+ rightMotorB.rotation(rotationUnits::deg)) / 2.0;
}

void simulateControllerInput(double x, double y, double timeOrDistance, bool time)
{
	resetRotations();
	
	if (time)
	{
		double start = time_t;
		
		rightMotorF.spin(directionType::fwd, (y - x), velocityUnits::pct);
		leftMotorF.spin(directionType::fwd, (y + x), velocityUnits::pct);
		rightMotorB.spin(directionType::fwd, (y - x), velocityUnits::pct);
		leftMotorB.spin(directionType::fwd, (y + x), velocityUnits::pct);
		
		while (time_t - start < timeOrDistance) //time
		{
			task::sleep(20);
		}
		
		rightMotorF.stop(brakeType::brake);
		rightMotorB.stop(brakeType::brake);
		leftMotorF.stop(brakeType::brake);
		leftMotorB.stop(brakeType::brake);
	}
	else
	{
		float initAv = frontMotorsValAverage();
		float numDegs = inchesToDegrees(timeOrDistance); //distance
		
		rightMotorF.spin(directionType::fwd, (y - x), velocityUnits::pct);
		leftMotorF.spin(directionType::fwd, (y + x), velocityUnits::pct);
		rightMotorB.spin(directionType::fwd, (y - x), velocityUnits::pct);
		leftMotorB.spin(directionType::fwd, (y + x), velocityUnits::pct);
		
		while (frontMotorsValAverage() - initAv < numDegs)
		{
			task::sleep(20);
		}
		
		rightMotorF.stop(brakeType::brake);
		rightMotorB.stop(brakeType::brake);
		leftMotorF.stop(brakeType::brake);
		leftMotorB.stop(brakeType::brake);
	}
}

inline float degreesToTicks(float degrees)
{
	return degrees * (90.0 / 36.0);
	//FIGURE THIS OUT
	//900 ticks / 1 revolution of wheel
	//900 ticks / 360 degrees
	//90 ticks / 36 degrees
	//deg * 90/36
}

inline float inchesToDegrees(float inches)
{
    return 360 * (inches / (2 * 2 * M_PI));
}

inline float frontMotorsValAverage(void)
{
	 return (leftMotorF.rotation(rotationUnits::deg)
							+ rightMotorF.rotation(rotationUnits::deg)) / 2.0;
}

void turnFakePID(double degrees, double initialVelocity)
{
	resetRotations();
    
    if(deg > 0)
	{
        deg += 25;
    }
    else
	{
        speed *= -1;
        deg -= 25;
    }
	
    double rad = 6.5;
    double arcLength = (2.0 * rad * M_PI) * (deg / 360.0);
    double rot = 360.0 * (arcLength / (4.0 * M_PI));
	
    leftMotorB.startRotateTo(rot / 2, vex::rotationUnits::deg, speed, vex::velocityUnits::pct);
    leftMotorF.startRotateTo(rot / 2, vex::rotationUnits::deg, speed, vex::velocityUnits::pct);
    rightMotorB.startRotateTo(-1 * rot / 2, vex::rotationUnits::deg, -1*speed, vex::velocityUnits::pct);
    rightMotorF.rotateTo(-1 * rot / 2, vex::rotationUnits::deg, -1*speed, vex::velocityUnits::pct);
}

void firstMoveAuton(double inches, double vel)
{
	resetRotations();
    
    double radius = 2;
    double n = -1.0;
    if (direction)
	{
        n *= -1.0;
    }
    
    double deg = n*360 * (distance / (2*radius*M_PI));
	
	thread torqueGetterThread = thread(torqueGetter);
    
    leftMotorF.startRotateTo(deg, vex::rotationUnits::deg, n * 90, vex::velocityUnits::rpm);
    leftMotorB.startRotateTo(deg, vex::rotationUnits::deg, n * 90, vex::velocityUnits::rpm);
    rightMotorF.startRotateTo(deg, vex::rotationUnits::deg, n * 90, vex::velocityUnits::rpm);   
    rightMotorB.rotateTo(deg, vex::rotationUnits::deg, n * 90, vex::velocityUnits::rpm);
	
	stop = true;
	torqueGetterThread.interrupt();

	torqueAverage = torqueSum / torqueIterations;
	
	/*
	*  Mass * Radius of Wheel * (CONSTANT) Velocity^2
	* ------------------------------------------------
	* 			8 * Torque of the Motor
	* 
	*/
	
	double velocityInMS = (vel * (2 * M_PI) * radius) / 60.0; //velocityInMeters/Second
	
	friction = (MASS * 2 * (velocityInMS * velocityInMS)) / (8 * torqueAverage);
	
	/***********NEED	MASS***********/
}

void torqueGetter(void)
{
	while (true)
	{
		task::sleep(torqueGetterDelay - 5);
		
		int num = 0;
		
		double torqueAverageTemp = 0;
		
		if (leftMotorF.torque(torqueUnits::Nm) > (torqueAverage / 2))
		{
			num++;
			torqueAverageTemp += leftMotorF.torque(torqueUnits::Nm);
		}
		if (leftMotorB.torque(torqueUnits::Nm) > (torqueAverage / 2))
		{
			num++;
			torqueAverageTemp += leftMotorB.torque(torqueUnits::Nm);
		}
		if (rightMotorF.torque(torqueUnits::Nm) > (torqueAverage / 2))
		{
			num++;
			torqueAverageTemp += rightMotorF.torque(torqueUnits::Nm);
		}
		if (rightMotorB.torque(torqueUnits::Nm) > (torqueAverage / 2))
		{
			num++;
			torqueAverageTemp += rightMotorB.torque(torqueUnits::Nm);
		}
		
		torqueSum += (torqueAverageTemp / num);
		torqueIterations++;
		
		if (!stop)
		{
			torqueAverage = torqueSum / torqueIterations;
		}
	}
}

void resetRotations(void)
{
    leftMotorF.resetRotation();
    leftMotorB.resetRotation();
    rightMotorB.resetRotation();
    rightMotorF.resetRotation();
}

void turnBot(double deg, double speed)
{
    resetRotations();
    
    if(deg > 0)
	{
        deg += 25;
    }
    else
	{
        speed *= -1;
        deg -= 25;
    }
	
    double rad = 6.5;
    double arcLength = (2.0*rad*M_PI)*(deg/360.0);
    double rot = 360.0*(arcLength/(4.0*M_PI));
    leftMotorB.startRotateTo(rot,vex::rotationUnits::deg,speed,vex::velocityUnits::pct);
    leftMotorF.startRotateTo(rot,vex::rotationUnits::deg,speed,vex::velocityUnits::pct);
    rightMotorB.startRotateTo(-1*rot,vex::rotationUnits::deg,-1*speed,vex::velocityUnits::pct);
    rightMotorF.rotateTo(-1*rot,vex::rotationUnits::deg,-1*speed,vex::velocityUnits::pct);
}

void moveBot(double distance, bool direction)
{
    resetRotations();
    
    double radius = 2;
    double n = -1.0;
    if (direction)
	{
        n *= -1.0;
    }
    
    double deg = n*360 * (distance / (2*radius*M_PI));
    
    leftMotorF.startRotateTo(deg, vex::rotationUnits::deg, n * 90, vex::velocityUnits::rpm);
    leftMotorB.startRotateTo(deg, vex::rotationUnits::deg, n * 90, vex::velocityUnits::rpm);
    rightMotorF.startRotateTo(deg, vex::rotationUnits::deg, n * 90, vex::velocityUnits::rpm);   
    rightMotorB.rotateTo(deg, vex::rotationUnits::deg, n * 90, vex::velocityUnits::rpm);
}

inline void pre_auton(void)
{
	Brain.Screen.print("750Z");
}

int getNumDirectionButtonsPressed(void)
{
	int n = 0;
	
    /*
    Replace with
    
    if (SLOW_LEFT->pressing())
		n++;
	if (SLOW_RIGHT->pressing())
		n++;
	if (SLOW_UP->pressing())
		n++;
    
    if using diff buttons
    */
    
	if (Controller1.ButtonLeft.pressing())
		n++;
	if (Controller1.ButtonRight.pressing())
		n++;
	if (Controller1.ButtonUp.pressing())
		n++;
		
	return n;
}

inline bool checkOnlyDirectionButtonPressed(void)
{ //returns true if only one direction button is pressed
	return (getNumDirectionButtonsPressed() == 1);
}

// sets controller binds for user control
void setUserControlBinds(void)
{
	/* RIGHT BUMPERS */
	LAUNCH_BUTTON->pressed([] ()
	{ // these are called anonymous functions
		launcherMotor.spin(directionType::fwd, 100, velocityUnits::pct);
	});
	LAUNCH_BUTTON->released([] ()
	{
		launcherMotor.stop();
	});
	
	/* LEFT BUMPERS	*/
	INTAKE_IN->pressed([] ()
	{
		if (!INTAKE_OUT->pressing())
		{ //if ButtonL2 is pressed, don't do anything
			rollerMotor.spin(directionType::rev, 100, velocityUnits::pct);
		}
	});
	INTAKE_IN->released([] ()
	{
		if (!INTAKE_OUT->pressing())
		{ //if ButtonL2 is pressed, don't do anything
			rollerMotor.stop();
		}
	});
	
	INTAKE_OUT->pressed([] ()
	{
		if (!INTAKE_IN->pressing())
		{ //if ButtonL1 is pressed, don't do anything
			rollerMotor.spin(directionType::fwd, 100, velocityUnits::pct);
		}
	});
	INTAKE_OUT->released([] ()
	{
		if (!INTAKE_IN->pressing())
		{ //if ButtonL1 is pressed, don't do anything
			rollerMotor.stop();
		}
	});
	
	/*	DIRECTION BUTTONS	*/
	SLOW_LEFT->pressed([] ()
	{
		/*
		 * ONLY RUNS THE CODE IF NONE OF THE OTHER
		 * DIRECTION BUTTONS ARE PRESSED
		 */
		 
		if (checkOnlyDirectionButtonPressed())
		{
			rightMotorF.spin(directionType::fwd, 7, velocityUnits::pct);
            leftMotorF.spin(directionType::rev, 7, velocityUnits::pct);
            rightMotorB.spin(directionType::fwd, 7, velocityUnits::pct);
            leftMotorB.spin(directionType::rev, 7, velocityUnits::pct);
		}
	});
	SLOW_RIGHT->pressed([] ()
	{
		if (checkOnlyDirectionButtonPressed())
		{
			rightMotorF.spin(directionType::rev, 7, velocityUnits::pct);
            leftMotorF.spin(directionType::fwd, 7, velocityUnits::pct);
            rightMotorB.spin(directionType::rev, 7, velocityUnits::pct);
            leftMotorB.spin(directionType::fwd, 7, velocityUnits::pct);
		}
	});
	SLOW_UP->pressed([] ()
	{
		if (checkOnlyDirectionButtonPressed())
		{
			rightMotorF.spin(directionType::fwd, 35, velocityUnits::pct);
            leftMotorF.spin(directionType::fwd, 35, velocityUnits::pct);
            rightMotorB.spin(directionType::fwd, 35, velocityUnits::pct);
            leftMotorB.spin(directionType::fwd, 35, velocityUnits::pct);
		}
	});
}

void driver(void)
{
	setUserControlBinds();

    while (1)
	{
		rightMotorF.spin(directionType::fwd, (VERTICAL_AXIS->value() - HORIZONTAL_AXIS->value()), velocityUnits::pct);
		leftMotorF.spin(directionType::fwd, (VERTICAL_AXIS->value() + HORIZONTAL_AXIS->value()), velocityUnits::pct);
		rightMotorB.spin(directionType::fwd, (VERTICAL_AXIS->value() - HORIZONTAL_AXIS->value()), velocityUnits::pct);
		leftMotorB.spin(directionType::fwd, (VERTICAL_AXIS->value() + HORIZONTAL_AXIS->value()), velocityUnits::pct);
		
        task::sleep(20);
    }
}

void auton(void)
{
	int n = 1;
	
	if (col == COLOR::RED)
	{
		n *= -1;
	}
	
	if (side = SIDE::BACK)
	{
		rollerMotor.spin(100, directionType::fwd);
		moveBot(46);
		task::sleep(500);
		moveBot(-17.69);
		task::sleep(500);
		turnBot(90 * n, 50);
		task::sleep(500);
		moveBot(37); //27.39
		task::sleep(1000);
	}
	else
	{
		
	}
}
