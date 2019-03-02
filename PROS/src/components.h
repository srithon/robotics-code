#include "main.h"

const float PI = 3.1415926;
const float WHEEL_RADIUS = 2.0f; //inches
const float WHEEL_CIRCUMFERENCE = (2.0f * PI) * WHEEL_RADIUS;

const float integralMax = 38586.4; //experimental value (OPTIONAL)
const float kP = 0;
const float kI = 0;
const float kD = 0;

const uint8_t LEFT_MOTOR_FRONT_PORT = 1;
const uint8_t RIGHT_MOTOR_FRONT_PORT = 2;
const uint8_t LEFT_MOTOR_BACK_PORT = 3;
const uint8_t RIGHT_MOTOR_BACK_PORT = 4;
const uint8_t SHOOTER_MOTOR_PORT = 5;
const uint8_t ROLLER_MOTOR_PORT = 6;

const pros::Motor* leftMF = new pros::Motor(LEFT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
const pros::Motor* rightMF = new pros::Motor(RIGHT_MOTOR_FRONT_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
const pros::Motor* leftMB = new pros::Motor(LEFT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
const pros::Motor* rightMB = new pros::Motor(RIGHT_MOTOR_BACK_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
const pros::Motor* shooter = new pros::Motor(SHOOTER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
const pros::Motor* roller = new pros::Motor(ROLLER_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

extern pros::Controller* controller;// = new pros::Controller(pros::E_CONTROLLER_MASTER);

//pros::Competition competition;

const pros::Motor* driveMotors[4] = {leftMF, rightMF, leftMB, rightMB};
