#pragma once
#include "main.h"
#include "ports.h"
#define LOAD_SPEED -10
#define SHOOT_SPEED 140
class Shooter : protected pros::Motor_Group
{
  public:
    Shooter()
      : pros::Motor_Group({ SHOOTER_LEFT, SHOOTER_RIGHT })
    {
        this->set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    }
    void load() { this->move_velocity(LOAD_SPEED); }
    void shoot() { this->move_velocity(SHOOT_SPEED); }
    using pros::Motor_Group::brake;
    using pros::Motor_Group::move_velocity;
};
