#include "motor.h"

void set_speed(uint16_t speed){
    
    uint16_t duty;
    
    if(MOTOR_DIRECTION)
        speed=6000-speed;
  
    duty = speed;// * MAX_MOTOR_SPEED / 100;
    
    MCPWM_ChannelPrimaryDutySet(1,duty);
}

void set_speed_scaled(uint8_t speed){
    
    uint16_t duty;
    
    if(MOTOR_DIRECTION)
        speed=100-speed;
  
    duty = speed * MAX_MOTOR_SPEED / 100;
    
    MCPWM_ChannelPrimaryDutySet(1,duty);
}

void set_direction(MOT_DIR direction){
    
    if(direction){
        MOTOR_DIRECTION = 0;
    }else{
        MOTOR_DIRECTION = 1;
    }
}

void start_motor(uint16_t speed, MOT_DIR direction){
    MOTOR_INH = 1;
    set_speed(speed);
    set_direction(direction);
}

void start_motor_manual(uint8_t speed){
    MOTOR_INH = 1;
    
    if(speed & 0x80){
        set_direction(MOT_DIR_LEFT);
    }else{
        set_direction(MOT_DIR_RIGHT);
    }
    
    speed &= 0x7F;
    
    set_speed_scaled(speed);
}

void stop_motor(){
    MOTOR_INH = 0;
    set_speed(0);
    set_direction(0);
}