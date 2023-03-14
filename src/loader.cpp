#include "loader.h"

void
Loader::load()
{
    if (loader_state == loader_state::stopped) {
        set_state(loader_state::loading);
    } else if (loader_state == loader_state::loading) {
        set_state(loader_state::unloading);
    }
}

void
Loader::set_state(enum loader_state state)
{
    loader_state = state;
    if (state == loader_state::stopped) {
        this->brake();
    } else if (state == loader_state::loading) {
        this->move_velocity(loader_speed);
    } else if (state == loader_state::unloading) {
        this->move_velocity(-loader_speed);
    }
}
void
Loader::update()
{
    if (loader_state == loader_state::stopped) {
        this->brake();
    } else if (this->at_target()) {
        if (loader_state == loader_state::loading) {
            set_state(loader_state::unloading);
        } else if (loader_state == loader_state::unloading) {
            set_state(loader_state::stopped);
        }
    }
}
