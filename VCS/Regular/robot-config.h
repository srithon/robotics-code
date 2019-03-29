using namespace vex;

/**************************************************/
/*Motor Port Declaration Panel*/

const int FRONT_RIGHT_PORT = PORT10;
const int BACK_RIGHT_PORT = PORT19;
const int FRONT_LEFT_PORT = PORT2;
const int BACK_LEFT_PORT = PORT11;

const int ROLLER_PORT = PORT7;
const int ROLLER2_PORT = PORT18;

const int LAUNCHER_PORT = PORT16;

/***************************************************/

/**************************************************/
/*Initializing Panel*/

brain Brain = brain();

//eController Controller1 = eController();
controller Controller1 = controller();

competition comp = competition();

motor rightMotorF = motor(FRONT_RIGHT_PORT, false);
motor rightMotorB = motor(BACK_RIGHT_PORT, true);
motor leftMotorF = motor(FRONT_LEFT_PORT, true);
motor leftMotorB = motor(BACK_LEFT_PORT, false);

motor rollerMotor = motor(ROLLER_PORT, true);
motor rollerMotor2 = motor(ROLLER2_PORT, false);

motor launcherMotor = motor(LAUNCHER_PORT, true);
/**************************************************/

controller::axis* VERTICAL_AXIS = &Controller1.Axis3;
controller::axis* HORIZONTAL_AXIS = &Controller1.Axis1;

controller::button* SLOW_LEFT = &Controller1.ButtonLeft;
controller::button* SLOW_RIGHT = &Controller1.ButtonRight;
controller::button* SLOW_UP = &Controller1.ButtonUp;

controller::button* INTAKE_IN = &Controller1.ButtonL2;
controller::button* INTAKE_OUT = &Controller1.ButtonR2;

controller::button* LAUNCH_BUTTON = &Controller1.ButtonR1;
