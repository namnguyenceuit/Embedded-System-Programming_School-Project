#include "stm32f4xx_it.h"
#include "main.h"
#include "queue.h"

volatile uint8_t b_receive_done;
volatile int flag_multi_input;
volatile int time_count_down;
volatile int time_count_up;
volatile int flag_time_update;

queue_t queue_sender;
queue_t queue_receiver;

uint8_t msgSend;
uint8_t msgReceive;
uint8_t b_success;

extern uint8_t SPI_data_get;

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
/*                 STM32F4xx USART Interrupt Handlers                   */
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
  }
}

/******************************************************************************/
/*                 STM32F4xx SPI Interrupt Handlers                   */
/******************************************************************************/
void SPI3_IRQHandler(void)
{
	if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) != RESET)
  {
    SPI_data_get = SPI_I2S_ReceiveData(SPI3);
    //SPI_I2S_ClearFlag(SPI2, SPI_I2S_IT_RXNE);
  }
}

/******************************************************************************/
/*                 STM32F4xx TIMER Interrupt Handlers                   */
/******************************************************************************/

void TIM3_IRQHandler(void)
{
	static uint32_t time1 =0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		if(++time1>1000)
		{
			time1 = 0;
			time_count_down --;
			flag_time_update = 1;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 

	}
}
void TIM4_IRQHandler(void)
{
	static uint32_t time2 =0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		time2 ++;
		if(time2>1000)
		{
			time2 = 0;
			time_count_up ++;
			STM_EVAL_LEDOn(LED3);
		}
		if(time2 == 500)
			STM_EVAL_LEDOff(LED3);
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 

	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
