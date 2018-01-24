/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f429i_discovery.h"
#include "stm32f4xx_tim.h"
#include "string.h"
#include "queue.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "uart_isr_configuration.h"
#include "student_info.h"
#include "basic_operation.h"
#include "simple_led.h"
#include "advance_led.h"
#include "timer_counter.h"

#define SS_DISABLE GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SS_ENABLE  GPIO_SetBits(GPIOA, GPIO_Pin_4)

extern volatile uint8_t SPI_data_get;
extern volatile uint8_t b_receive_done;
extern volatile int time_count_down;
extern volatile int time_count_up;
extern volatile int flag_multi_input;
extern volatile int flag_time_update;

extern queue_t queue_sender;
extern queue_t queue_receiver;


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
