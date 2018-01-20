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

void uart_interrupt_init(void);
void uart_send(char *q);
void uart_receive(void);
void from_receive_to_send(queue_t * send, queue_t * receive);
void queue_push_string(queue_t * q, const char * string, const uint8_t length);
int get_data(queue_t q);
void option2_input_operand(int *a, int *b);
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
		// Send option & wait for user input
		uart_send(MAIN_MENU);
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
			default:
				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
				b_receive_done = 1;
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
	int check;
	// Home screen option 2
	uart_send(NEWLINE);
	uart_send(OPTION2);
	uart_receive();	
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);					
	uart_send(NEWLINE);
	
	// get data to check if wrong input
	choose = queue_get_data.items[0];
	check = queue_get_data.items[1];
	
	// Check if user input two char like "aa" or "bb" or not a char in option like "f"
	//							Enter					 a							e
	while (check != 13 || choose < 97 || choose > 101)
	{
		uart_send("Not a option!\n");
		uart_send(NEWLINE);
		uart_send(OPTION2);
		uart_receive();
		
		queue_get_data = queue_receiver;
		// Print to terminal
		from_receive_to_send(&queue_sender, &queue_receiver);					
		uart_send(NEWLINE);
	
		choose = queue_get_data.items[0];
		check = queue_get_data.items[1];
	}
		
		//TODO: ESC doesn't show the previous menu
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
	
	choose = queue_get_data.items[0];
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	
	switch (choose)
	{
		case 'a':
			STM_EVAL_LEDOn(LED3);
			break;
		case 'b':
			STM_EVAL_LEDOff(LED3);
			break;
		case 'c':
			blink();
			break;
		default:
			uart_send(NEWLINE);
			uart_send(OPTION3);
	}
	
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
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);								
	uart_send(NEWLINE);
	
	// Convert to int
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
}

void option2_input_operand(int *a, int *b)
{	
	int operand1, operand2;
	char* NUM1_REQUEST = "Operand 1: ";
	char* NUM2_REQUEST = "Operand 2: ";
	
	/* Process for operand 1	*/
	uart_send(NEWLINE);
	uart_send(NUM1_REQUEST);
	uart_receive();
	
	// Get data input
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);						
	uart_send(NEWLINE);
	
	// Convert to int
	operand1 = get_data(queue_get_data);
	*a = operand1;
	
	
	// Process for operand 2
	uart_send(NUM2_REQUEST);
	uart_receive();
	
	// Get data input	
	queue_get_data = queue_receiver;
	
	// Print to terminal	
	from_receive_to_send(&queue_sender, &queue_receiver);
	uart_send(NEWLINE);
	
	// Convert to int
	operand2 = get_data(queue_get_data);
	*b = operand2;
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
	char temp[100];
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
