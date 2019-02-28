#include "main.h"
#include "components.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void checkButtons(void* params)
{
  while (true)
  {
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
    {

    }

		pros::delay(2);
  }
}

void opcontrol()
{
	pros::Task checkButtonsTask(checkButtons);

	while (true)
  {
    float power = controller.get_analog(ANALOG_LEFT_Y);
    float turn = controller.get_analog(ANALOG_RIGHT_X);

    lMF.move_velocity(power + turn);
    lMB.move_velocity(power + turn);
    rMF.move_velocity(power - turn);
    rMB.move_velocity(power - turn);
  }
}

/*void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);
	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		left_mtr = left;
		right_mtr = right;
		pros::delay(20);
	}*/
