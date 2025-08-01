#include "main.h"

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftDrive({11, 12, -13});
pros::MotorGroup rightDrive({18, -19, -20});
//pros::MotorGroup leftDrive({1});
//pros::MotorGroup rightDrive({-11});

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
	pros::lcd::set_text(1, "evan wang is #1 brat na!");

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
void autonomous() {
    leftDrive.move(100);
    rightDrive.move(100);
    pros::delay(2000);
    leftDrive.move(0);
    rightDrive.move(0);
}

void opcontrol() {
	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A) && controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
        {
            autonomous();
        }
        int vertical = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y); // Y axis of the left joystick
        int horizontal = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // X axis of the right joystick
        int leftSpeed = vertical - horizontal; // Calculate left motor speed 
        int rightSpeed = vertical + horizontal; // Calculate right motor speed

        /*int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
        int forward = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) ? 127 : 0;
        int backward = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) ? -127 : 0;
        int drive = forward + backward;
        int leftSpeed = drive + turn;
        int rightSpeed = drive - turn;*/

        leftDrive.move(leftSpeed);
        rightDrive.move(rightSpeed);

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
            leftDrive.move(300);
            rightDrive.move(-300);
        }
        pros::delay(15);
    }
}