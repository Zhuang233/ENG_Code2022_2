#include "can.h"
#include "main.h"
#include "motor.h"
#include <stdbool.h>
#include "eng_control.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

bool CAN1_send_current_flag = false;
bool CAN2_send_current_flag = false;

//calcuelate angle of motor
int16_t d_angle;
void cal_angle(Motor_Msg *motor)
{
	d_angle = motor->angle_actual - motor->angle_last;
	if(d_angle <= -4096) motor->turns += 1;
	else if(d_angle >= 4096) motor->turns -= 1;
	motor->angle = motor->turns * 8192 + motor->angle_actual - motor->original_position;
}

//decode message of motor
void getMotor_message(Motor_Msg *motor,uint8_t *data)
{
	motor->angle_last = motor->angle_actual;
	motor->angle_actual = (uint16_t)(data[0] << 8 | data[1]);
	motor->speed_last = motor->speed_actual;
	motor->speed_actual = (int16_t)(data[2] << 8 | data[3]); 
	motor->real_current = (int16_t)(data[4] << 8 | data[5]);
	motor->temperature = data[6];
  
  if(motor->first_run == true)                  //first_run为true时，记下初始位置
  {
    motor->angle = 0;
    motor->turns = 0;
    motor->angle_last = 0;
    motor->original_position = motor->angle_actual;
    motor->first_run = false;
  }
  else
    cal_angle(motor);
}



/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)  //can1
{
  CAN_RxHeaderTypeDef Rx_Msg;
  uint8_t rx_data[8];

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &Rx_Msg, rx_data);

  switch (Rx_Msg.StdId)			//can1 motor message decode
  {
		case CAN_Motor1_ID:
		case CAN_Motor2_ID:
		case CAN_Motor3_ID:
		case CAN_Motor4_ID:
		case CAN_Motor5_ID:
		case CAN_Motor6_ID:
		case CAN_Motor7_ID:
		case CAN_Motor8_ID:
			{
				static uint8_t i = 0;
				//get motor id
				i = Rx_Msg.StdId - CAN_Motor1_ID;
				getMotor_message(&motor_msg[i],rx_data);
				break;
			}
    default:
				break;		
  }
//  if(CAN1_send_current_flag == true)
//  {
//    CAN1_send_current_flag = false;
//    CAN1_Set_Current();
//  }
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)  //can2
{
	CAN_RxHeaderTypeDef Rx_Msg;
  uint8_t rx_data[8];

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &Rx_Msg, rx_data);

  switch (Rx_Msg.StdId)			//can2 motor message decode
  {
      case CAN_Motor1_ID:
      case CAN_Motor2_ID:
      case CAN_Motor3_ID:
      case CAN_Motor4_ID:
      case CAN_Motor5_ID:
      case CAN_Motor6_ID:
      case CAN_Motor7_ID:
      case CAN_Motor8_ID:
      {
          static uint8_t i = 0;
          //get motor id
          i = Rx_Msg.StdId - CAN_Motor1_ID;
          getMotor_message(&motor_msg[i+8],rx_data);
//					if(zero_flag == 1)
//						CAN2_Set_BackCur(0,pid_gndgrip_spd.outPID,0,0);
//					else ;
					
          break;
      }

      default:
      {
          break;
      }
  }
//  if(CAN2_send_current_flag == true)
//  {
//    CAN2_send_current_flag = false;
//    CAN2_Set_Current();
//  }
}

void CAN1_Set_AheadCur(int16_t C1, int16_t C2, int16_t C3, int16_t C4)
{
  uint8_t TX_Data[8];
  uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;

	Tx_Msg.StdId=0x200;	 //set chassis motor current.
	Tx_Msg.IDE=CAN_ID_STD;		  // 不使用扩展标识符
	Tx_Msg.RTR=CAN_RTR_DATA;		  // 消息类型为数据帧，一帧8位
	Tx_Msg.DLC=8; 
  
  TX_Data[0] = C1 >> 8;
	TX_Data[1] = C1;
	TX_Data[2] = C2 >> 8;
	TX_Data[3] = C2;
	TX_Data[4] = C3 >> 8;
	TX_Data[5] = C3;
	TX_Data[6] = C4 >> 8;
	TX_Data[7] = C4;
  
  HAL_CAN_AddTxMessage(&hcan1, &Tx_Msg, TX_Data, &send_mail_box);    //将数据储存进邮箱FIFOx
}

void CAN1_Set_BackCur(int16_t C1, int16_t C2, int16_t C3, int16_t C4)
{
  uint8_t TX_Data[8];
  uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;

	Tx_Msg.StdId=0x1FF;	 //set chassis motor current.
	Tx_Msg.IDE=CAN_ID_STD;		  // 不使用扩展标识符
	Tx_Msg.RTR=CAN_RTR_DATA;		  // 消息类型为数据帧，一帧8位
	Tx_Msg.DLC=8; 
  
  TX_Data[0] = C1 >> 8;
	TX_Data[1] = C1;
	TX_Data[2] = C2 >> 8;
	TX_Data[3] = C2;
	TX_Data[4] = C3 >> 8;
	TX_Data[5] = C3;
	TX_Data[6] = C4 >> 8;
	TX_Data[7] = C4;
  
  HAL_CAN_AddTxMessage(&hcan1, &Tx_Msg, TX_Data, &send_mail_box);    //将数据储存进邮箱FIFOx
}

void CAN2_Set_AheadCur(int16_t C1, int16_t C2, int16_t C3, int16_t C4)
{
  uint8_t TX_Data[8];
  uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;

	Tx_Msg.StdId=0x200;	 //set chassis motor current.
	Tx_Msg.IDE=CAN_ID_STD;		  // 不使用扩展标识符
	Tx_Msg.RTR=CAN_RTR_DATA;		  // 消息类型为数据帧，一帧8位
	Tx_Msg.DLC=8; 
  
  TX_Data[0] = C1 >> 8;
	TX_Data[1] = C1;
	TX_Data[2] = C2 >> 8;
	TX_Data[3] = C2;
	TX_Data[4] = C3 >> 8;
	TX_Data[5] = C3;
	TX_Data[6] = C4 >> 8;
	TX_Data[7] = C4;
  
  HAL_CAN_AddTxMessage(&hcan2, &Tx_Msg, TX_Data, &send_mail_box);    //将数据储存进邮箱FIFOx
}

void CAN2_Set_BackCur(int16_t C1, int16_t C2, int16_t C3, int16_t C4)
{
  uint8_t TX_Data[8];
  uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;

	Tx_Msg.StdId=0x1FF;	 //set chassis motor current.
	Tx_Msg.IDE=CAN_ID_STD;		  // 不使用扩展标识符
	Tx_Msg.RTR=CAN_RTR_DATA;		  // 消息类型为数据帧，一帧8位
	Tx_Msg.DLC=8; 
  
  TX_Data[0] = C1 >> 8;
	TX_Data[1] = C1;
	TX_Data[2] = C2 >> 8;
	TX_Data[3] = C2;
	TX_Data[4] = C3 >> 8;
	TX_Data[5] = C3;
	TX_Data[6] = C4 >> 8;
	TX_Data[7] = C4;
  
  HAL_CAN_AddTxMessage(&hcan2, &Tx_Msg, TX_Data, &send_mail_box);    //将数据储存进邮箱FIFOx
}


void CAN1_Set_Current(void)
{
  uint16_t i;
  uint32_t send_mail_box;
  uint8_t TX_Data[8];
	CAN_TxHeaderTypeDef Tx_Msg;

	Tx_Msg.StdId=0x200;	 //set chassis motor current.
	Tx_Msg.IDE=CAN_ID_STD;		  // 不使用扩展标识符
	Tx_Msg.RTR=CAN_RTR_DATA;		  // 消息类型为数据帧，一帧8位
	Tx_Msg.DLC=8;
  for(i=0; i<4; i++)
	{
		TX_Data[2*i]  = motor_msg[i].given_current>>8;
		TX_Data[2*i+1]= motor_msg[i].given_current;
	}
  HAL_CAN_AddTxMessage(&hcan1, &Tx_Msg, TX_Data, &send_mail_box);    //将数据储存进邮箱FIFOx
  
	Tx_Msg.StdId=0x1FF;
	Tx_Msg.IDE=CAN_ID_STD;		  // 不使用扩展标识符
	Tx_Msg.RTR=CAN_RTR_DATA;		  // 消息类型为数据帧，一帧8位
	Tx_Msg.DLC=8;
	for(i=0; i<4; i++)
	{
		TX_Data[2*i]  = motor_msg[i+4].given_current>>8;
		TX_Data[2*i+1]= motor_msg[i+4].given_current;
	}  
  HAL_CAN_AddTxMessage(&hcan1, &Tx_Msg, TX_Data, &send_mail_box);    //将数据储存进邮箱FIFOx
}

void CAN2_Set_Current(void)
{
  uint16_t i;
  uint32_t send_mail_box;
  uint8_t TX_Data[8];
	CAN_TxHeaderTypeDef Tx_Msg;

	Tx_Msg.StdId=0x200;	 //set chassis motor current.
	Tx_Msg.IDE=CAN_ID_STD;		  // 不使用扩展标识符
	Tx_Msg.RTR=CAN_RTR_DATA;		  // 消息类型为数据帧，一帧8位
	Tx_Msg.DLC=8;
  for(i=0; i<4; i++)
	{
		TX_Data[2*i]  = motor_msg[i+8].given_current>>8;
		TX_Data[2*i+1]= motor_msg[i+8].given_current;
	}
  HAL_CAN_AddTxMessage(&hcan2, &Tx_Msg, TX_Data, &send_mail_box);    //将数据储存进邮箱FIFOx
  
	Tx_Msg.StdId=0x1FF;
	Tx_Msg.IDE=CAN_ID_STD;		  // 不使用扩展标识符
	Tx_Msg.RTR=CAN_RTR_DATA;		  // 消息类型为数据帧，一帧8位
	Tx_Msg.DLC=8;
	for(i=0; i<4; i++)
	{
		TX_Data[2*i]  = motor_msg[i+12].given_current>>8;
		TX_Data[2*i+1]= motor_msg[i+12].given_current;
	}  
  HAL_CAN_AddTxMessage(&hcan2, &Tx_Msg, TX_Data, &send_mail_box);    //将数据储存进邮箱FIFOx
}

void can_filter_init(void)
{

  CAN_FilterTypeDef CAN_FilterInitStructure;
  CAN_FilterInitStructure.FilterActivation = ENABLE;
  CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
  CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;
  CAN_FilterInitStructure.FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.FilterIdLow = 0x0000;
  CAN_FilterInitStructure.FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.FilterBank = 0;
  CAN_FilterInitStructure.FilterFIFOAssignment = CAN_RX_FIFO0;
  HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);


  CAN_FilterInitStructure.SlaveStartFilterBank = 14;
  CAN_FilterInitStructure.FilterBank = 14;
  CAN_FilterInitStructure.FilterFIFOAssignment = CAN_RX_FIFO1;
  HAL_CAN_ConfigFilter(&hcan2, &CAN_FilterInitStructure);
  HAL_CAN_Start(&hcan2);
  HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);

}


