#include "spi.h"

uint32_t spi1_err=0;
uint32_t spi5_err=0;

//void SPI5_Init()
//{
//	SPI_InitTypeDef SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

//	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_SPI5);
//	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_SPI5);
//	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_SPI5);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOF, &GPIO_InitStructure);

//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
//	SPI_Init(SPI5, &SPI_InitStructure);
//	//SPI_CalculateCRC(SPI1, DISABLE);                                      //Enable CRC 

//	SPI_SSOutputCmd(SPI5, ENABLE);  
//	SPI_Cmd(SPI5, ENABLE);

//}
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
//uint8_t SPI5_ReadWriteByte(uint8_t TxData,uint8_t *RxData)
//{		 			 
//	uint16_t time_out=0;
//  while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET)
//	{
//		if(time_out++==1000)
//		{
//			spi1_err++;
//			return SPI_FAILED;
//		}
//	}//等待发送区空  
//	time_out=0;
//	SPI_I2S_SendData(SPI5, TxData); //通过外设SPIx发送一个byte  数据
//		
//  while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET)
//	{
//		if(time_out++==1000)
//		{
//			spi1_err++;
//			return SPI_FAILED;
//		}
//	}
//	*RxData=SPI_I2S_ReceiveData(SPI5); //返回通过SPIx最近接收的数据	
// 	return SPI_OK;	    
//}

