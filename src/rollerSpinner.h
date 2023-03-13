#ifndef ROLLERSPINNER_INCLUDED
#define ROLLERSPINNER_INCLUDED
#include "drivetrain.h"
#include "ports.h"

class RollerSpinner
{
    // pointers to drivetrain and roller spinning motor
  private:
    Drivetrain* drive;
    pros::Motor spinner = pros::Motor(ROLLER_SPINNER);
    int velocity;

    // constructor of roller spinner class
  public:
    RollerSpinner(int velocity_, Drivetrain* drive_)
      : velocity(velocity_)
      , drive(drive_)
    {
        spinner.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    }

    // spins the wheel either forward or reverse.  If the wheel is spining also
    // slightly drive backwards to apply pressure on the wheel.
    void update(bool fwd, bool rev)
    {
        if (fwd || rev) {
            drive->setMotorVelocity(Drivetrain::MotorPosition::front_left, -32);
            drive->setMotorVelocity(Drivetrain::MotorPosition::front_right, -8);
            drive->setMotorVelocity(Drivetrain::MotorPosition::back_right, -8);
            drive->setMotorVelocity(Drivetrain::MotorPosition::back_left, -32);
            if (fwd) {
                spinner.move(velocity * 127 / 100);
            } else if (rev) {
                spinner.move(-velocity * 127 / 100);
            }
        } else {
            spinner.brake();
        }
    }
};
#endif
