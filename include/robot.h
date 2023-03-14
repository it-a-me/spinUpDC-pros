#pragma once
#include "drivetrain.h"
#include "loader.h"
#include "ports.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include "rollerSpinner.h"
#include "shooter.h"

using brake_mode = pros::motor_brake_mode_e_t;
class Robot
{
  public:
    pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
    Drivetrain drive = Drivetrain(brake_mode::E_MOTOR_BRAKE_COAST);
    RollerSpinner roller_spinner = RollerSpinner(&drive);
    Shooter shooter = Shooter();
    Loader loader = Loader();

  protected:
    void primary_mode();
    void alt_mode();

  public:
    void Usercontrol();
};
