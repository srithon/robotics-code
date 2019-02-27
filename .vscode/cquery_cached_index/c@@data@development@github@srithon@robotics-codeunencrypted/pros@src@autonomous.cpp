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
void moveBot(const int, const double, const bool);
void pidTurn(const float);
void rotateDriveMotors(const double, const std::int32_t, const bool, const bool, const bool, const bool);
double getPosition(const bool);
void resetMotorRotations(void);

void autonomous()
{
  moveBot(12, 50, true);
  pros::delay(1000);
  pidTurn(90);
}

void pidTurn(const float deg)
{
  double arcLength = WHEEL_CIRCUMFERENCE * (deg / 360.0);
  double rot = 360.0*(arcLength/(4.0*PI));

	float desiredDegs = rot;
	float currentDegs = 0;

	float lastError = 0;
	float error = 0;
	float errorSum = 0;

	float rateErrorChange;
	float motorSpinVelocity;

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

		motorSpinVelocity = (error * kP) + (errorSum * kI) + (rateErrorChange * kD);

    rMF.move_velocity(motorSpinVelocity);
    rMB.move_velocity(motorSpinVelocity);
    lMF.move_velocity(-motorSpinVelocity);
    lMB.move_velocity(-motorSpinVelocity);

		lastError = error;

		pros::delay(2);
	}
}

void moveBot(const int inches, const double velocity, const bool direction)
{
  double deg = inches / (360 * WHEEL_CIRCUMFERENCE);

  if (!direction)
    deg = -deg;

  //double averagePosition = getAveragePosition();
  double lMBaveragePosition = lMB.get_position();
  rotateDriveMotors(deg, velocity, true, true, true, true);

  //while (getAveragePosition() < averagePosition + encoderTicks)
  while (lMB.get_position() < lMBaveragePosition)
  {
    pros::delay(20);
  }
}

void rotateDriveMotors(const double deg, const std::int32_t velocity, const bool rfm, const bool lfm, const bool rbm, const bool lbm)
{
  if (rfm)
  {
    rMF.move_relative(deg, velocity);
  }
  if (lfm)
  {
    lMF.move_relative(deg, velocity);
  }
  if (rbm)
  {
    rMB.move_relative(deg, velocity);
  }
  if (lbm)
  {
    lMB.move_relative(deg, velocity);
  }
}

double getPosition(bool average)
{
  double t = 0;

  for (int i = 0; i < 4; i++)
  {
    t += driveMotors[i]->get_position();
  }

  if (average)
    return t / 4.0;
  else
    return t;
}

void resetMotorRotations(void)
{
  for (pros::Motor* motor : driveMotors)
  {
    motor->set_zero_position(motor->get_position());
  }
}
