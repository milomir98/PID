#include "encoder.h"


int encoder_get_count(void) {
	int r = +(int)POS1CNT;
	return r;
}

uint16_t encoder_get_velocity(void) {
	int r = +(int)VEL1CNT;
    
    if(r & 0x80000000){
        r = ~(r-1);
    }
	return r;
}


