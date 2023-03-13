#ifndef DRIVETRAIN_INCLUDED
#define DRIVETRAIN_INCLUDED
#include <array>
using std::array;
#include <memory>
#include <string>

#include "pros/motors.hpp"

#include "main.h"
#include "ports.h"

class Wheel
{
  public:
    pros::Motor motor;
    int velocity = 0;
    explicit Wheel(std::int8_t port)
      : motor(pros::Motor(port))
    {
    }
};

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
    array<Wheel, 4> wheels = {
        Wheel(FRONT_LEFT),
        Wheel(FRONT_RIGHT),
        Wheel(BACK_LEFT),
        Wheel(BACK_RIGHT),
    };
    // speed of horizontal movement
    int horizontalVelocity = 100;
    // if we are in precision slower speed or max speed
    bool slowed = false;

    // adds sideways motion to current motor speeds
    void horizontal(HorizontalDir dir)
    {
        if (dir == HorizontalDir::Left) {
            wheels[front_left].velocity += -horizontalVelocity;
            wheels[front_right].velocity += horizontalVelocity;
            wheels[back_left].velocity += horizontalVelocity;
            wheels[back_right].velocity += -horizontalVelocity;

        } else if (dir == HorizontalDir::Right) {
            wheels[front_left].velocity += horizontalVelocity;
            wheels[front_right].velocity += -horizontalVelocity;
            wheels[back_left].velocity += -horizontalVelocity;
            wheels[back_right].velocity += horizontalVelocity;
        }
    }

    // resets motor velocities to 0 to ensure that they get reset between frames
    void reset_velocities()
    {
        for (Wheel& wheel : wheels) {
            wheel.velocity = 0;
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
    explicit Drivetrain(pros::motor_brake_mode_e stoppingMode)
    {
        reset_velocities();
        setStoppingMode(stoppingMode);
    };

    // tell motors to act on current velocities
    void drive()
    {
        for (Wheel& wheel : wheels) {
            wheel.motor.move(wheel.velocity * 127 / 100);
        }
        reset_velocities();
    }

    // reset velocities to prevent carry over between frames
    // limits access to private fields
    int getMotorVelocity(MotorPosition pos) { return wheels[pos].velocity; }

    void setMotorVelocity(MotorPosition pos, int vel)
    {
        wheels[pos].velocity = vel;
    }

    // sets stoping mode for all motors at once
    void setStoppingMode(pros::motor_brake_mode_e mode)
    {
        for (Wheel& wheel : wheels) {
            wheel.motor.set_brake_mode(mode);
        }
    }

    // stops all drivetrain motors
    void stop()
    {
        for (Wheel& wheel : wheels) {
            wheel.motor.brake();
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
            wheels[back_left].velocity = LeftStick;
            wheels[front_left].velocity = LeftStick;
            wheels[front_right].velocity = RightStick;
            wheels[back_right].velocity = RightStick;
            if (left) {
                horizontal(HorizontalDir::Left);
            } else if (right) {
                horizontal(HorizontalDir::Right);
            }
            for (Wheel& wheel : wheels) {
                wheel.velocity /= 2;
            }
            // stick movement
        } else if (LeftStick != 0 || RightStick != 0) {
            wheels[back_left].velocity = LeftStick;
            wheels[front_left].velocity = LeftStick;
            wheels[front_right].velocity = RightStick;
            wheels[back_right].velocity = RightStick;
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
        wheels[back_left].velocity = left;
        wheels[front_left].velocity = left;
        wheels[front_right].velocity = right;
        wheels[back_right].velocity = right;

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
