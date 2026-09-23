#include "main.h"
#include <cmath>

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous() {}

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
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER); // Makes the controller into an object called master, I think. This was automatically generated when I made the project.
	pros::Controller partner(pros::E_CONTROLLER_PARTNER); // Make a controller obkect incase of a partner controller

	signed char portFrontMotor = -2; // All of these need to be changed later to add the actual ports
	signed char portBackMotor = -1; // Integer means port, polarity means forward(+)/reversed(-)
	signed char starFrontMotor = 10;
	signed char starBackMotor = 11;
	signed char intakeMotor = 20;
	signed char portCascadeMotor = 19;
	signed char starCascadeMotor = 18;
	signed char clawMotor;

	pros::MotorGroup portMG({portFrontMotor, portBackMotor}); // Creates the motor group for the port side
	pros::MotorGroup starMG({starFrontMotor, starBackMotor}); // Creates the motor group for the star side
	pros::Motor intake({intakeMotor});	// Creates the motor for the pin intake
	pros::MotorGroup cascade({portCascadeMotor, starCascadeMotor}); // Creates the motor group for the cascade lift
	pros::Motor claw({clawMotor}); // Creates the motor for the claw

	int port; // Init the variables used during the while loop
	int star;
	bool done = false;
	int dir;
	int pivot;
	int L1;
	int L2;
	int R1;
	int R2;
	int UP;
	int DOWN;


	// Settings 
	int intakeVelocity = 127;
	int intakeReverseVelocity = -127;
	int clawUpVelocity = 10;
	int clawDownVelocity = -10;
	int cascadeUpVelocity = 50;
	int cascadeDownVelocity = -50;
	


	while (!done) { // Running loop, the code in these brackets will loop until the game ends
		dir = master.get_analog(ANALOG_LEFT_Y); // Gets the forward/back from the left joystick
		pivot = master.get_analog(ANALOG_RIGHT_X); //gets the left/right from the right joystick
		L1 = master.get_digital(DIGITAL_L1);
		L2 = master.get_digital(DIGITAL_L2);
		R1 = master.get_digital(DIGITAL_R1);
		R2 = master.get_digital(DIGITAL_R2);
		UP = master.get_digital(DIGITAL_UP);
		DOWN = master.get_digital(DIGITAL_DOWN);


		if (L1) {
			intake.move(intakeVelocity);
		} else if (L2) {
			intake.move(intakeReverseVelocity);
		}

		if (R1) {
			claw.move(clawUpVelocity);
		} else if (R2) {
			claw.move(clawDownVelocity);
		} else {
			claw.brake();
		}

		if (DOWN) {
			cascade.move(cascadeDownVelocity);
		} else if (UP) {
			cascade.move(cascadeUpVelocity);
		} else {
			cascade.brake();
		}


		port = dir + pivot; // the math for driving/turning the drive train.
		star = dir - pivot;

		// pivot = std::sqrt(pivot); // These two lines apply the square root curve to the amount
		// pivot = pivot * 10;       // of pivot, which makes it feel more natural to the driver.

		/* I use port and starboard (or star for short) when refering to the sides,
		because left and right can be ambiguous and confusing*/

		portMG.move(port); // Powers the port motors, minus any amount of turning
		starMG.move(star); // Powers the starboard motors, sum any amount of turning

		pros::delay(20); // This marks the polling rate, this programs polls user input every 20ms

	}
}