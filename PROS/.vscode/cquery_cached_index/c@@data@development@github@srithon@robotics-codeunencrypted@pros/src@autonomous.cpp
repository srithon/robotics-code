#include "main.h"
#include "components.h"

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
void stopMotors(bool = 1, bool = 1, bool = 1, bool = 1);
//use the explicit keyword to prevent conversions of float types?

void autonomous()
{
  moveBot(12.0, static_cast<int32_t>(50), true);
  pros::delay(1000);
  pidTurn(90.0f);
}

void pidTurn(float deg)
{
  float arcLength = WHEEL_CIRCUMFERENCE * (deg / 360.0f);
  float rot = 360.0f*(arcLength/(4.0f*PI));

	float desiredDegs = rot;
	float currentDegs = 0;

	float lastError = 0;
	float error = 0;
	float errorSum = 0;

	float rateErrorChange;

	std::int32_t motorSpinVelocity;

  resetMotorRotations();

	while (currentDegs < desiredDegs)
	{
		currentDegs = getPosition(true); // this or averageEncoderValue() - not sure

		//proportional
		error = (desiredDegs - currentDegs);

		//integral
		if (errorSum < integralMax)
			errorSum += error;
		else
			errorSum = integralMax;

		//derivative
		rateErrorChange = (error - lastError);// / 16;//(getChangeInTime());

		motorSpinVelocity = static_cast<std::int32_t>((error * kP) + (errorSum * kI) + (rateErrorChange * kD));

    rightMF->move_velocity(motorSpinVelocity);
    rightMB->move_velocity(motorSpinVelocity);
    leftMF->move_velocity(motorSpinVelocity); //check sign
    leftMB->move_velocity(motorSpinVelocity); //check sign

		lastError = error;

		pros::delay(2);
	}
}

void moveBot(double inches, int32_t velocity, bool direction)
{
  double deg = inches / (360.0 * static_cast<double>(WHEEL_CIRCUMFERENCE));

  if (!direction)
    deg = -deg;

  //double averagePosition = getAveragePosition();
  double lMBEndingPosition = leftMB->get_position() + deg;
  rotateDriveMotors(deg, velocity);

  //while (getAveragePosition() < averagePosition + encoderTicks)
  while (leftMB->get_position() < lMBEndingPosition)
  {
    pros::delay(20);
  }

  stopMotors();
}

void rotateDriveMotors(double deg, std::int32_t velocity, bool rfm, bool lfm, bool rbm, bool lbm)
{
  if (rfm)
  {
    rightMF->move_relative(deg, velocity);
  }
  if (lfm)
  {
    leftMF->move_relative(deg, velocity);
  }
  if (rbm)
  {
    rightMB->move_relative(deg, velocity);
  }
  if (lbm)
  {
    leftMB->move_relative(deg, velocity);
  }
}

float getPosition(bool average)
{
  double t = 0;

  for (int i = 0; i < 4; i++)
  {
    t += driveMotors[i]->get_position();
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
