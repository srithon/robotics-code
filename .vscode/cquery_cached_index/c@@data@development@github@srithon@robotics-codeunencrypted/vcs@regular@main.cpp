#include "robot-config.h"
#include <math.h>

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

int main(void)
{
	pre_auton();
	comp.autonomous(auton);
	comp.usercontrol(driver);
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
	
	if (col == COLOR::BLUE)
	{
		n *= -1;
	}
	
	if (side = SIDE::FRONT)
	{
		//front code
	}
	else
	{
		//back code
	}
}