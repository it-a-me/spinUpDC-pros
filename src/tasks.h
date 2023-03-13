#pragma once
#include "main.h"
#include "ports.h"
namespace tasks {
void
loader_task(void* args)
{
    auto* master = (pros::Controller*)args;
    const int loader_speed = 50;
    auto loader = pros::Motor(LOADER);
    loader.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    loader.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    loader.tare_position();
    while (true) {
        if (master->get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
            loader.move_velocity(loader_speed);
            pros::delay(20);
            loader.tare_position();
        } else if (master->get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
            loader.move_velocity(-loader_speed);
            pros::delay(20);
            loader.tare_position();
        } else if (master->get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            while (loader.get_position() < 150) {
                loader.move_velocity(loader_speed);
                pros::delay(20);
                if (master->get_digital(pros::E_CONTROLLER_DIGITAL_UP) ||
                    master->get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) ||
                    master->get_digital_new_press(
                      pros::E_CONTROLLER_DIGITAL_A)) {
                    break;
                }
            }
            while (loader.get_position() > 0) {
                loader.move_velocity(-loader_speed);
                pros::delay(20);
                if (master->get_digital(pros::E_CONTROLLER_DIGITAL_UP) ||
                    master->get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
                    break;
                }
            }
            loader.brake();
            pros::delay(20);

            // auto piston = pros::ADIDigitalOut(PNEUMATICS);
            // piston.set_value(true);
            // pros::delay(1000);
            // piston.set_value(false);
        } else {
            loader.brake();
            pros::delay(20);
        }
    }
    // ...
}

}
