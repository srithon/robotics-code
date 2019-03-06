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
void rotateDriveMotors(double, std::int32_t, bool = 1, bool = 1, bool = 1, bool = 1);
void resetMotorRotations(void);
inline float getEncoderEq(float);

auto chassis = okapi::ChassisControllerFactory::create(LEFT_MOTOR_FRONT_PORT, RIGHT_MOTOR_FRONT_PORT);

void autonomous()
{
  chassis.moveDistance(getEncoderEq(12.0f));
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

inline float getEncoderEq(float inches)
{
  return inches / WHEEL_CIRCUMFERENCE * ENCODER_TICKS_PER_WHEEL_ROTATION;
}
