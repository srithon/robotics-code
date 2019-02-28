#include "main.h"

static const double PI = 3.1415926535897;
static const double WHEEL_RADIUS = 2.0; //inches
static const double WHEEL_CIRCUMFERENCE = (2 * PI) * WHEEL_RADIUS;

static const float integralMax = 38586.4; //experimental value (OPTIONAL)
static const float kP = 0;
static const float kI = 0;
static const float kD = 0;

static const int LEFT_MOTOR_FRONT_PORT = 1;
static const int RIGHT_MOTOR_FRONT_PORT = 2;
static const int LEFT_MOTOR_BACK_PORT = 3;
static const int RIGHT_MOTOR_BACK_PORT = 4;
static const int SHOOTER_MOTOR_PORT = 5;
static const int ROLLER_MOTOR_PORT = 6;

static const pros::Motor* leftMF = new pros::Motor(LEFT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
static const pros::Motor* rightMF = new pros::Motor(RIGHT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
static const pros::Motor* leftMB = new pros::Motor(LEFT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
static const pros::Motor* rightMB = new pros::Motor(RIGHT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
static const pros::Motor* shooter = new pros::Motor(SHOOTER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
static const pros::Motor* roller = new pros::Motor(ROLLER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

extern pros::Controller* controller;// = new pros::Controller(pros::E_CONTROLLER_MASTER);

//pros::Competition competition;

static const pros::Motor* driveMotors[4] = {leftMF, rightMF, leftMB, rightMB};
