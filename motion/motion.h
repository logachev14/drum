#pragma once
#include <stdint.h>
#include "project_config.h"
STRONG_ENUM(Command,
            NO_COMMAND = 0,
            SET_NEW_POSITION = 1,
            HOME_AXIS = 2,
            CHANGE_TOOL = 3,
            SET_CHAMBER_TEMP = 4,
            SET_WAIT_CHAMBER_TEMP = 5,
            SET_EXTR_1_TEMP = 6,
            SET_EXTR_2_TEMP = 7,
            SET_WAIT_EXTR_1_TEMP = 8,
            SET_WAIT_EXTR_2_TEMP = 9,
            DISABLE_MOTORS = 10,
            CLEAR_STATES = 11,
            MOVE_X_FORWARD = 12,
            MOVE_X_BACKWARD = 13,
            MOVE_Y_FORWARD = 14,
            MOVE_Y_BACKWARD = 15,
            MOVE_Z_FORWARD = 16,
            MOVE_Z_BACKWARD = 17,
            MOVE_E_FORWARD = 18,
            MOVE_E_BACKWARD = 19,
            STOP_MOVE = 20);

struct Motion
{
public:
    float x;
    float y;
    float z;
    float e;
    float f;
    uint8_t command;
    // for circular buffer :
    volatile Motion & operator=(const Motion & motion) volatile
    {
        x = motion.x;
        y = motion.y;
        z = motion.z;
        e = motion.e;
        f = motion.f;
        
        command = motion.command;
        return *this;
    }
    
    Motion(const volatile Motion & motion)
    {
        x = motion.x;
        y = motion.y;
        z = motion.z;
        e = motion.e;
        f = motion.f;
        
        command = motion.command;
    }
    
    Motion(const Motion & motion)
    {
        x = motion.x;
        y = motion.y;
        z = motion.z;
        e = motion.e;
        f = motion.f;

        command = motion.command;
    }
    
    Motion(): x(0.0),
              y(0.0),
              z(0.0),
              e(0.0),
              f(0.0),
              command(0)

    {
    }
};
