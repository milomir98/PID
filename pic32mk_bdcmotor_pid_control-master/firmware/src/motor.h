#ifndef MOTOR_H
#define MOTOR_H

#include "xc.h"
#include "p32xxxx.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h> 
#include "config.h"

#include "config/default/peripheral/mcpwm/plib_mcpwm.h"

#define MAX_MOTOR_SPEED 6000
#define MOTOR_DIRECTION LATBbits.LATB13
#define MOTOR_INH LATAbits.LATA10


typedef enum mot_dir{
    MOT_DIR_LEFT,
    MOT_DIR_RIGHT
}MOT_DIR;


void set_speed(uint16_t speed);

void set_speed_scaled(uint8_t speed);

void set_direction(MOT_DIR direction);

void start_motor(uint16_t speed, MOT_DIR direction);

void start_motor_manual(uint8_t speed);

void stop_motor();



#endif