#include "util.h"


void int32_to_str(uint8_t *str, uint32_t num){
    
    uint8_t tmp[4] = {(uint8_t)(num>>24), (uint8_t)(num>>16),(uint8_t)(num>>8),(uint8_t)num};
    
    memcpy(str,tmp,4);

}

void int16_to_str(uint8_t *str, uint16_t num){
    
    uint8_t tmp[2] = {(uint8_t)(num>>8),(uint8_t)num};
    
    memcpy(str,tmp,2);

}

uint16_t str_to_int16(uint8_t *str){
        
    uint8_t tmp1, tmp2;
    
    tmp1 = (uint8_t)str[0];
    tmp2 = (uint8_t)str[1];
    
    uint16_t r;
    
    r = tmp1 << 8;
    r |= tmp2;
    
    return r;
}

uint8_t str_to_int8(uint8_t *str){
        
    uint8_t r;
    
    r = (uint8_t)str[0];
    
    return r;

}
