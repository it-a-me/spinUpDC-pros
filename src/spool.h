#ifndef SPOOL_INCLUDED
#define SPOOL_INCLUDED

#include "drivetrain.h"

class Spool
{
  public:
    enum SpoolState
    {
        Stopped,
        Unwinding,
        Winding,
    };

  private:
    Drivetrain* drive;
    SpoolState state = Stopped;
    Motor* spool;
    int velocity;

  public:
    Spool(Motor* spool_, int velocity_, Drivetrain* drive_)
      : drive(drive_)
      , spool(spool_)
      , velocity(velocity_)
    {
        spool->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        spool->set_gearing(pros::E_MOTOR_GEAR_600);
    }
    ~Spool() { delete spool; }

    void update(bool unwind, bool wind)
    {
        if (unwind) {
            state = Unwinding;
            spool->move_velocity(velocity * 600 / 100);
        } else if (wind) {
            state = Winding;
            spool->move_velocity(-velocity / 2 * 600 / 100);
        } else if (state == Unwinding) {
            spool->move_velocity(velocity * 600 / 100);
        } else {
            state = Stopped;
            spool->brake();
        }
        drive->unspooling = state == Unwinding;
    }
};
#endif
