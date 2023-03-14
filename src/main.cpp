#include "main.h"
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"

#include "drivetrain.h"
#include "ports.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include "robot.h"
#include "rollerSpinner.h"
#include "shooter.h"
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
    pros::lcd::set_text(1, "Hello Decln");
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
    // RollerSpinner roller_spinner =
    //   RollerSpinner(new pros::Motor(12), 40, &drive);
    auto drive = Drivetrain(pros::E_MOTOR_BRAKE_COAST);

    drive.userControl(-40, -30, false, false);
    auto roller_spinner = pros::Motor(ROLLER_SPINNER);
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
    Robot().Usercontrol();
}
