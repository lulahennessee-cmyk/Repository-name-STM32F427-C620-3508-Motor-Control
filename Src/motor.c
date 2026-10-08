#include "motor.h"
#include "can.h"


 //define variables for motor1
Motor_t motor1;

void Motor_Update(uint8_t data[8])
{
    uint16_t new_ecd; //save value by ecd this time
    
    new_ecd = (uint16_t)((data[0] << 8) | data[1]);
	//data[0]=high8 data[1]=low8
  //0x12         0x34      0x1234-->analysis value of ecd

    motor1.speed_rpm =
        (int16_t)((data[2] << 8) | data[3]);
  //positive negative signs --> direction
    motor1.current =
        (int16_t)((data[4] << 8) | data[5]);

    motor1.temperature = data[6];

    if(motor1.ecd_init == 0)//first
    {
        motor1.ecd = new_ecd;
        motor1.last_ecd = new_ecd;
        motor1.total_ecd = 0;
        motor1.ecd_init = 1;// sign initation

        return;
    }
    int16_t delta;    //delta = new - last
    delta = (int16_t)(new_ecd - motor1.last_ecd);


   
    if(delta > 4096)
    {
        delta -= 8192;
    }
    else if(delta < -4096)
    {
        delta += 8192;
    }// correct direction of rotation


    motor1.total_ecd += delta;

    motor1.ecd = new_ecd;
    motor1.last_ecd = new_ecd;
}




void Motor_SendCurrent(int16_t current)//Sent PIDs output by the from of current,transform current to CAN language
{
    CAN_TxHeaderTypeDef tx_header;  //CAN sent's header-->ID type   Data length
    uint8_t tx_data[8];    //8 bits
    uint32_t tx_mailbox;   //CANmail to sent data

    tx_header.StdId = 0x200;//C620's CAN ID(who)

    tx_header.ExtId = 0;    //no extension ID (only 11 no 29)
	
    tx_header.IDE = CAN_ID_STD;// use standard ID(only 11)(IDE-->extension)

    tx_header.RTR = CAN_RTR_DATA;//(RTR-->Romote Transmission Request)=ordinary data 

    tx_header.DLC = 8;//the value of data bit

    tx_header.TransmitGlobalTime = DISABLE;//(no time information)

    tx_data[0] = (uint8_t)(current >> 8);//CAN-H
    tx_data[1] = (uint8_t)(current & 0xFF);//CAN-L


    tx_data[2] = 0;
    tx_data[3] = 0;
    tx_data[4] = 0;
    tx_data[5] = 0;
    tx_data[6] = 0;
    tx_data[7] = 0;
		//only motor1
		
    HAL_CAN_AddTxMessage(
        &hcan1,
        &tx_header,
        tx_data,
        &tx_mailbox
    );//sent CAN data
}

float Motor_GetAngle(void)
{
    return ((float)motor1.total_ecd * 360.0f) / 8192.0f;
}
