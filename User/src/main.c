#include "main.h"
#include "queue.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#define STR_MAX_LENGTH	100
#define TC_BIT			0x0040
#define TC_SHIFT_LEFT	6
#define RXNE_BIT		0x0020
#define RXNE_SHIFT_LEFT	5
#define SS_DISABLE GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SS_ENABLE  GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define EMPTY_QUEUE_RECEIVER queue_receiver.capacity = 0;

void uart_interrupt_init(void);
void timer3_interrupt_init(void);
void timer4_interrupt_init(void);
void SPI_Configuration(void);

void uart_send(char *q);
void uart_receive(void);
void from_receive_to_send(queue_t * send, queue_t * receive);
void queue_push_string(queue_t * q, const char * string, const uint8_t length);
int get_data(queue_t q);
void countdown(void);
void stopwatch(void);
void option2_input(int *a, int *b);
void get_input_number(int *operand);
void option2_print_result(char *result);

void plus(void);
void subtract(void);
void multiply(void);
void divide(void);
void module(void);
void blink(void);
void timer_counter(void);

void student_info(void);
void basic_operation(void);
void simple_led(void);
void advance_led(void);

volatile uint8_t SPI_data_get;

extern volatile uint8_t b_receive_done;
extern volatile int time_count_down;
extern volatile int time_count_up;
extern volatile int flag_multi_input;
extern volatile int flag_time_update;

extern queue_t queue_sender;
extern queue_t queue_receiver;

char* MAIN_MENU = "\n----------MAIN MENU----------\n\
										Choose your option (1, 2, ..): \n\
										1. Student info\n\
										2. Basic operation\n\
										3. Simple led\n\
										4. Advance led\n\
										5. Timer\n\
										ESC: return previous menu\n\
										Your option: ";

char* OPTION1 = "\n1. Student info:\n\
								ID: 14520555\n\
								Full name: Nguyen Thanh Nam\n\
								ESC: return previous menu\n";

char* OPTION2 = "2. Basic operation( a,b,..):\n\
								a. Plus\n\
								b. Subtract\n\
								c. Multiply\n\
								d. Divide\n\
								e. Module\n\
								ESC: return previous menu\n\
								Your option: ";

char* OPTION3 = "3. Simple led:\n\
								a. On Green\n\
								b. Off Green\n\
								c. Blink led\n\
								ESC: return previous menu\n\
								Your option: ";

char* OPTION4 = "4. Advance led(a,b,c):\n\
								a. Set set led\n\
								b. Set direction\n\
								c. Start\n\
								ESC: return previous menu\n\
								Your option: ";

char* OPTION5 = "5. Timer(a,b):\n\
								a. Countdown\n\
								b. Stopwatch\n\
								ESC: return previous menu\n\
								Your option: ";

char* NEWLINE = "\n";
char* ESC = "ESC: return previous menu\n";

int choose;
queue_t queue_get_data;

char* c_result;
char a_result[100];	
int i_result;

int main(){
	queue_init(&queue_sender);
	queue_init(&queue_receiver);
	queue_init(&queue_get_data);
	
	
	// Uart interrupt init
	uart_interrupt_init();
	timer3_interrupt_init();
	timer4_interrupt_init();

	for(;;){
		queue_receiver.capacity = 0;
		// Send option & wait for user input
		uart_send(MAIN_MENU);
		//while(queue_is_empty(&queue_receiver));
		uart_receive();
		
		queue_get_data = queue_receiver;
		from_receive_to_send(&queue_sender, &queue_receiver);
		uart_send(NEWLINE);
		
		// Check input option
		// TODO: check if user input orther keys
		choose = get_data(queue_get_data);
		
		switch (choose)
		{
			case 1:
				student_info();
				break;
			case 2:
				basic_operation();
				break;
			case 3:
				simple_led();
				break;
			case 4:
				advance_led();
				break;
			case 5:
				timer_counter();
				break;
		}
	}
	return 0;
}

void student_info()
{
	uart_send(OPTION1);
	uart_send(NEWLINE);
	uart_receive();	
}

void basic_operation()
{
	// Home screen option 2
	queue_receiver.capacity = 0;
	uart_send(NEWLINE);
	uart_send(OPTION2);
	uart_receive();	
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);					
	uart_send(NEWLINE);
	
	// get data to check if wrong input
	choose = queue_get_data.items[0];
	
	// Check if user input two char like "aa" or "bb" or not a char in option like "f"
	//							Enter					 a							e
	while (choose < 97 || choose > 101)
	{
		if (choose == 27)
			return;
		uart_send("Not a option!\n");
		uart_send(NEWLINE);
		uart_send(OPTION2);
		uart_receive();
		
		queue_get_data = queue_receiver;
		// Print to terminal
		from_receive_to_send(&queue_sender, &queue_receiver);					
		uart_send(NEWLINE);
	
		choose = queue_get_data.items[0];
	}
		
		//TODO: ESC doesn't show the previous menu
	//TODO: check if input_operand not a number
		switch (choose)
		{
			case 'a':
				plus();
				break;
			case 'b':
				subtract();
				break;
			case 'c':
				multiply();
				break;
			case 'd':
				divide();
				break;
			case 'e':
				module();
				break;
		}			
}

void simple_led()
{
	uart_send(NEWLINE);
	uart_send(OPTION3);
	uart_receive();	
	queue_get_data = queue_receiver;
	
	from_receive_to_send(&queue_sender, &queue_receiver);				
	uart_send(NEWLINE);
	
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	
	// get data to check if wrong input
	choose = queue_get_data.items[0];
	
	// Check if user input two char like "aa" or "bb" or not a char in option like "f"
	// TODO: define variables' name
	//    					 a							c
	while (choose < 97 || choose > 99)
	{
		if (choose == 27)
			return;
		uart_send("Not a option!\n");
		uart_send(NEWLINE);
		uart_send(OPTION3);
		uart_receive();
		
		queue_get_data = queue_receiver;
		// Print to terminal
		from_receive_to_send(&queue_sender, &queue_receiver);					
		uart_send(NEWLINE);
	
		choose = queue_get_data.items[0];
	}
	
	switch (choose)
	{
		case 'a':
			STM_EVAL_LEDOn(LED3);
			simple_led();
			break;
		case 'b':
			STM_EVAL_LEDOff(LED3);
			simple_led();
			break;
		case 'c':
			blink();
			simple_led();
			break;
	}
}

void advance_led()
{
	uart_send(OPTION4);
	uart_receive();
	SPI_Configuration();
	SS_DISABLE;

	SPI_I2S_SendData(SPI1, 99);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	SS_ENABLE;
}

void timer_counter()
{
	queue_receiver.capacity = 0;
	uart_send(OPTION5);
	uart_receive();
	from_receive_to_send(&queue_sender, &queue_receiver);				
	uart_send(NEWLINE);
	
	choose = queue_receiver.items[0];
	
	switch (choose)
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
	int check;
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
			uart_send(NEWLINE);
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
		uart_send(NEWLINE);
		uart_send(ESC);;
		uart_receive();
		queue_receiver.capacity = 0;
				
		choose = queue_receiver.items[0];
	}while (choose != 27);
	timer_counter();
}

void stopwatch()
{
	STM_EVAL_LEDInit(LED3);
	time_count_up = 0;
	queue_receiver.capacity = 0;
	uart_send("\nb. Stopwatch");
	uart_send("\nPress SPACE to Start\n");
	uart_send("ESC: return previous menu\n");
	uart_receive();
	
	choose = queue_receiver.items[0];
	if(choose == 27)
	{
		timer_counter();
	}
	
	while(choose != 32)
	{
		uart_send("Please press SPACE to Start\n");
		uart_send("ESC: return previous menu\n");
		uart_receive();
			
		choose = queue_receiver.items[0];	
		uart_send(NEWLINE);
	}
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	queue_receiver.capacity = 0;
	uart_send("\nCounting...\nPress SPACE to Stop\n");
	uart_send(NEWLINE);
	uart_receive();
	
	choose = queue_receiver.items[0];
	while(choose != 32)
	{
		queue_receiver.capacity = 0;
		uart_send("Counting...\n");
		uart_send("Please press SPACE to Stop\n");
		uart_receive();
		
		choose = queue_receiver.items[0];
		uart_send(NEWLINE);
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
		uart_send(NEWLINE);
		uart_send(ESC);;
		uart_receive();
		queue_receiver.capacity = 0;
				
		choose = queue_receiver.items[0];
	}while (choose != 27);
	timer_counter();
	time_count_up = 0;
} 

void SPI_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* SPI_MASTER configuration -----------------------------*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
	/*-------- Configuring SlaveSelect-Pin PA4 --------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*-------- Configuring SCK, MISO, MOSI --------*/
	//																SCK					MISO				MOSI
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* SPI_SLAVE configuration ------------------------------*/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/*-------- Configuring SCK, MISO, MOSI --------*/
	//																	SS					SCK						MISO				MOSI
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_SPI3);  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3);   
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3);  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SPI3);  
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI3, &SPI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
  SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, ENABLE);
 
  /* Enable SPI_SLAVE */
  SPI_Cmd(SPI3, ENABLE);
  /* Enable SPI_MASTER */
  SPI_Cmd(SPI1, ENABLE); 
}

void blink()
{
	int i, j, times;
	char* INPUT_TIMES = "Blink LED! Input times you want to blink: ";
	char* BLINKING = "Blinking.... Pls wait & look at the board...\n";
		
	/* Process for times input	*/
	queue_push_string(&queue_sender, NEWLINE, strlen(NEWLINE));
	uart_send(INPUT_TIMES);
	get_input_number(&times);
	uart_send(NEWLINE);
	uart_send(BLINKING);
	
	for (i = 0; i < times; i++)
	{
			STM_EVAL_LEDOn(LED3);
			STM_EVAL_LEDOn(LED4);
			for(j = 0; j < 3000000; j++);
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOff(LED4);
			for(j = 0; j < 3000000; j++);
	}
}
void plus()
{
	// variables for calculation
	int operand1;
	int operand2;
	
	option2_input(&operand1, &operand2);
	
	// Cal sum
	i_result = operand1 + operand2;
	
	// Convert back to string
	sprintf(a_result,"%d",i_result);
	c_result = a_result;
	
	// Print result
	option2_print_result(c_result);
}

void subtract()
{
	// variables for calculation
	int operand1;
	int operand2;

	option2_input(&operand1, &operand2);
	
	// Cal sum
	i_result = operand1 - operand2;
	
	// Convert back to string
	sprintf(a_result,"%d",i_result);
	c_result = a_result;
	
	// Print result
	option2_print_result(c_result);
}

void multiply(void)
{
	// variables for calculation
	int operand1;
	int operand2;

	option2_input(&operand1, &operand2);
	
	// Cal sum
	i_result = operand1 * operand2;
	
	// Convert back to string
	sprintf(a_result,"%d",i_result);
	c_result = a_result;
	
	// Print result
	option2_print_result(c_result);
}


void divide(void)
{
	// variables for calculation
	int operand1;
	int operand2;

	float div;

	option2_input(&operand1, &operand2);
	
	// Cal sum
	div = (float)(operand1) / (float)(operand2);
	
	// Convert back to string
	sprintf(a_result,"%0.2f",div);
	c_result = a_result;
	
	// Print result
	option2_print_result(c_result);	
}

void module(void)
{
	// variables for calculation
	int operand1;
	int operand2;

	float mol;

	option2_input(&operand1, &operand2);
	
	// Cal sum
	mol = sqrt((float)(operand1 * operand1) + (float)(operand2 * operand2));
	
	// Convert back to string
	sprintf(a_result,"%0.2f",mol);
	c_result = a_result;
	
	// Print result
	option2_print_result(c_result);	
}

void option2_print_result(char *result)
{
	
	char* txtResult = "Result: ";	
	
	
	uart_send(NEWLINE);
	uart_send(txtResult);
	uart_send(result);	
	do
	{
		uart_send(NEWLINE);
		uart_send(ESC);;
		uart_receive();
		queue_receiver.capacity = 0;
				
		choose = queue_receiver.items[0];
	}while (choose != 27);
	
	basic_operation();
}

void option2_input(int * a, int *b)
{	
	char* NUM1_REQUEST = "Operand 1: ";
	char* NUM2_REQUEST = "Operand 2: ";
	
	
	//Process for operand 1	
	uart_send(NEWLINE);
	uart_send(NUM1_REQUEST);
	get_input_number(a);
	
	
	uart_send(NEWLINE);
	// Process for operand 2
	uart_send(NUM2_REQUEST);
	get_input_number(b);
}

void get_input_number(int *operand)
{
	int i = 0, data_receive;
	char data[100];
	for(;;)
	{
		queue_receiver.capacity = 0;
		uart_receive();
		
		data_receive = queue_receiver.items[0];
		
		if (data_receive < 48 || data_receive > 57)
		{
			if (data_receive == 10)
				break;
			else
				continue;
		}
		else
		{
				data[i] = data_receive;
			
				// Print to terminal
				from_receive_to_send(&queue_sender, &queue_receiver);
				i++;
		}
	}
	*operand = atoi(data);
	queue_receiver.capacity = 0;
	free(data);
}

void uart_interrupt_init()
{
	GPIO_InitTypeDef gpio_init;
	USART_InitTypeDef usart_init;
	NVIC_InitTypeDef nvic_init;

	GPIO_StructInit(&gpio_init);
	USART_StructInit(&usart_init);
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpio_init);

	usart_init.USART_BaudRate = 9600;
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart_init.USART_Parity = USART_Parity_No;
	usart_init.USART_StopBits = USART_StopBits_1;
	usart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &usart_init);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	nvic_init.NVIC_IRQChannel = USART3_IRQn;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&nvic_init);

	USART_Cmd(USART3, ENABLE);
}

void queue_push_string(queue_t * q, const char * string, const uint8_t length)
{
	int i;
	for(i = 0; i < length; i++)
	{
		queue_push(q, string[i]);
	}
}

void uart_send(char *q)
{
	queue_push_string(&queue_sender, q, strlen(q));
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void uart_receive()
{
	b_receive_done = 0;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	while(b_receive_done == 0);
}

void from_receive_to_send(queue_t * send, queue_t * receive)
{
	uint8_t item;
	uint8_t b_success = 0;
	while(queue_is_empty(receive) == 0)
	{
		item = queue_pop(receive, &b_success);
		queue_push(send, item);
	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

int get_data(queue_t q)
{
	char temp[100] = {NULL};
	int i = 0;
	
	while (queue_is_empty(&q) == 0)
	{
		temp[i] = (char)(q.items[i]);
		q.capacity--;
		q.items[i] = q.items[i+1];
		i++;
	}
	
	return atoi(temp);
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
