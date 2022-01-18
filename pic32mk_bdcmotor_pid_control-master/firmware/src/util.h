#ifndef UTIL_H 
#define UTIL_H

#include "xc.h"
#include "p32xxxx.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h> 
#include <string.h>
#include "config.h"

void int32_to_str(uint8_t *str, uint32_t num);

void int16_to_str(uint8_t *str, uint16_t num);

uint16_t str_to_int16(uint8_t *str);

uint8_t str_to_int8(uint8_t *str);


#endif /* UTIL_H */
