#ifndef CONFIG_H
#define CONFIG_H

#include "xc.h"
#include "p32xxxx.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE


#define CMD_SET_KP 'p'  // 112
#define CMD_SET_KI 'i'  // 105
#define CMD_SET_KD 'd'  // 100

#define CMD_START_MOTOR_PID 's' // 115
#define CMD_START_MOTOR_SET_PWM 'M' // 77
#define CMD_START_MOTOR_MANUAL 'm'  // 109 
#define CMD_STOP_MOTOR 'S'  // 83

#define CMD_RESET 'R'   // 82

#define CMD_GET_VELOCITY 'v'    // 118


#endif