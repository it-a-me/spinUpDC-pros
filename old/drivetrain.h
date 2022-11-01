#include "vex.h"
using namespace vex;

// manual drivetrain implementation
class Drivetrain {
public:
  // abstraction towards direction, used as a paramenter when moving
  // horizontally
  enum HorizontalDir {
    Left,
    Right,
  };

protected:
  // array of the 4 motors in the drivetrain
  motor *motors[4];
  // array of motor velocities
  int motor_velocities[4];
  // speed of horizontal movement
  int horizontalVelocity = 70;
  // if we are in precision slower speed or max speed
  bool slowed = false;

  // adds sideways motion to current motor speeds
  void horizontal(HorizontalDir dir) {
    if (dir == HorizontalDir::Left) {
      motor_velocities[front_left] += -horizontalVelocity;
      motor_velocities[front_right] += horizontalVelocity;
      motor_velocities[back_left] += horizontalVelocity;
      motor_velocities[back_right] += -horizontalVelocity;

    } else if (dir == HorizontalDir::Right) {
      motor_velocities[front_left] += horizontalVelocity;
      motor_velocities[front_right] += -horizontalVelocity;
      motor_velocities[back_left] += -horizontalVelocity;
      motor_velocities[back_right] += horizontalVelocity;
    }
  }

  // resets motor velocities to 0 to ensure that they get reset between frames
  void reset_velocities() {
    for (int i = 0; i < 4; i++) {
      motor_velocities[i] = 0;
    }
  }

public:
  bool unspooling = false;
  // allows you to reference motors by position on robot rather than index in
  // int array
  enum MotorPosition {
    front_left,
    front_right,
    back_left,
    back_right,
  };

  // drivetrain constructor to ensure no null references
  Drivetrain(motor *fl, motor *fr, motor *bl, motor *br,
             brakeType stoppingMode) {

    motors[front_left] = fl;
    motors[front_right] = fr;
    motors[back_left] = bl;
    motors[back_right] = br;
    reset_velocities();
    setStoppingMode(stoppingMode);
  };

  // tell motors to act on current velocities
  void drive() {
    for (int i = 0; i < 4; i++) {
      // reduce speed in slow mode
      if (unspooling) {
        motors[i]->setVelocity(motor_velocities[i] * 0.3, percent);
      } else if (slowed) {
        motors[i]->setVelocity(motor_velocities[i] * 0.5, percent);
      } else {
        motors[i]->setVelocity(motor_velocities[i], percent);
      }
      motors[i]->spin(forward);
    }
    // reset velocities to prevent carry over between frames
    reset_velocities();
  }
  // limits access to private fields
  int getMotorVelocity(MotorPosition m) { return motor_velocities[m]; }

  void setMotorVelocity(MotorPosition m, int vel) { motor_velocities[m] = vel; }

  // sets stoping mode for all motors at once
  void setStoppingMode(brakeType mode) {
    for (motor *m : motors) {
      m->setStopping(mode);
    }
  }

  // stops all drivetrain motors
  void stop() {
    for (motor *m : motors) {
      m->stop();
    }
  }

  // toggle speed
  void ToggleSlow() {
    slowed = !slowed;
    // return slowed;
  }

  void userControl(int LeftStick, int RightStick, bool left, bool right) {
    // mix horizontal and stick movement
    if ((LeftStick != 0 || RightStick != 0) && (left || right)) {
      motor_velocities[back_left] = LeftStick;
      motor_velocities[front_left] = LeftStick;
      motor_velocities[front_right] = RightStick;
      motor_velocities[back_right] = RightStick;
      if (left) {
        horizontal(HorizontalDir::Left);
      } else if (right) {
        horizontal(HorizontalDir::Right);
      }
      for (int i = 0; i < 4; i++) {
        motor_velocities[i] *= 0.5;
      }
      // stick movement
    } else if (LeftStick != 0 || RightStick != 0) {
      motor_velocities[back_left] = LeftStick;
      motor_velocities[front_left] = LeftStick;
      motor_velocities[front_right] = RightStick;
      motor_velocities[back_right] = RightStick;
      // horiziontal movement
    } else if (left) {
      horizontal(HorizontalDir::Left);
    } else if (right) {
      horizontal(HorizontalDir::Right);
    }
  }
  // drive for a set time
  void AutonomousDrive(float left, float right, double t) {
    motor_velocities[back_left] = left;
    motor_velocities[front_left] = left;
    motor_velocities[front_right] = right;
    motor_velocities[back_right] = right;

    this->drive();
    wait(t*1000, msec);
    this->stop();
  }
  // drive horizontally for a set time
  void AutonomousHorizontal(HorizontalDir dir, double t) {
    this->horizontal(dir);
    this->drive();
    wait(t, sec);
    this->stop();
  }
};