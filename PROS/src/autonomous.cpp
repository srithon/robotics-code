#include "components.h"
#include <thread>

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous(void);
void moveBot(double, int32_t, bool);
void pidTurn(float);
void rotateDriveMotors(double, std::int32_t, bool = 1, bool = 1, bool = 1, bool = 1);
float getPosition(bool);
void resetMotorRotations(void);
void turn(float);
void stopMotors(bool = 1, bool = 1, bool = 1, bool = 1);
void block(const pros::Motor* motorToBlock, float relativePosition);
void move(double ticks);
//use the explicit keyword to prevent conversions of float types?

void autonomous()
{
  //pros::lcd::set_text(1, "Ran autonomous!");
  //moveBot(12.0, static_cast<int32_t>(50), true);
  //move(ENCODER_TICKS_PER_ROTATION);
  //pros::delay(1000);
  //pidTurn(90.0f);

//  turn(90.0f);
  //pros::lcd::set_text(2, "Finished Turn");

//  while (true)
//    pros::delay(5000);

  pros::lcd::set_text(1, std::to_string(leftMF->get_position()));
  rotateDriveMotors(1000, 100);
  block(leftMF, 1000);

  pros::lcd::set_text(2, std::to_string(leftMF->get_position()));
  rotateDriveMotors(-1000, 100);
  block(leftMF, -1000);

  pros::lcd::set_text(3, std::to_string(leftMF->get_position()));
  rotateDriveMotors(-400, 100);
  block(leftMF, -400);

  pros::lcd::set_text(4, std::to_string(leftMF->get_position()));
}

void turn(float deg) //c
{
  float arcLength = 2.0 * BOT_RADIUS * PI * (deg / 360.0f);
  //float rot = 360.0f*(arcLength/(4.0f*PI)); //degrees
  float rot = (arcLength/(4.0f*PI)) * (ENCODER_TICKS_PER_ROTATION); //ticks

  leftMB->move_relative(rot, 50);
  leftMF->move_relative(rot, 50);
  rightMB->move_relative(-1*rot, 50);
  rightMF->move_relative(-1*rot, 50);

  block(rightMF, rot);

  pros::lcd::set_text(3, "Average Position - " + std::to_string(getPosition(true)));
  double degreesTurned = getPosition(true) / (ENCODER_TICKS_PER_ROTATION * 2.0 * BOT_RADIUS * PI) * (4.0f * PI) * 360;
  pros::lcd::set_text(4, "Degrees Turned - " + std::to_string(degreesTurned));
}

void move(double ticks)
{
  leftMB->move_relative(ticks, 200);
  leftMF->move_relative(ticks, 200);
  rightMB->move_relative(ticks, 200);
  rightMF->move_relative(ticks, 200);
}

void block(const pros::Motor* motorToBlock, float desiredPosition) //desiredPosition has to be in ticks
{
  double finalPosition = std::abs(motorToBlock->get_position()) + std::abs(desiredPosition);

//  pros::lcd::set_text(5, "Desired Position In Block - " + std::to_string(finalPosition));

//  short iteration = 0;

  while (std::abs(motorToBlock->get_position()) < finalPosition)
  {
    pros::delay(125);
    //pros::lcd::set_text(7, std::to_string(iteration++) + " " + std::to_string(motorToBlock->get_position()));
  }

//  pros::lcd::set_text(7, "Done blocking");
}

//Positive is clockwise, Negative is counter-clockwise
void pidTurn(float desiredDegrees)
{
  float desiredTicks = (desiredDegrees * BOT_RADIUS) / (360 * WHEEL_RADIUS);

  float cError = desiredTicks;
  float lError = desiredTicks;
  float errSum = 0;

  while (std::abs(cError) < 0.01)
  {
    cError = (desiredTicks - leftMF->get_position());

    if (std::abs(errSum) > INTEGRAL_MAX)
      errSum += cError;	

    float speed = (kP * cError) + (kI * errSum) + (kD * (cError - lError));

    rotateDriveMotors(speed);

    lError = cError;
  }
}

void moveBot(double inches, int32_t velocity, bool direction)
{
  //double desiredTicks = inches / (360.0 * static_cast<double>(WHEEL_CIRCUMFERENCE));
  double desiredTicks = (ENCODER_TICKS_PER_ROTATION * (inches / WHEEL_CIRCUMFERENCE));

  pros::lcd::set_text(1, ("Deg = " + std::to_string(desiredTicks)));

  if (!direction)
    desiredTicks = -desiredTicks;

  pros::delay(2000);

  //double averagePosition = getAveragePosition();

  //rotateDriveMotors(desiredTicks, velocity);

  double lMBEndingPosition = leftMB->get_position() + desiredTicks;

  rightMF->move_relative(desiredTicks, velocity);
  leftMF->move_relative(desiredTicks, velocity);
  rightMB->move_relative(desiredTicks, velocity);
  leftMB->move_relative(desiredTicks, velocity);

  //while (getAveragePosition() < averagePosition + encoderTicks)

  int iter = 0;

  while (leftMB->get_position() < lMBEndingPosition) //replace with BLOCK function
  {
    pros::lcd::set_text(3, ("Iteration - " + std::to_string(iter)));
    pros::delay(100);
  }

  pros::lcd::set_text(4, "Stopping Motors");

  stopMotors();
}

void rotateDriveMotors(double desiredTicks, std::int32_t velocity, bool rfm, bool lfm, bool rbm, bool lbm)
{
  if (rfm)
  {
    rightMF->move_relative(desiredTicks, velocity);
  }
  if (lfm)
  {
    leftMF->move_relative(desiredTicks, velocity);
  }
  if (rbm)
  {
    rightMB->move_relative(desiredTicks, velocity);
  }
  if (lbm)
  {
    leftMB->move_relative(desiredTicks, velocity);
  }
}

float getPosition(bool average) //returns encoder ticks
{
  double t = 0;

  for (int i = 0; i < 4; i++)
  {
    t += std::abs(driveMotors[i]->get_position());
  }

  if (average)
    return static_cast<float>(t) / 4.0f;
  else
    return static_cast<float>(t);
}

void resetMotorRotations(void)
{
  for (const pros::Motor* motor : driveMotors)
  {
    motor->set_zero_position(motor->get_position());
  }
}

void stopMotors(bool rfm, bool lfm, bool rbm, bool lbm) //does this work?
{
  const int32_t zero = static_cast<int32_t>(0);

  if (rfm)
    rightMF->move_voltage(zero);
  if (lfm)
    leftMF->move_voltage(zero);
  if (rbm)
    rightMB->move_voltage(zero);
  if (lbm)
    leftMB->move_voltage(zero);
}
