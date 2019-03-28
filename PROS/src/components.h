#include "main.h"

#ifndef COMPONENTS_H
#define COMPONENTS_H

const float PI = 3.1415926;
const float WHEEL_RADIUS = 2.0f; //inches
const float WHEEL_CIRCUMFERENCE = (2.0f * PI) * WHEEL_RADIUS;

const double ENCODER_TICKS_PER_ROTATION = 347.991801;
const float BOT_RADIUS = 7.63f;

const float integralMax = 38586.4; //experimental value (OPTIONAL)
const float kP = 12.5f;//0.210913f;
const float kI = 1.8f;
const float kD = 0.268f;

const uint8_t LEFT_MOTOR_FRONT_PORT = 2;
const uint8_t RIGHT_MOTOR_FRONT_PORT = 10;
const uint8_t LEFT_MOTOR_BACK_PORT = 11;
const uint8_t RIGHT_MOTOR_BACK_PORT = 19;
const uint8_t SHOOTER_MOTOR_PORT = 8;
const uint8_t ROLLER_MOTOR_PORT = 7;
const uint8_t ROLLER_MOTOR2_PORT = 20;

const extern pros::Motor* leftMF;// = new pros::Motor(LEFT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
const extern pros::Motor* rightMF;// = new pros::Motor(RIGHT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
const extern pros::Motor* leftMB;// = new pros::Motor(LEFT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
const extern pros::Motor* rightMB;// = new pros::Motor(RIGHT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
const extern pros::Motor* shooter;// = new pros::Motor(SHOOTER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
const extern pros::Motor* flipperRoller;// = new pros::Motor(ROLLER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
const extern pros::Motor* intakeRoller;

const extern pros::Motor* driveMotors[4];// = {leftMF, rightMF, leftMB, rightMB};

//extern pros::Controller* controller;// = new pros::Controller(pros::E_CONTROLLER_MASTER);

//pros::Competition competition;

#endif
