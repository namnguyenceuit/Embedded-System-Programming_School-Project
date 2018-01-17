#include "main.h"
#include "queue.h"
#include "stdlib.h"
#include "stdio.h"

#define STR_MAX_LENGTH	100
#define TC_BIT			0x0040
#define TC_SHIFT_LEFT	6
#define RXNE_BIT		0x0020
#define RXNE_SHIFT_LEFT	5

void uart_interrupt_my_init(void);
void uart_my_send(void);
void uart_my_receive(void);
void from_receive_to_send(queue_t * send, queue_t * receive);
void queue_push_string(queue_t * Q, const char * string, const uint8_t length);
int get_data(queue_t q);
void home_screen_option(void);
void option1(void);
void option2(void);

extern volatile uint8_t b_receive_done;
// extern variable declare for using option
extern volatile uint8_t msgSend;

extern queue_t queue_sender;
extern queue_t queue_receiver;

char* choose = "Choose your option (1, 2, ..): \n";
char* op1 = 		"1. Student info\n";
char* op2 = 	  "2. Basic operation\n";
char* option3 = "3. Simple led\n";
char* option4 = "4. Advance led\n";
char* option5 = "5. Audio\n";
char* useroption = "Your option: ";
char* escape = "ESC: return previous menu\n";
char* newline = "\n";


int main(){
	
	queue_init(&queue_sender);
	queue_init(&queue_receiver);
	
	// Uart interrupt init
	uart_interrupt_my_init();

	for(;;){
		// Send option & wait for user input
		home_screen_option();
		from_receive_to_send(&queue_sender, &queue_receiver);
		uart_my_send();
		queue_push_string(&queue_sender, newline, strlen(newline));
		uart_my_send();
		
		// Check input option
		switch (msgSend)
		{
			case '1':
				option1();
				break;
			case '2':
				option2();
				break;
			default:
				home_screen_option();
		}
	}
	return 0;
}

void option1()
{
	char* id = "ID: 14520555\n";
	char* name = "Full name: Nguyen Thanh Nam\n";
	
	queue_push_string(&queue_sender, newline, strlen(newline));
	queue_push_string(&queue_sender, op1, strlen(op1));
	queue_push_string(&queue_sender, id, strlen(id));
	queue_push_string(&queue_sender, name, strlen(name));
	queue_push_string(&queue_sender, escape, strlen(escape));
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();
	uart_my_receive();
}

void option2()
{
	char* num1 = "Operand 1: ";
	char* num2 = "Operand 2: ";
	char* txtresult = "Result: ";
	
	// variables for calculation
	int operand1;
	int operand2;
	int sum;
	
	// variables for showing result
	char* result;
	char sum_convert[100];
	
	// queue for get data input
	queue_t queue_get_data;
	
	queue_push_string(&queue_sender, newline, strlen(newline));
	queue_push_string(&queue_sender, op2, strlen(op2));
	
	/* Process for operand 1	*/
	queue_push_string(&queue_sender, num1, strlen(num1));
	uart_my_send();
	uart_my_receive();
	
	// Get data input
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);			
	uart_my_send();					
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();
	
	// Convert to int
	operand1 = get_data(queue_get_data);					
	
	
	// Process for operand 2
	queue_push_string(&queue_sender, num2, strlen(num2));
	uart_my_send();
	uart_my_receive();
	
	// Get data input	
	queue_get_data = queue_receiver;
	
	// Print to terminal	
	from_receive_to_send(&queue_sender, &queue_receiver);
	uart_my_send();
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();			
	
	// Convert to int
	operand2 = get_data(queue_get_data);
	
	// Cal sum
	sum = operand1 + operand2;
	
	// Convert back to string
	sprintf(sum_convert,"%d",sum);
	result = sum_convert;
	
	// Print result
	queue_push_string(&queue_sender, txtresult, strlen(txtresult));
	queue_push_string(&queue_sender, result, strlen(result));
	queue_push_string(&queue_sender, newline, strlen(newline));
	
	queue_push_string(&queue_sender, escape, strlen(escape));
	
	queue_push_string(&queue_sender, newline, strlen(newline));
	uart_my_send();
	uart_my_receive();
}

void home_screen_option()
{
	queue_push_string(&queue_sender, choose, strlen(choose));
	queue_push_string(&queue_sender, op1, strlen(op1));
	queue_push_string(&queue_sender, op2, strlen(op2));
	queue_push_string(&queue_sender, option3, strlen(option3));
	queue_push_string(&queue_sender, option4, strlen(option4));
	queue_push_string(&queue_sender, option5, strlen(option5));
	queue_push_string(&queue_sender, escape, strlen(escape));
	queue_push_string(&queue_sender, useroption, strlen(useroption));
	uart_my_send();
	uart_my_receive();
}

void uart_interrupt_my_init()
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

void queue_push_string(queue_t * Q, const char * string, const uint8_t length)
{
	int i;
	for(i = 0; i < length; i++)
	{
		queue_push(Q, string[i]);
	}
}

void uart_my_send()
{
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void uart_my_receive()
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
