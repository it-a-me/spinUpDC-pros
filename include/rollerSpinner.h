#pragma once
#include "drivetrain.h"
#include "ports.h"

class RollerSpinner : private pros::Motor
{
    // pointers to drivetrain and roller spinning motor
  private:
    const int velocity = 100;
    Drivetrain* drive;

    // constructor of roller spinner class
  public:
    explicit RollerSpinner(Drivetrain* drive_)
      : pros::Motor(ROLLER_SPINNER)
      , drive(drive_)
    {
        this->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    }
    void spin_forward()
    {
        this->move(velocity);
        drive->roller_reverse();
    };
    void spin_reverse()
    {
        this->move(-velocity);
        drive->roller_reverse();
    }
    using pros::Motor::brake;
};
