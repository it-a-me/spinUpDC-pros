#include "main.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"

#include "drivetrain.h"
#include "rollerSpinner.h"
#include "spool.h"
#include "vision.h"
#define FRONT_LEFT -9
#define FRONT_RIGHT 10
#define BACK_LEFT -19
#define BACK_RIGHT 18
// new pros::Motor(-9),
//                                   new pros::Motor(10),
//                                   new pros::Motor(-19),
//                                   new pros::Motor(18),

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void
initialize()
{
    pros::lcd::initialize();
    pros::lcd::set_text(1, "Hello Declan");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void
disabled()
{
}
/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void
competition_initialize()
{
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start
 * it from where it left off.
 */
void
dumpy(Drivetrain* drive)
{
    drive->AutonomousDrive(100, 100, 2);
    drive->AutonomousDrive(-100, -100, 1.75);
}
void
autonomous()
{
    // Drivetrain drive = Drivetrain(new pros::Motor(9),
    //                               new pros::Motor(-10),
    //                               new pros::Motor(19),
    //                               new pros::Motor(-18),
    //                               pros::E_MOTOR_BRAKE_COAST);

    // dumpy(&drive);
    // drive.stop();
    // RollerSpinner roller_spinner =
    //   RollerSpinner(new pros::Motor(12), 40, &drive);
    Drivetrain drive = Drivetrain(new pros::Motor(FRONT_LEFT),
                                  new pros::Motor(FRONT_RIGHT),
                                  new pros::Motor(BACK_LEFT),
                                  new pros::Motor(BACK_RIGHT),
                                  pros::E_MOTOR_BRAKE_COAST);

    drive.userControl(-40, -30, false,false);
    Motor roller_spinner = Motor(12);
    roller_spinner.set_gearing(pros::E_MOTOR_GEAR_RED);
    drive.drive();
    pros::delay(1000);

    roller_spinner.move_relative(-345, 60);
    pros::delay(2000);
    drive.userControl(70, -70, 0, 0);
    drive.drive();
    pros::delay(1000);
    drive.userControl(100, 100, 0, 0);
    drive.drive();
    pros::delay(3000);
    // drive.stop();
    // drive.userControl(-100, -100, 0, 0);
    // drive.drive();
    // pros::delay(500);
    drive.userControl(-100, -100, 0, 0);
    drive.drive();

    pros::delay(500);

    drive.userControl(0, 0, 0, 0);
    drive.drive();

    

}

/**
 * Runs the operator control code. This function will be started in its own
 * task with the default priority and stack size whenever the robot is enabled
 * via the Field Management System or the VEX Competition Switch in the
 * operator control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart
 * the task, not resume it from where it left off.
 */
void
opcontrol()
{
    VisionSensor vision =
      VisionSensor(new pros::Vision(6, pros::E_VISION_ZERO_TOPLEFT));

    // initialize the controller
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    // initialize the drivetrian
    Drivetrain drive = Drivetrain(new pros::Motor(-9),
                                  new pros::Motor(10),
                                  new pros::Motor(-19),
                                  new pros::Motor(18),
                                  pros::E_MOTOR_BRAKE_COAST);
    // initialize the spool
    Spool spool = Spool(new pros::Motor(14), 44, &drive);

    // initialize the roller spinner
    RollerSpinner roller_spinner =
      RollerSpinner(new pros::Motor(12), 60, &drive);

    while (true) {

        pros::lcd::print(2, "%d", vision.red_on_top());
        // allows user to control robot
        drive.userControl(master.get_analog(ANALOG_LEFT_Y),
                          master.get_analog(ANALOG_RIGHT_Y),
                          master.get_digital(pros::E_CONTROLLER_DIGITAL_L2),
                          master.get_digital(pros::E_CONTROLLER_DIGITAL_R2));

        // start continously unspooling if "a" is pressed
        // spool only while "b" is held down
        spool.update(master.get_digital(pros::E_CONTROLLER_DIGITAL_A),
                     master.get_digital(pros::E_CONTROLLER_DIGITAL_B));
        // clockwise "L2" counterclockwise "R2"
        roller_spinner.update(
          master.get_digital(pros::E_CONTROLLER_DIGITAL_L1),
          master.get_digital(pros::E_CONTROLLER_DIGITAL_R1));
        // drivetrain motors take effect
        drive.drive();
        pros::delay(20);
    }
}
