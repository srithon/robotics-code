#include "robot-config.h"
#include <math.h>
#include <string>
void auton(void);
void driver(void);
void pre_auton(void);
int getNumDirectionButtonsPressed(void);
bool checkOnlyDirectionButtonPressed(void);
void setUserControlBinds(void);
void moveBot(double, bool);
void moveBot(double, double, bool);
void turnBot(double, double);
void resetRotations(void);
void drawShooter();
void releaseShooter();
int selectAuton();
enum COLOR
{
	RED, BLUE
};

enum SIDE
{
	FRONT, BACK
};

/*********CONFIGURE************/

	COLOR col = COLOR::RED;
	SIDE side = SIDE::FRONT;
	
/*********CONFIGURE************/

int main(void)
{
	pre_auton();
	comp.autonomous(auton);
	comp.drivercontrol(driver);
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
    
   /* if(deg > 0)
	{
        deg += 25;
    }
    else
	{
        speed *= -1;
        deg -= 25;
    }*/
	
    double rad = 7.63;
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

void moveBot(double distance, double speed, bool direction)
{
    resetRotations();
    
    double radius = 2;
    double n = -1.0;
    if (direction)
	{
        n *= -1.0;
    }
    
    double deg = n*360 * (distance / (2*radius*M_PI));
    
    leftMotorF.startRotateTo(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct);
    leftMotorB.startRotateTo(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct);
    rightMotorF.startRotateTo(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct);   
    rightMotorB.rotateTo(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct);
}

inline void pre_auton(void)
{
    int pos = selectAuton();
    Brain.Screen.clearScreen();
    Brain.Screen.print("750Z");
    Brain.Screen.newLine();
    if(pos == 1){
        col = COLOR::RED;
        side = SIDE::FRONT;
    }
    else if(pos == 3){
        col = COLOR::RED;
        side = SIDE::BACK;
    }
    else if(pos == 4){
        col = COLOR::BLUE;
        side = SIDE::FRONT;
    }
    else if(pos == 6){
        col = COLOR::BLUE;
        side = SIDE::BACK;
    }
}
int selectAuton() {
    Brain.Screen.clearScreen();

    Brain.Screen.drawRectangle(10, 80, 140, 50, color::red);
    Brain.Screen.drawRectangle(160, 80, 140, 50, color::red);
    Brain.Screen.drawRectangle(310, 80, 140, 50, color::red);

    Brain.Screen.drawRectangle(10, 150, 140, 50, color::blue);
    Brain.Screen.drawRectangle(160, 150, 140, 50, color::blue);
    Brain.Screen.drawRectangle(310, 150, 140, 50, color::blue);

    Brain.Screen.printAt(31, 105, "Front Flag");
    Brain.Screen.printAt(183, 105, "Front Plat");
    Brain.Screen.printAt(360, 105, "Back");

    Brain.Screen.printAt(31, 175, "Front Flag");
    Brain.Screen.printAt(185, 175, "Front Plat");
    Brain.Screen.printAt(360, 175, "Back");

    while(true) {
        if(Brain.Screen.pressing()) {
            int xPos = Brain.Screen.xPosition();
            int yPos = Brain.Screen.yPosition();

            if(yPos >= 80 && yPos <= 130) {
                if(xPos >= 10 && xPos <= 150) {
                    return 1;
                }
                else if(xPos >= 160 && xPos <= 300) {
                    return 2;
                }
                else if(xPos >= 310 && xPos <= 450){
                    return 3;
                }
            }
            else if(yPos >= 150 && yPos <= 200) {
                if(xPos >= 10 && xPos <= 150) {
                    return 4;
                }
                else if(xPos >= 160 && xPos <= 300) {
                    return 5;
                }
                else if(xPos >= 310 && xPos <= 450){
                    return 6;
                }
            }
        }
    }
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
		rollerMotor.spin(directionType::fwd, 100, velocityUnits::pct);
        
        if (!INTAKE_OUT->pressing())
            rollerMotor2.spin(directionType::rev, 100, velocityUnits::pct);
	});
	INTAKE_IN->released([] ()
	{
        rollerMotor.stop();
        
		if (!INTAKE_OUT->pressing())
		{
            rollerMotor2.stop();
		}
	});
	
	INTAKE_OUT->pressed([] ()
	{
		rollerMotor2.spin(directionType::fwd, 60, velocityUnits::pct);
	});
	INTAKE_OUT->released([] ()
	{
		rollerMotor2.stop();
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
void drawShooter(){
    launcherMotor.rotateTo(1500,vex::rotationUnits::deg,100,velocityUnits::pct);
}
void releaseShooter(){
  //  launcherMotor.spin(directionT)
}
void auton(void)
{
	int n = -1;
	
	if (col == COLOR::BLUE)
	{
		n *= -1;
	}
	
	if (side == SIDE::FRONT)
	{
		//front code
       // moveBot(24.0,5);
       // turnBot(90.0,5);
        moveBot(2.0, 20, true);
        turnBot(n*90.0,12.5);
        drawShooter();
        /*
        moveBot(40,50, true);
        moveBot(8, 20, true);
        moveBot(40, 50, false);
        moveBot(8, 20, false);
        turnBot(-n*90.0,25);
        rollerMotor2.spin(directionType::rev, 70.0, velocityUnits::pct);
        moveBot(42, 50, true);
        moveBot(12, 30, false);
        turnBot(-n*90.0, 25);
        moveBot(14.0, 70, true);
        rollerMotor2.stop(brakeType::brake);*/
        
       // drawShooter();
	}
	else
	{
        Brain.Screen.print("750ZZ");
		rollerMotor2.spin(directionType::rev, 70.0, velocityUnits::pct);
        moveBot(42, 60, true);
        moveBot(6, 40, true);
        moveBot(12,40, false);
        turnBot(n*90, 40);
        moveBot(18, 60, true);
	}
}
