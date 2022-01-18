#include "xc.h"
#include "p32xxxx.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>
#include <stdio.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "util.h"
#include "motor.h"
#include "pid.h"
#include "encoder.h"
#include "config.h"
#include "gsm_at.h"


   
volatile uint32_t update_count=0;
volatile uint32_t delay_count=0;

volatile float Kp = 0.074;
volatile float Ki = 0; 
volatile float Kd = 0.371;

uint8_t msg_rec[17] = {'\r', '\n', '+', 'C', 'M', 'T', 'I', ':', ' ', '"', 'S', 'M', '"', ',', '1', '\r', '\n'};

int main ( void )
{
    uint8_t str[20];// = {0x07,0xD0,'\n'}; //desired_velocity
    uint8_t str2[20]; //print velocity
    uint8_t str3[] = {'a','b','\n'};    // print PID
    uint8_t buffer[] = {'a','b','\n'};
    uint8_t buffer8[] = {'a','\n'};
    
    uint8_t cmd8 = 0;
    uint8_t tmp = 0;
    uint8_t str_tmp[20];
    
    uint8_t tmp8 = 0;
    uint16_t tmp16 = 0;
    uint16_t pid_output = 0;
    
    uint16_t velocity;
    uint16_t desired_velocity;
    
    int error_velocity = 0;
    int error_previous = 0;
    int error_accumulator = 0;
    int error_derivative = 0;
    int last_velocity = 0;
    
    velocity = 0;
    desired_velocity = 0;
    
    
    bool state = false;
        
    MOT_DIR direction;
    
    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    TMR2_Start();
    
    QEI1_Start();

    MCPWM_Start();
    
    stop_motor();
    
    int i = 0, flag = 0;
    int ctrl = 0;
    
    while ( true )
    {
        //uint8_t flag = 0;
        if(UART2_ReceiverIsReady())
        {
            tmp = UART2_ReadByte();
            str_tmp[i] = tmp;
            //UART3_WriteByte(cmd8);
            i++;
            flag = 1;
            if(i == 17 && flag == 1)
            {   
                //UART3_WriteByte('U');
                for(int j = 0; j<17; j++)
                    if(str_tmp[j] != msg_rec[j])
                    {
                        flag = 0;
                        //UART3_WriteByte(str[j]);
                    }
                if(flag == 1)
                {
                    //UART3_WriteByte('S');
                    ctrl = readMsg();
                    //UART3_WriteByte('S');
                }
                flag = 0;
                i = 0;
                deleteMsg();
            }
        }
        /*if(ctrl == GSM_MOTOR_ON)
        {
            UART3_WriteByte('\r');
            UART3_WriteByte('N');
            UART3_WriteByte('\r');
            ctrl = 5;
        }
        if(ctrl == GSM_MOTOR_OFF)
        {
            UART3_WriteByte('\r');
            UART3_WriteByte('F');
            UART3_WriteByte('\r');
            ctrl = 5;
        }
        if(ctrl == 0)
        {
            UART3_WriteByte('\r');
            UART3_WriteByte('P');
            UART3_WriteByte('\r');
            ctrl = 5;
        }*/
        if(UART3_ReceiverIsReady() || ctrl != 0)
        {
            if(ctrl == -1)
            {
                if(last_velocity == GSM_MOTOR_ON)
                    desired_velocity = 55;//last_velocity;
                else
                    desired_velocity = last_velocity;
                
                state = true;

                if(desired_velocity>500)desired_velocity=500;
                direction = tmp16 >> 15;
            }
            if(ctrl == GSM_MOTOR_OFF)
            {
                cmd8 = CMD_STOP_MOTOR;
                desired_velocity = 0;
            }
            if(ctrl == 0)
                cmd8 = UART3_ReadByte();
            if(ctrl > 0)//2 && ctrl < 1000)
            {
                desired_velocity = ctrl;
                
                state = true;
                
                if(desired_velocity>500)desired_velocity=500;
                direction = tmp16 >> 15;
                last_velocity = desired_velocity;
            }
            
            ctrl = 0;
        
            switch (cmd8) {
                case CMD_SET_KP:
                  //  UART3_WriteByte('P');
                    UART3_Read(buffer,2);
                    tmp16 = str_to_int16(buffer);
                    Kp = tmp16/1000.0;

                    break;

                case CMD_SET_KI:
                  //  UART3_WriteByte('I');
                    UART3_Read(buffer,2);
                    tmp16 = str_to_int16(buffer);
                    Ki = tmp16/10000.0;

                    break;

                case CMD_SET_KD:
                  //  UART3_WriteByte('D');
                    UART3_Read(buffer,2);
                    tmp16 = str_to_int16(buffer);
                    Kd = tmp16/1000.0;

                    break;    

                case CMD_START_MOTOR_PID:
                    state = true;
                    //UART3_WriteByte('s');
                    UART3_Read(buffer,2);
                    tmp16 = str_to_int16(buffer);

                    desired_velocity = tmp16 & 0x01FF;
                    if(desired_velocity>500)desired_velocity=500;
                    direction = tmp16 >> 15;
                    last_velocity = desired_velocity;
                    //int16_to_str(str3,desired_velocity);
                    //UART3_Write(str3,3);
                   // sprintf(str3,"%u\n",desired_velocity);
                   // UART3_Write(str3,strlen(str3));
                    
                    //desired_velocity = tmp16 & 0x3FFF;
                   // if(desired_velocity>10600)desired_velocity=10600;
                   // direction = desired_velocity >> 15;

                    break;  
                    
                case CMD_START_MOTOR_SET_PWM:
                    state = false;
                    //UART3_Write("START",5);
                    UART3_Read(buffer,2);
                    tmp16 = str_to_int16(buffer);                    
                    if(tmp16>6000)tmp16 = 6000;
                    start_motor(tmp16,0);

                    break;
                
                case CMD_START_MOTOR_MANUAL:
                    state = false;
                   // UART3_WriteByte('M');
                    //UART3_Write("MANUAL",6);
                    UART3_Read(buffer8,1);
                    tmp8 = str_to_int8(buffer8);  
                    //UART3_WriteByte(tmp8);
                    start_motor_manual(tmp8);

                    break;
                   
                    
                case CMD_STOP_MOTOR:
                    state = false;
                    //UART3_WriteByte('S');
                    //UART3_Write("STOP",4);

                    stop_motor();

                    break;
                    
                case CMD_GET_VELOCITY:
                    //UART3_WriteByte('V');
                    int16_to_str(str3,velocity);
                    UART3_Write(str3,2);

                    break;
                    
                case CMD_RESET:
                    //UART3_WriteByte('R');
                    state = false;
                    desired_velocity = 0;
                    error_velocity = 0;
                    error_accumulator = 0;
                    error_derivative = 0;
                    
                    Kp = 0;
                    Ki = 0;
                    Kd = 0;
                    stop_motor();

                    break;
            }
            
            cmd8 = 0;
        
            //last_velocity = desired_velocity;
        }

//        if(delay_count>=100){
           
//            sprintf(str2,"V %u\n", velocity);
            //sprintf(str,"G %u %u %d %u\n", desired_velocity, velocity, error_velocity, boliMeDjole);
//            UART3_Write(str2,strlen(str2));
            

            //if(ADCHS_ChannelResultIsReady(ADCHS_CH2)){
            //    current_sense_1 = ADCHS_ChannelResultGet(ADCHS_CH2);
            //}
            //
            //int16_to_str(str3,current_sense_1);
            //UART3_Write(str3,3);
            
            
//            delay_count = 0;
             
//        }
        
        
        

        if(update_count>=10){
                             
            velocity = (uint16_t)((500.0/7140.0)*(float)encoder_get_velocity());
            
            error_previous = error_velocity;
            
            error_velocity = pid_error(desired_velocity,velocity);//desired_velocity - velocity;//
            
            error_accumulator += error_velocity;
            
            error_derivative = error_previous - error_velocity;
 
            pid_output = pid_control(error_velocity, error_accumulator, error_derivative);
            
            if(state)start_motor(pid_output,direction);
            
            
            sprintf(str,"G %u %u %d %u\n", desired_velocity, velocity, error_velocity, pid_output); // - TREBA OTKOMENTARISATI
            UART3_Write(str,strlen(str)); // - TREBA OTKOMENTARISATI
            
            //int16_to_str(str2,error_velocity);
            //UART3_Write(str2,2);
            //int16_to_str(str3,velocity);
            //UART3_Write(str3,3);
            
            update_count=0;
        }

    }

    return ( EXIT_FAILURE );
}

