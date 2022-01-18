#include "pid.h"


uint16_t pid_error_scaled(uint16_t desired_value, uint16_t tmp_value){
    
    int tmp = (int)(((4096.0/6000.0)*desired_value) - tmp_value);
        
    if(tmp>=0){
        return +(uint16_t)(((4096.0/6000.0)*desired_value) - tmp_value);
    }else{
        return -(uint16_t)(((4096.0/6000.0)*desired_value) - tmp_value);
    }
        
}

int pid_error(uint16_t desired_value, uint16_t tmp_value){
    
    int tmp = desired_value - tmp_value;
        
    return tmp;
        
}

uint16_t pid_control(int error, int error_accumulator, int error_derivative){
    
    static float PID = 0.0;
    
    PID += (Kp * error + Ki * error_accumulator + Kd * error_derivative) * 6000/500;
    
    
    if(PID > PID_MAX)PID = PID_MAX;
    if(PID < PID_MIN)PID = PID_MIN;
    
    return (uint16_t)PID;
}
