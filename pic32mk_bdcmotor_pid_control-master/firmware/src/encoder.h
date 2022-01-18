#ifndef ENCODER_H_
#define ENCODER_H_


#include "xc.h"
#include "p32xxxx.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h> 
#include "config.h"


int encoder_get_count(void);

uint16_t encoder_get_velocity(void);




#endif