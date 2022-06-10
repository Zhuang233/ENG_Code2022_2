#include "WHT101.h"
#include "stdbool.h"
/*
WHT101协议

角度输出：
0x55 0x53 RollL RollH PitchL PitchH YawL YawH TL TH SUM
计算方法：
偏航角（z 轴）Yaw=((YawH<<8)|YawL)/32768*180(°)
温度计算公式：
T=((TH<<8)|TL) /100 ℃
校验和：
Sum=0x55+0x53+RollH+RollL+PitchH+PitchL+YawH+YawL+TH+TL

*/


bool read_start = 0;
uint8_t yaw_data = 0;
uint8_t yaw_buf[11] = {0};
uint8_t buf_size = 0;
uint8_t sum = 0;

uint8_t yawl = 0;
uint8_t yawh = 0;
uint16_t yaw = 0;

float chassis_yaw = 0.0;
float last_yaw = 0.0;
int32_t yaw_turns = 0; 

void decode_WHT101_data()
{

	if(read_start == 1)
	{
		yaw_buf[buf_size++] = yaw_data;
		
		if(buf_size == 11)//接收完毕
		{
			read_start = 0;
			buf_size = 0;
			for(uint8_t i = 0;i < 10 ; i++)
			{
				sum+=yaw_buf[i];
			}
			if(sum == yaw_buf[10] && yaw_buf[1]==0x53)//接收到角度帧且校验成功
			{
				yawh = yaw_buf[7];
				yawl = yaw_buf[6];
				yaw=(yawh<<8)|yawl;
				chassis_yaw =(float)yaw/32768*180;
				if(chassis_yaw > 180.0f)
				{
					chassis_yaw = chassis_yaw - 360.0f;
				}
				if(last_yaw > 170 && chassis_yaw < -170)
				{
					yaw_turns++;
				}
				if(last_yaw < -170 && chassis_yaw > 170)
				{
					yaw_turns--;
				}
				last_yaw = chassis_yaw; 
				chassis_yaw += yaw_turns * 360.0f;
				
			}
			sum = 0;
		}
	}
	
	if(read_start == 0 && yaw_data == 0x55)//检测到帧头
	{
		
		read_start = 1;
		yaw_buf[buf_size++] = yaw_data;
	}
	
}
