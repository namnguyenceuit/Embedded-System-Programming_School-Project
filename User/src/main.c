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

void uart_interrupt_my_init(void);
void uart_my_send(void);
void uart_my_send2(char *q);
void uart_my_receive(void);
void from_receive_to_send(queue_t * send, queue_t * receive);
void queue_push_string(queue_t * Q, const char * string, const uint8_t length);
int get_data(queue_t q);
void input_operand(int *a, int *b);
void op2_print_result(char *result);
void plus(void);
void subtract(void);
void multiply(void);
void divide(void);
void module(void);
void blink(void);
void menu_home(void);
void menu_option2(void);
void menu_option3(void);
void student_info(void);
void basic_operation(void);
void simple_led(void);

extern volatile uint8_t b_receive_done;
// extern variable declare for using option
extern volatile uint8_t msgSend;

extern queue_t queue_sender;
extern queue_t queue_receiver;

char* MAIN_MENU = "Choose your option (1, 2, ..): \n\
								1. Student info\n\
								2. Basic operation( a,b,..):\n\
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


int main(){
	
	queue_init(&queue_sender);
	queue_init(&queue_receiver);
	
	// Uart interrupt init
	uart_interrupt_my_init();

	for(;;){
		// Send option & wait for user input
		menu_home();
		from_receive_to_send(&queue_sender, &queue_receiver);
		uart_my_send();
		uart_my_send2(NEWLINE);
		
		// Check input option
		// TODO: add other functions
		// TODO: check if user input orther keys
		switch (msgSend)
		{
			case '1':
				student_info();
				break;
			case '2':
				basic_operation();
				break;
			case '3':
				simple_led();
				break;
			default:
				menu_home();
		}
	}
	return 0;
}

void menu_home()
{
	uart_my_send2(MAIN_MENU);
	uart_my_receive();
}

void menu_option2()
{
	uart_my_send2(NEWLINE);
	uart_my_send2(OPTION2);
}

void menu_option3()
{
	uart_my_send2(NEWLINE);
	uart_my_send2(OPTION3);
}

void student_info()
{
	uart_my_send2(OPTION1);
	uart_my_send2(NEWLINE);
	uart_my_receive();
}

void basic_operation()
{
	queue_t queue_get_data;
	int op_operator;
	
	// Home screen option 2
	menu_option2();
	uart_my_receive();	
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);			
	uart_my_send();					
	uart_my_send2(NEWLINE);
	
	op_operator = queue_get_data.items[0];
	
	//TODO: ESC doesn't show the previous menu
	switch (op_operator)
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
		default:
			menu_option2();
	}	
}

void simple_led()
{
	queue_t queue_get_data;
	int op_operator;
	
	menu_option3();
	uart_my_receive();	
	queue_get_data = queue_receiver;
	
	from_receive_to_send(&queue_sender, &queue_receiver);
	uart_my_send();					
	uart_my_send2(NEWLINE);
	
	op_operator = queue_get_data.items[0];
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	
	
	switch (op_operator)
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
			menu_option3();
	}
	
}

void blink()
{
	int i, j, times;
	char* INPUT_TIMES = "Blink LED! Input times you want to blink: ";
	
	// queue for get data input
	queue_t queue_get_data;
	
	/* Process for times input	*/
	queue_push_string(&queue_sender, NEWLINE, strlen(NEWLINE));
	uart_my_send2(INPUT_TIMES);
	uart_my_receive();
	
	// Get data input
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);			
	uart_my_send();					
	uart_my_send2(NEWLINE);
	
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
	
	uart_my_send2(NEWLINE);
}
void plus()
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char sum_convert[100];	
	int sum;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	sum = operand1 + operand2;
	
	// Convert back to string
	sprintf(sum_convert,"%d",sum);
	result = sum_convert;
	
	// Print result
	op2_print_result(result);
}

void subtract()
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char sub_convert[100];	
	int sub;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	sub = operand1 - operand2;
	
	// Convert back to string
	sprintf(sub_convert,"%d",sub);
	result = sub_convert;
	
	// Print result
	op2_print_result(result);
}

void multiply(void)
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char mul_convert[100];	
	int mul;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	mul = operand1 * operand2;
	
	// Convert back to string
	sprintf(mul_convert,"%d",mul);
	result = mul_convert;
	
	// Print result
	op2_print_result(result);	
}


void divide(void)
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char div_convert[100];	
	float div;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	div = (float)(operand1) / (float)(operand2);
	
	// Convert back to string
	sprintf(div_convert,"%0.2f",div);
	result = div_convert;
	
	// Print result
	op2_print_result(result);	
}

void module(void)
{
	// variables for calculation
	// TODO: uint8_t variables
	int operand1;
	int operand2;
	// variables for showing result
	char* result;
	char mol_convert[100];	
	float mol;

	input_operand(&operand1, &operand2);
	
	// Cal sum
	mol = sqrt((float)(operand1 * operand1) + (float)(operand2 * operand2));
	
	// Convert back to string
	sprintf(mol_convert,"%0.2f",mol);
	result = mol_convert;
	
	// Print result
	op2_print_result(result);	
}

void op2_print_result(char *result)
{
	char* txtResult = "Result: ";	
	char* ESC = "ESC: return previous menu\n";
	
	uart_my_send2(txtResult);
	uart_my_send2(result);
	uart_my_send2(NEWLINE);
	uart_my_send2(ESC);;
	uart_my_send2(NEWLINE);
	uart_my_receive();
}

void input_operand(int *a, int *b)
{	
	int operand1, operand2;
	char* NUM1_REQUEST = "Operand 1: ";
	char* NUM2_REQUEST = "Operand 2: ";
	
	// queue for get data input
	queue_t queue_get_data;
	
	/* Process for operand 1	*/
	uart_my_send2(NEWLINE);
	uart_my_send2(NUM1_REQUEST);
	uart_my_receive();
	
	// Get data input
	queue_get_data = queue_receiver;
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);			
	uart_my_send();					
	uart_my_send2(NEWLINE);
	
	// Convert to int
	operand1 = get_data(queue_get_data);
	*a = operand1;
	
	
	// Process for operand 2
	uart_my_send2(NUM2_REQUEST);
	uart_my_receive();
	
	// Get data input	
	queue_get_data = queue_receiver;
	
	// Print to terminal	
	from_receive_to_send(&queue_sender, &queue_receiver);
	uart_my_send();
	uart_my_send2(NEWLINE);
	
	// Convert to int
	operand2 = get_data(queue_get_data);
	*b = operand2;
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

void uart_my_send2(char *q)
{
	queue_push_string(&queue_sender, q, strlen(q));
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
