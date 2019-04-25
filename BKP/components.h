#include "main.h"

#ifndef COMPONENTS_H
#define COMPONENTS_H

const double PI = 3.1415926535897;
const double WHEEL_RADIUS = 2.0; //inches
const double WHEEL_CIRCUMFERENCE = (2 * PI) * WHEEL_RADIUS;

const float integralMax = 38586.4; //experimental value (OPTIONAL)
const float kP = 0;
const float kI = 0;
const float kD = 0;

const int LEFT_MOTOR_FRONT_PORT = 1;
const int RIGHT_MOTOR_FRONT_PORT = 2;
const int LEFT_MOTOR_BACK_PORT = 3;
const int RIGHT_MOTOR_BACK_PORT = 4;
const int SHOOTER_MOTOR_PORT = 5;
const int ROLLER_MOTOR_PORT = 6;

pros::Motor lMF(LEFT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rMF(RIGHT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor lMB(LEFT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rMB(RIGHT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor shooter(SHOOTER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor roller(ROLLER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor *driveMotors[4] = {&lMF, &rMF, &lMB, &rMB};

#endif
