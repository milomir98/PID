#include "gsm_at.h"


uint8_t at_ok[6] = "\r\nOK\r\n";
//uint8_t msg_rec[17] = {'\r', '\n', '+', 'C', 'M', 'T', 'I', ':', ' ', '"', 'S', 'M', '"', ',', '1', '\r', '\n'};
uint8_t read_msg_hdr[23] = {'\r', '\n', '+', 'C', 'M', 'G', 'R', ':', ' ', '"', 'R', 'E', 'C', ' ', 'U', 'N', 'R', 'E', 'A', 'D', '"', ',', '"'};
uint8_t msg_motorOn[14] = "Pokreni motor.";
uint8_t msg_motorOff[15] = "Zaustavi motor.";
uint8_t msg_velocity[7] = "Brzina ";

uint8_t velocity_tmp[3];
uint16_t gsmTmp = 0;

uint8_t i = 0;
uint8_t flag = 0;
uint8_t rxbuffer[128];
uint8_t cmd;
uint8_t num[13];
uint8_t text_msg[161];


void gsmModemInit()
{
    LATFbits.LATF0 = 0;
    CORETIMER_DelayMs(100);
    LATFbits.LATF0 = 1;
    CORETIMER_DelayMs(15000);
    LATFbits.LATF0 = 0;
}

void gsmInit_noResp()
{
    // GSM MODEM INIT //
    UART2_Write("AT\r", 3);
    while(! UART2_ReceiverIsReady());
    
    // ECHO TURN OFF //
    UART2_Write("ATE0\r", 6);
    while(! UART2_ReceiverIsReady());
    
    CORETIMER_DelayMs(500);
}

void gsmInit()
{
    // GSM MODEM INIT //
    UART2_Write("AT\r", 3);
    while(! UART2_ReceiverIsReady());
    UART2_Read(rxbuffer, 6);
    for(int j = 0; j<6; j++)
    {
        //UART3_WriteByte(rxbuffer[j]);
        if(rxbuffer[j] != at_ok[j])
            flag = 1;
    }
    //if(flag == 0)
    //    UART3_WriteByte('S');
    //UART3_WriteByte('\r');
    
    // ECHO TURN OFF //
    UART2_Write("ATE0\r", 6);
    while(! UART2_ReceiverIsReady());
    UART2_Read(rxbuffer, 6);
    for(int j = 0; j<6; j++)
    {
        //UART3_WriteByte(rxbuffer[j]);
        if(rxbuffer[j] != at_ok[j])
            flag = 1;
    }
    //if(flag == 0)
    //    UART3_WriteByte('S');
    //UART3_WriteByte('\r');
    
    // DELETE MESSAGES FROM MEMORY //
    UART2_Write("AT+CMGD=0,4\r", 13);
    while(! UART2_ReceiverIsReady());
    UART2_Read(rxbuffer, 6);
    for(int j = 0; j<6; j++)
    {
        //UART3_WriteByte(rxbuffer[j]);
        if(rxbuffer[j] != at_ok[j])
            flag = 1;
    }
    //if(flag == 0)
    //    UART3_WriteByte('S');
    //UART3_WriteByte('\r');
    
    // MESSAGES FORMAT //
    UART2_Write("AT+CMGF=1\r", 11);
    while(! UART2_ReceiverIsReady());
    UART2_Read(rxbuffer, 6);
    for(int j = 0; j<6; j++)
    {
        //UART3_WriteByte(rxbuffer[j]);
        if(rxbuffer[j] != at_ok[j])
            flag = 1;
    }
    //if(flag == 0)
    //    UART3_WriteByte('S');
    //UART3_WriteByte('\r');
}

void deleteMsg()
{
    // DELETE MESSAGES FROM MEMORY //
    UART2_Write("AT+CMGD=0,4\r", 13);
    flag = 0;
    while(! UART2_ReceiverIsReady());
    UART2_Read(rxbuffer, 6);
    for(int j = 0; j<6; j++)
    {
        //UART3_WriteByte(rxbuffer[j]);
        if(rxbuffer[j] != at_ok[j])
            flag = 1;
    }
    //if(flag == 0)
    //    UART3_WriteByte('S');
    //UART3_WriteByte('\r');
}

int readMsg()
{
    UART2_Write("AT+CMGR=1\r", 11);
    while(! UART2_ReceiverIsReady());
    UART2_Read(rxbuffer, 23);
    for(int j = 0; j<23; j++)
    {
        //UART3_WriteByte(rxbuffer[j]);
        if(rxbuffer[j] != read_msg_hdr[j])
            flag = 1;
    }
    //if(flag == 0)
    //    UART3_WriteByte('S');
    //UART3_WriteByte('\r');
    
    UART2_Read(rxbuffer, 13);
    if(rxbuffer[13] == '"')
    {
        for(int j = 0; j < 12; j++)
            num[j] = rxbuffer[j];
    }
    else
    {
        for(int j = 0; j < 13; j++)
            num[j] = rxbuffer[j];
        UART2_Read(rxbuffer, 31); //IGNORE ", DATE AND TIME, AND \r\n - 1+28+2
    }    
    
    cmd = 0;
    i = 0;
    while(cmd != 0x0d)
    {
        while(! UART2_ReceiverIsReady());
        cmd = UART2_ReadByte();
        //UART3_WriteByte(cmd);
        text_msg[i] = cmd;
        i++;
    }

    //UART3_Write(num, 13);
    //UART3_WriteByte('S');
    //UART3_WriteByte('\r');
    
    
    //UART3_Write(text_msg, i-1);
    //UART3_WriteByte('S');
    //UART3_WriteByte('\r');
    
    flag = 0;
    UART2_Read(rxbuffer, 1);
    UART2_Read(rxbuffer, 6);
    for(int j = 0; j<6; j++)
    {
        //UART3_WriteByte(rxbuffer[j]);
        if(rxbuffer[j] != at_ok[j])
            flag = 1;
    }
    //if(flag == 0)
    //    UART3_WriteByte('B');
    //UART3_WriteByte('\r');
    
    if(memcmp(text_msg, msg_motorOn, sizeof(msg_motorOn)) == 0)
        return GSM_MOTOR_ON;
    
    if(memcmp(text_msg, msg_motorOff, sizeof(msg_motorOff)) == 0)
        return GSM_MOTOR_OFF;
    
    if(memcmp(text_msg, msg_velocity, sizeof(msg_velocity)) == 0)
    {
        if(text_msg[10] == '.')
        {
            velocity_tmp[2] = text_msg[7];
            velocity_tmp[1] = text_msg[8];
            velocity_tmp[0] = text_msg[9];
            
            gsmTmp = velocity_tmp[0] - 48;
            gsmTmp += (velocity_tmp[1] - 48) * 10;
            gsmTmp += (velocity_tmp[2] - 48) * 100;
            return gsmTmp;
        }
        if(text_msg[9] == '.')
        {
            velocity_tmp[1] = text_msg[7];
            velocity_tmp[0] = text_msg[8];

            gsmTmp = velocity_tmp[0] - 48;
            gsmTmp += (velocity_tmp[1] - 48) * 10;
            return gsmTmp;
        }
        if(text_msg[8] == '.')
        {
            velocity_tmp[0] = text_msg[7];
            
            gsmTmp = velocity_tmp[0] - 48;
            return gsmTmp;
        }
    }

    return 0;
}