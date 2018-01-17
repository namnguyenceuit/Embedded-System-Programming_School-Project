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
void home_screen_option(void);
void option1(void);


extern volatile uint8_t b_receive_done;
extern volatile uint8_t msgSend;

extern queue_t queue_sender;
extern queue_t queue_receiver;
queue_t queue_test;

char* choose = "Choose your option (1, 2, ..): \n";
char* op1 = 		"1. Student info\n";
char* op2 = 	  "2. Basic operation\n";
char* option3 = "3. Simple led\n";
char* option4 = "4. Advance led\n";
char* option5 = "5. Audio\n";
char* useroption = "Your option: ";
char* newline = "\n";

char* num1 = "Operand 1: ";
char* num2 = "Operand 2: ";
char* sum3 = "Result: ";
	
char* escape = "ESC: return previous menu\n";
char* id = "ID: 14520555\n";
char* name = "Full name: Nguyen Thanh Nam\n";
int a[100];
char e[100];
int b;
int c;
int sum;
char* test;
char test2[100];
int i = 0;
int j = 0;


int main(){
	queue_init(&queue_sender);
	queue_init(&queue_receiver);

	uart_interrupt_my_init();

	for(;;){
		home_screen_option();
		from_receive_to_send(&queue_sender, &queue_receiver);
		uart_my_send();
		queue_push_string(&queue_sender, newline, strlen(newline));
		uart_my_send();
		
		if (msgSend == 27)
		{
			home_screen_option();
		}
		
		//just for testing
		//TODO: write in a function, using switch case
		if (msgSend == 49) 
		{
			option1();
		}
		
		
		if (msgSend == 50)
		{
			queue_push_string(&queue_sender, newline, strlen(newline));
			queue_push_string(&queue_sender, op2, strlen(op2));
			
			queue_push_string(&queue_sender, num1, strlen(num1));
			uart_my_send();
			uart_my_receive();
			
			queue_test = queue_receiver;
			
			from_receive_to_send(&queue_sender, &queue_receiver);			
			uart_my_send();					
			queue_push_string(&queue_sender, newline, strlen(newline));
			uart_my_send();
			
					
			while (queue_is_empty(&queue_test) == 0)
			{
				e[i] = (char)(queue_test.items[i]);
				queue_test.capacity--;
				queue_test.items[i] = queue_test.items[i+1];
				i++;
			}
			
			b = atoi(e);
						
			/*
			if (e[0] == '2')
			{
				queue_push_string(&queue_sender, "aa", strlen("aa"));
				uart_my_send();
			} */

			queue_push_string(&queue_sender, num2, strlen(num2));
			uart_my_send();
			uart_my_receive();
			
			queue_test = queue_receiver;
			
			from_receive_to_send(&queue_sender, &queue_receiver);
			uart_my_send();
			queue_push_string(&queue_sender, newline, strlen(newline));
			uart_my_send();			
			
			while (queue_is_empty(&queue_test) == 0)
			{
				e[j] = queue_test.items[j];
				queue_test.capacity--;
				queue_test.items[j] = queue_test.items[j+1];
				j++;
			}
			
			c = atoi(e);	
			
			sum = b + c;
			
			/*
			if (sum == 7)
			{
				queue_push_string(&queue_sender, "aa", strlen("aa"));
				uart_my_send();
			} */
			
			sprintf(test2,"%d",sum);
			test = test2;
			queue_push_string(&queue_sender, sum3, strlen(sum3));
			queue_push_string(&queue_sender, test, strlen(test));
			uart_my_send();
			uart_my_receive();
		} 
	}
	return 0;
}



void option1()
{
	queue_push_string(&queue_sender, newline, strlen(newline));
	queue_push_string(&queue_sender, op1, strlen(op1));
	queue_push_string(&queue_sender, id, strlen(id));
	queue_push_string(&queue_sender, name, strlen(name));
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
