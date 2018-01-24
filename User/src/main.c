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

void uart_interrupt_init(void);
void SPI_Configuration(void);
void Delay(__IO uint32_t nCount);

void uart_send(char *q);
void uart_receive(void);
void from_receive_to_send(queue_t * send, queue_t * receive);
void queue_push_string(queue_t * q, const char * string, const uint8_t length);
int get_data(queue_t q);
void option2_input_operand(int *a, int *b);
void get_input_operand(int *operand);
void option2_print_result(char *result);
void plus(void);
void subtract(void);
void multiply(void);
void divide(void);
void module(void);
void blink(void);

void student_info(void);
void basic_operation(void);
void simple_led(void);
void advance_led(void);

volatile uint8_t SPI_data_get;
volatile uint8_t SPI_data_get2;
extern volatile uint8_t b_receive_done;

extern queue_t queue_sender;
extern queue_t queue_receiver;

char* MAIN_MENU = "Choose your option (1, 2, ..): \n\
								1. Student info\n\
								2. Basic operation\n\
								3. Simple led\n\
								4. Advance led\n\
								5. Audio\n\
								ESC: return previous menu\n\
								Your option: ";

char* OPTION1 = "\n1. Student info\n\
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

char* OPTION3 = "3. Simple led\n\
								a. On Green\n\
								b. Off Green\n\
								c. Blink led\n\
								ESC: return previous menu\n\
								Your option: ";

char* NEWLINE = "\n";

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
	SPI_Configuration();
	SS_DISABLE;

	SPI_I2S_SendData(SPI1, 99);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	SS_ENABLE;
  //Delay(1000);
	
	if (SPI_data_get2 == 99)
	{
		uart_send("AAAAAAAAA");
	}
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
		
	/* Process for times input	*/
	queue_push_string(&queue_sender, NEWLINE, strlen(NEWLINE));
	uart_send(INPUT_TIMES);
	uart_receive();
	
	// Get data input
	queue_get_data = queue_receiver;
	
	
	choose = queue_get_data.items[0];
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);								
	uart_send(NEWLINE);
	
	// Convert to int
	// TODO: add timer
	times = get_data(queue_get_data);
	for (i = 0; i < times; i++)
	{
			STM_EVAL_LEDOn(LED3);
			STM_EVAL_LEDOn(LED4);
			for(j = 0; j < 3000000; j++);
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOff(LED4);
			for(j = 0; j < 3000000; j++);
	}
	
	uart_send(NEWLINE);
}
void plus()
{
	// variables for calculation
	int operand1;
	int operand2;
	
	option2_input_operand(&operand1, &operand2);
	
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

	option2_input_operand(&operand1, &operand2);
	
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

	option2_input_operand(&operand1, &operand2);
	
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

	option2_input_operand(&operand1, &operand2);
	
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

	option2_input_operand(&operand1, &operand2);
	
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
	char* ESC = "ESC: return previous menu\n";
	
	uart_send(txtResult);
	uart_send(result);
	uart_send(NEWLINE);
	uart_send(ESC);;
	uart_send(NEWLINE);
	uart_receive();
	
	queue_receiver.capacity = 0;
	choose = queue_receiver.items[0];
	
	while (choose != 27)
	{
		uart_send(ESC);
		queue_receiver.capacity = 0;
		uart_receive();
	
		choose = queue_receiver.items[0];
	}
	basic_operation();
}

void option2_input_operand(int * a, int *b)
{	
	char* NUM1_REQUEST = "Operand 1: ";
	char* NUM2_REQUEST = "Operand 2: ";
	
	
	//Process for operand 1	
	uart_send(NEWLINE);
	uart_send(NUM1_REQUEST);
	
	get_input_operand(a);
	
	// Process for operand 2
	uart_send(NUM2_REQUEST);
	get_input_operand(b);
}

void get_input_operand(int *operand)
{
	int i = 0;
	char data[100];
	for(;;)
	{
		queue_receiver.capacity = 0;
		uart_receive();
		
		// Get data input
		queue_get_data = queue_receiver;
		data[i] = queue_get_data.items[0];
		
		// Print to terminal
		from_receive_to_send(&queue_sender, &queue_receiver);
		if (data[i] == 10 || data[i] == 27)
			break;
		i++;
	}
	*operand = atoi(data);
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
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
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

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
