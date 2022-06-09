#include "usart.h"
#include "remote_control.h"
#include "stdio.h"
#include "array.h"
#include "referee.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
//uint8_t		USART1_Tx_buffer[USART1_TX_BUFFER_SIZE] = {0};
uint8_t		USART1_Rx_Buffer[USART1_RX_BUFFER_SIZE] = {0};

uint8_t 	USART2_Rx_Buffer[USART2_RX_BUFFER_SIZE] = {0};
uint8_t 	USART2_Tx_Buffer[USART2_RX_BUFFER_SIZE] = {0};
uint8_t 	USART2_Rx_DMA_Buffer[USART2_RX_BUFFER_SIZE] = {0};
uint8_t 	USART2_Tx_DMA_Buffer[USART2_RX_BUFFER_SIZE] = {0};

uint8_t		USART3_Rx_Buffer[USART3_RX_BUFFER_SIZE] = {0};
uint8_t		USART3_Tx_Buffer[USART3_RX_BUFFER_SIZE] = {0};
uint8_t		USART3_Rx_DMA_Buffer[USART3_RX_BUFFER_SIZE] = {0};
uint8_t		USART3_Tx_DMA_Buffer[USART3_RX_BUFFER_SIZE] = {0};

uint8_t		UART4_Tx_Buffer[UART4_TX_BUFFER_SIZE] = {0};
uint8_t		UART4_Rx_Buffer[UART4_RX_BUFFER_SIZE] = {0};
uint8_t		UART4_Tx_DMA_Buffer[UART4_TX_BUFFER_SIZE] = {0};
uint8_t		UART4_Rx_DMA_Buffer[UART4_RX_BUFFER_SIZE] = {0};

uint8_t		UART5_Tx_Buffer[UART5_RX_BUFFER_SIZE] = {0};
uint8_t		UART5_Rx_Buffer[UART5_RX_BUFFER_SIZE] = {0};
uint8_t		UART5_Tx_DMA_Buffer[UART5_RX_BUFFER_SIZE] = {0};
uint8_t		UART5_Rx_DMA_Buffer[UART5_RX_BUFFER_SIZE] = {0};

uint8_t		USART6_Rx_Buffer[USART6_RX_BUFFER_SIZE] = {0};
uint8_t		USART6_Tx_Buffer[USART6_TX_BUFFER_SIZE] = {0};
uint8_t		USART6_Tx_DMA_Buffer[USART6_TX_BUFFER_SIZE] ={0};
uint8_t		USART6_Rx_DMA_Buffer[USART6_RX_BUFFER_SIZE] = {0};

usart_rt usart1,usart2,usart3,uart4,uart5,usart6;


//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
//#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕
	USART3->DR = (uint8_t) ch;
	return ch;
}
#endif


/**
  * @brief      enable global uart it and do not use DMA transfer done it
  * @param[in]  huart: uart IRQHandler id
  * @param[in]  pData: receive buff 
  * @param[in]  Size:  buff size
  * @retval     set success or fail
  */
int uart_receive_dma_no_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t Size)
{
  uint32_t tmp1 = 0;

  tmp1 = huart->RxState;
	
	if (tmp1 == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0))
		{
			return HAL_ERROR;
		}

		huart->pRxBuffPtr = pData;
		huart->RxXferSize = Size;
		huart->ErrorCode  = HAL_UART_ERROR_NONE;

		/* Enable the DMA Stream */
		HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, Size);

		/* 
		 * Enable the DMA transfer for the receiver request by setting the DMAR bit
		 * in the UART CR3 register 
		 */
		SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

		return HAL_OK;
	}
	else
	{
		return HAL_BUSY;
	}
}

/**
  * @brief          usart dma init
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          串口DMA初始化
  * @param[in]      none
  * @retval         none
  */
void usart_dma_init(void)
{
   	/* open uart idle it */
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
	__HAL_UART_CLEAR_IDLEFLAG(&huart3);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
  
  uart_receive_dma_no_it(&huart1, USART1_Rx_Buffer, USART1_RX_BUFFER_SIZE);
  uart_receive_dma_no_it(&huart3, USART3_Rx_Buffer, USART3_RX_BUFFER_SIZE);
}