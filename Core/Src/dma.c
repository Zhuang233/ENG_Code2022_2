#include "dma.h"																	   	  
#include "delay.h"	

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,uint32_t chx,uint32_t dir)
{ 
	//__HAL_LINKDMA(&huart1,hdmarx,hdma_usart1_rx); 
  
  hdma_usart1_rx.Instance = DMA_Streamx;
  hdma_usart1_rx.Init.Channel = chx;
  hdma_usart1_rx.Init.Direction = dir;
  hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
  hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
  hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_usart1_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_usart1_rx.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_usart1_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
  
  HAL_DMA_DeInit(&hdma_usart1_rx);
  HAL_DMA_Init(&hdma_usart1_rx);
} 
//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void DMA_Transfer_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
//	if(DMA_GetCmdStatus(DMA_Streamx) != DISABLE)	//确保DMA可以被设置  
//		return;
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	  
