#include "main.h"

#ifndef COMPONENTS_H
#define COMPONENTS_H

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

static pros::Motor leftMF(LEFT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
static pros::Motor rightMF(RIGHT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
static pros::Motor leftMB(LEFT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
static pros::Motor rightMB(RIGHT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
static pros::Motor shooter(SHOOTER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
static pros::Motor roller(ROLLER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

static pros::Controller controller(pros::E_CONTROLLER_MASTER);
//pros::Competition competition;

static pros::Motor *driveMotors[4] = {&leftMF, &rightMF, &leftMB, &rightMB};

#endif
