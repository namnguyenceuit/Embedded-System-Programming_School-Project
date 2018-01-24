#include "stm32f4xx_it.h"
#include "main.h"
#include "queue.h"

/* Private define ------------------------------------------------------------*/
#define STR_MAX_LENGTH	100
/* Private variables ---------------------------------------------------------*/
volatile uint8_t b_receive_done;

queue_t queue_sender;
queue_t queue_receiver;

uint8_t msgSend;
uint8_t msgReceive;
uint8_t b_success;

extern uint8_t SPI_data_get;
extern uint8_t SPI_data_get2;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/******************************************************************************/

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)
  {
    if(0 == queue_is_empty(&queue_sender))
    {
      b_success = 0;
      msgSend = queue_pop(&queue_sender, &b_success);
      USART_SendData(USART3, (uint16_t)msgSend);
    }
    else
    {
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    }
  }
  else if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
  {
    msgReceive = (uint8_t)USART_ReceiveData(USART3);
		queue_push(&queue_receiver, msgReceive);
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		b_receive_done = 1;
		/*
    if(msgReceive == 10 || msgReceive == 27)
    {
			queue_push(&queue_receiver, msgReceive);
      USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
			b_receive_done = 1;
    }
    else
    {
      queue_push(&queue_receiver, msgReceive);
    } */
  }
}

void SPI3_IRQHandler(void)
{
	if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) != RESET)
  {
    SPI_data_get = SPI_I2S_ReceiveData(SPI3);
		SPI_data_get2 = SPI_data_get;
    //SPI_I2S_ClearFlag(SPI2, SPI_I2S_IT_RXNE);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
