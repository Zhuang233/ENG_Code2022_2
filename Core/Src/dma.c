#include "dma.h"																	   	  
#include "delay.h"	

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
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
//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void DMA_Transfer_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
//	if(DMA_GetCmdStatus(DMA_Streamx) != DISABLE)	//ȷ��DMA���Ա�����  
//		return;
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
}	  
