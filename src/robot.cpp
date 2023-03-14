#include "robot.h"

void
Robot::primary_mode()
{
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        roller_spinner.spin_forward();
    } else {
        roller_spinner.brake();
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        shooter.shoot();
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        shooter.load();
    } else {
        shooter.brake();
    }
}
void
Robot::alt_mode()
{
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        roller_spinner.spin_reverse();
    } else {
        roller_spinner.brake();
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) ||
        master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        shooter.load();
    } else {
        shooter.brake();
    }
}

void
Robot::Usercontrol()
{
    while (true) {
        // allows user to control robot
        drive.userControl(master.get_analog(ANALOG_LEFT_Y),
                          master.get_analog(ANALOG_RIGHT_Y),
                          master.get_digital(pros::E_CONTROLLER_DIGITAL_L2),
                          master.get_digital(pros::E_CONTROLLER_DIGITAL_R2));

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
            loader.up();
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
            loader.down();
        } else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            loader.load();
        } else {
            loader.update();
        }

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            alt_mode();
        } else {
            primary_mode();
        }
        // drivetrain motors take effect
        drive.drive();
        pros::delay(10);
    }
}
