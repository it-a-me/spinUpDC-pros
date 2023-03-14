#pragma once
#include "main.h"
#include "ports.h"
class Loader : private pros::Motor
{
  private:
    const int loader_speed = 50;
    const int degree_target = 150;
    const int start_position = 0;
    int loader_state = loader_state::stopped;

    enum loader_state
    {
        loading,
        unloading,
        stopped,
    };

  public:
    bool at_target()
    {
        return (loader_state == loader_state::loading &&
                this->get_position() > degree_target) ||
               (loader_state == loader_state::unloading &&
                this->get_position() < start_position);
    }
    Loader()
      : pros::Motor(LOADER)
    {
        this->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
        this->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    };

    void load();
    void update();
    void up()
    {
        loader_state = loader_state::stopped;
        this->tare_position();
        this->move_velocity(loader_speed);
    }
    void set_state(enum loader_state state);

    void down()
    {
        loader_state = loader_state::stopped;
        this->tare_position();
        this->move_velocity(-loader_speed);
    }
    using pros::Motor::brake;
    using pros::Motor::move_absolute;
};
