#ifndef DRIVETRAIN_INCLUDED
#define DRIVETRAIN_INCLUDED
#include <array>
using std::array;
#include <memory>
#include <string>

#include "pros/motors.hpp"
using pros::Motor;

#include "main.h"
// manual drivetrain implementation
class Drivetrain
{
  public:
    // abstraction towards direction, used as a paramenter when moving
    // horizontally
    enum HorizontalDir
    {
        Left,
        Right,
    };

  protected:
    // array of the 4 motors in the drivetrain
    array<pros::Motor*, 4> motors;
    // array of motor velocities
    array<int, 4> motor_velocities = { 0, 0, 0, 0 };
    // speed of horizontal movement
    int horizontalVelocity = 100;
    // if we are in precision slower speed or max speed
    bool slowed = false;

    // adds sideways motion to current motor speeds
    void horizontal(HorizontalDir dir)
    {
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
    void reset_velocities()
    {
        for (int& velocity : motor_velocities) {
            velocity = 0;
        }
    }

  public:
    bool unspooling = false;
    // allows you to reference motors by position on robot rather than index in
    // int array
    enum MotorPosition
    {
        front_left,
        front_right,
        back_left,
        back_right,
    };
    // drivetrain constructor to ensure no null references
    Drivetrain(Motor* front_left_,
               Motor* front_right_,
               Motor* back_left_,
               Motor* back_right_,
               pros::motor_brake_mode_e stoppingMode)
      : motors({ front_left_, front_right_, back_left_, back_right_ })
    {
        reset_velocities();
        setStoppingMode(stoppingMode);
    };
    ~Drivetrain()
    {
        for (Motor* m : motors) {
            delete m;
        }
    }

    // tell motors to act on current velocities
    void drive()
    {
        for (int i = 0; i < 4; i++) {
            // reduce speed in slow mode
            if (unspooling) {
                motors[i]->move((motor_velocities[i] * 0.4) * 127 / 100);
            } else if (slowed) {
                motors[i]->move((motor_velocities[i] * 0.5) * 127 / 100);
            } else {
                motors[i]->move(motor_velocities[i] * 127 / 100);
            }
        }

        // reset velocities to prevent carry over between frames
        reset_velocities();
    }
    // limits access to private fields
    int getMotorVelocity(MotorPosition pos) { return motor_velocities[pos]; }

    void setMotorVelocity(MotorPosition pos, int vel)
    {
        motor_velocities[pos] = vel;
    }

    // sets stoping mode for all motors at once
    void setStoppingMode(pros::motor_brake_mode_e mode)
    {
        for (Motor* motor : motors) {
            motor->set_brake_mode(mode);
        }
    }

    // stops all drivetrain motors
    void stop()
    {
        for (Motor* motor : motors) {
            motor->brake();
        }
    }

    // toggle speed
    void ToggleSlow()
    {
        slowed = !slowed;
        // return slowed;
    }

    void userControl(int LeftStick, int RightStick, bool left, bool right)
    {
        LeftStick = LeftStick * 100 / 127;
        RightStick = RightStick * 100 / 127;
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
            for (int& velocity : motor_velocities) {
                velocity /= 2;
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
    void AutonomousDrive(int left, int right, double seconds)
    {
        motor_velocities[back_left] = left;
        motor_velocities[front_left] = left;
        motor_velocities[front_right] = right;
        motor_velocities[back_right] = right;

        this->drive();
        pros::delay(seconds * float(1000));
        this->stop();
    }
    // drive horizontally for a set time
    void AutonomousHorizontal(HorizontalDir dir, double seconds)
    {
        this->horizontal(dir);
        this->drive();
        pros::delay(seconds * 1000);
        this->stop();
    }
};

#endif
