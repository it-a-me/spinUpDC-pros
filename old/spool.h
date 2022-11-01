#ifndef DRIVETRAIN_INCLUDED
#define DRIVETRAIN_INCLUDED
#include "drivetrain.h"
#endif
#include "vex.h"

using namespace vex;

class Spool {
public:
  enum SpoolState {
    Stopped,
    Unwinding,
    Winding,
  };

private:
  Drivetrain *drive;
  SpoolState state = Stopped;
  motor *spool;

public:
  Spool(motor *spool_motor, double velocity, Drivetrain *drivetrain) {
    drive = drivetrain;
    spool = spool_motor;
    spool->setVelocity(velocity, percent);
  }

  void update(bool unwind, bool wind) {
    if (unwind) {
      state = Unwinding;
      spool->spin(forward);
    } else if (wind) {
      state = Winding;
      spool->spin(reverse);
    } else if (state == Unwinding) {
      spool->spin(forward);
    } else {
      state = Stopped;
      spool->stop();
    }
    drive->unspooling = state == Unwinding;
  }
  // void wind(bool wind) {
  //   if (wind) {
  //     spinning = Spooling;
  //     SPOOL.spin(reverse);
  //   } else if (spinning == Spooling) {
  //     spinning = Stopped;
  //   }
  // }

  // void update() {
  //   if (spinning == Unspooling) {
  //     SPOOL.spin(dir);
  //     d->fr = d->fr / 3;
  //     d->br = d->br / 3;
  //     d->fl = d->fl / 3;
  //     d->bl = d->bl / 3;
  //   } else if (spinning == Stopped) {
  //     SPOOL.stop();
  //   }
  // }
};