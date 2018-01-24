#include "timer_counter.h"

int chooseOP5;
char* OPTION5 = "5. Timer(a,b):\n\
								a. Countdown\n\
								b. Stopwatch\n\
								ESC: return previous menu\n\
								Your option: ";

void timer_counter()
{
	queue_receiver.capacity = 0;
	uart_send(OPTION5);
	uart_receive();
	from_receive_to_send(&queue_sender, &queue_receiver);				
	uart_send("\n");
	
	chooseOP5 = queue_receiver.items[0];
	// Check if user input two char like "aa" or "bb" or not a char in option like "f"
	//							Enter					 a							e
	while (chooseOP5 < 97 || chooseOP5 > 98)
	{
		if (chooseOP5 == 27)
			return;
		uart_send("Not a option!\n");
		uart_send("\n");
		uart_send(OPTION5);
		uart_receive();
		
		// Print to terminal
		from_receive_to_send(&queue_sender, &queue_receiver);					
		uart_send("\n");
	
		chooseOP5 = queue_receiver.items[0];
	}
	
	switch (chooseOP5)
	{
		case 'a':
			countdown();
			break;
		case 'b':
			stopwatch();
			break;
		default:
			break;
	}
}

void countdown()
{
char a_result[100];	
	int data;
	uart_send("\na. Countdown (Input number & press Enter)");
	uart_send("\nInput time countdown (s): ");
	
	get_input_number(&data);
	time_count_down = data;	
	

	// enable Timer3 and interrupt for timer4
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	while(time_count_down != 0)
	{
		if(flag_time_update == 1)
		{
			sprintf(a_result,"%d",time_count_down);
			uart_send("\n");
			uart_send(a_result);
			uart_send("... ");
			flag_time_update = 0;
		}
	}
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	
	uart_send("\nTime out !");

	do
	{
		uart_send("\n");
		uart_send("ESC: return previous menu\n");;
		uart_receive();
		queue_receiver.capacity = 0;
				
		chooseOP5 = queue_receiver.items[0];
	}while (chooseOP5 != 27);
	timer_counter();
}

void stopwatch()
{
	char a_result[100];	
	STM_EVAL_LEDInit(LED3);
	time_count_up = 0;
	queue_receiver.capacity = 0;
	uart_send("\nb. Stopwatch");
	uart_send("\nPress SPACE to Start\n");
	uart_send("ESC: return previous menu\n");
	uart_receive();
	
	chooseOP5 = queue_receiver.items[0];
	if(chooseOP5 == 27)
	{
		timer_counter();
	}
	
	while(chooseOP5 != 32)
	{
		uart_send("Please press SPACE to Start\n");
		uart_send("ESC: return previous menu\n");
		uart_receive();
			
		chooseOP5 = queue_receiver.items[0];	
		uart_send("\n");
	}
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	queue_receiver.capacity = 0;
	uart_send("\nCounting...\nPress SPACE to Stop\n");
	uart_send("\n");
	uart_receive();
	
	chooseOP5 = queue_receiver.items[0];
	while(chooseOP5 != 32)
	{
		queue_receiver.capacity = 0;
		uart_send("Counting...\n");
		uart_send("Please press SPACE to Stop\n");
		uart_receive();
		
		chooseOP5 = queue_receiver.items[0];
		uart_send("\n");
	}		
	
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM4, DISABLE);
	STM_EVAL_LEDOff(LED3);

	uart_send("Time counted: ");
	sprintf(a_result,"%d", time_count_up);
	uart_send(a_result);
	uart_send("s");
	
	do
	{
		queue_receiver.capacity = 0;
		uart_send("\n");
		uart_send("ESC: return previous menu\n");;
		uart_send("\n");
		uart_receive();
				
		chooseOP5 = queue_receiver.items[0];
	}while (chooseOP5 != 27);
	timer_counter();
	time_count_up = 0;
} 

void timer3_interrupt_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef nvic_timer;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/4)/1000000)-1;   // frequency = 1MHz
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1;														// 1ms
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	nvic_timer.NVIC_IRQChannel = TIM3_IRQn;
  nvic_timer.NVIC_IRQChannelPreemptionPriority = 0;
  nvic_timer.NVIC_IRQChannelSubPriority = 1;
  nvic_timer.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_timer);   
	
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM3, DISABLE);
}
void timer4_interrupt_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef nvic_timer;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/4)/1000000)-1;   // frequency = 1MHz
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1;														// 1ms
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	nvic_timer.NVIC_IRQChannel = TIM4_IRQn;
  nvic_timer.NVIC_IRQChannelPreemptionPriority = 0;
  nvic_timer.NVIC_IRQChannelSubPriority = 1;
  nvic_timer.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_timer);   
	
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM4, DISABLE);
}

