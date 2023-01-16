#ifndef DRIVETRAIN_INCLUDED
#define DRIVETRAIN_INCLUDED
#include "drivetrain.h"
#endif
using namespace vex;

class RollerSpinner {
  //pointers to drivetrain and roller spinning motor
private:
  Drivetrain *drive;
  motor *spinner;

//constructor of roller spinner class
public:
  RollerSpinner(motor *spinner_motor, double velocity, Drivetrain *drivetrain) {
    spinner = spinner_motor;
    spinner->setVelocity(velocity, percent);
    spinner->setStopping(coast);
    drive = drivetrain;
  }

//spins the wheel either forward or reverse.  If the wheel is spining also slightly drive backwards to apply pressure on the wheel.
  void update(bool fwd, bool rev) {
    if (fwd || rev) {
      drive->setMotorVelocity(Drivetrain::MotorPosition::front_left, -16);
      drive->setMotorVelocity(Drivetrain::MotorPosition::front_right, -4);
      drive->setMotorVelocity(Drivetrain::MotorPosition::back_right, -4);
      drive->setMotorVelocity(Drivetrain::MotorPosition::back_left, -16);
      // dr->fr = -2;
      // dr->br = -2;
      // dr->fl = -8;
      // dr->bl = -8;
      if (fwd) {
        spinner->spin(forward);
      } else if (rev) {
        spinner->spin(reverse);
      }
    } else {
      spinner->stop();
    }
  }
};