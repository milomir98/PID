#ifndef PID_H
#define PID_H

#include "xc.h"
#include "p32xxxx.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h> 
#include "config.h"

#define PID_MAX 6000
#define PID_MIN 0

extern volatile float Kp;
extern volatile float Ki; 
extern volatile float Kd;


uint16_t pid_error_scaled(uint16_t, uint16_t);

int pid_error(uint16_t, uint16_t);

uint16_t pid_control(int,int,int);

#endif