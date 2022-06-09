#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

//IO��������
#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7���ģʽ
//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void i2cdevInit(void);
void i2cReadBytes(u8 DevID,u8 Addr,u8 *Pbuf,u8 Num);
void i2cReadBytesForOptFlow(u8 DevID,u8 Addr,u8 *Pbuf,u8 Num);
void i2cWriteByte(u8 DevID,u8 Addr,u8 Dat);
#endif
