#pragma once
#include "drivetrain.h"
#include "ports.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include "rollerSpinner.h"
#include "shooter.h"
#include "tasks.h"

using brake_mode = pros::motor_brake_mode_e_t;
class Robot
{
  protected:
    pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
    Drivetrain drive = Drivetrain(brake_mode::E_MOTOR_BRAKE_COAST);
    RollerSpinner roller_spinner = RollerSpinner(&drive);
    Shooter shooter = Shooter();
    void primary_mode()
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
    void alt_mode()
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

  public:
    void Usercontrol()
    {
        pros::Task loader_t(tasks::loader_task, (void*)&master, "loader task");
        while (true) {
            // allows user to control robot
            drive.userControl(
              master.get_analog(ANALOG_LEFT_Y),
              master.get_analog(ANALOG_RIGHT_Y),
              master.get_digital(pros::E_CONTROLLER_DIGITAL_L2),
              master.get_digital(pros::E_CONTROLLER_DIGITAL_R2));

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
};
