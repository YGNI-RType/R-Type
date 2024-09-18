#include "client.hpp"
#include "shared.hpp"
#include <iostream>

typedef struct s_global {
    float time;
} globalvar_t;

globalvar_t global;

void proc_input() {}

int main(void) {
    proc_input();

//    Sample clock to find start time
//     Sample user input (mouse, keyboard, joystick)
//     Package up and send movement command using simulation time
//     Read any packets from the server from the network system
//     Use packets to determine visible objects and their state
//     Render Scene
//     Sample clock to find end time
//     End time minus start time is the simulation time for the next frame
    return 0;
}
