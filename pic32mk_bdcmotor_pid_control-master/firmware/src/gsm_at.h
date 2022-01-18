#include "xc.h"
#include "p32xxxx.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h> 
#include "config.h"


#include "config/default/peripheral/uart/plib_uart2.h"
//#include "config/default/peripheral/uart/plib_uart3.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"


#define GSM_MOTOR_ON -1
#define GSM_MOTOR_OFF -2
//#define GSM_VELOCITY 3


void gsmModemInit();

void gsmInit_noResp();

void gsmInit();

void deleteMsg();

int readMsg();