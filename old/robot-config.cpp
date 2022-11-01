#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor FRONT_LEFT = motor(PORT9, ratio18_1, false);
motor FRONT_RIGHT = motor(PORT10, ratio18_1, true);
motor BACK_LEFT = motor(PORT19, ratio18_1, false);
motor BACK_RIGHT = motor(PORT18, ratio18_1, true);
motor ROLLER_SPINNER = motor(PORT12, ratio36_1, false);
motor SPOOL = motor(PORT14, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}