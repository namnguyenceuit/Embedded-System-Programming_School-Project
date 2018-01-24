#include "basic_operation.h"

int chooseOP2;
char* c_result;
char a_result[100];	
int i_result;

char* OPTION2 = "2. Basic operation( a,b,..):\n\
								a. Plus\n\
								b. Subtract\n\
								c. Multiply\n\
								d. Divide\n\
								e. Module\n\
								ESC: return previous menu\n\
								Your option: ";

void basic_operation()
{
	// Home screen option 2
	queue_receiver.capacity = 0;
	uart_send("\n");
	uart_send(OPTION2);
	uart_receive();	
	
	// Print to terminal
	from_receive_to_send(&queue_sender, &queue_receiver);					
	uart_send("\n");
	
	// get data to check if wrong input
	chooseOP2 = queue_receiver.items[0];
	
	// Check if user input two char like "aa" or "bb" or not a char in option like "f"
	//							Enter					 a							e
	while (chooseOP2 < 97 || chooseOP2 > 101)
	{
		if (chooseOP2 == 27)
			return;
		uart_send("Not a option!\n");
		uart_send("\n");
		uart_send(OPTION2);
		uart_receive();
		
		// Print to terminal
		from_receive_to_send(&queue_sender, &queue_receiver);					
		uart_send("\n");
	
		chooseOP2 = queue_receiver.items[0];
	}
		
		//TODO: ESC doesn't show the previous menu
	//TODO: check if input_operand not a number
		switch (chooseOP2)
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

void option2_input(int * a, int *b)
{	
	char* NUM1_REQUEST = "Operand 1: ";
	char* NUM2_REQUEST = "Operand 2: ";
	
	
	//Process for operand 1	
	uart_send("\n");
	uart_send(NUM1_REQUEST);
	get_input_number(a);
	
	
	uart_send("\n");
	// Process for operand 2
	uart_send(NUM2_REQUEST);
	get_input_number(b);
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
	
	
	uart_send("\n");
	uart_send(txtResult);
	uart_send(result);	
	do
	{
		uart_send("\n");
		uart_send("ESC: return previous menu\n");;
		uart_receive();
		queue_receiver.capacity = 0;
				
		chooseOP2 = queue_receiver.items[0];
	}while (chooseOP2 != 27);
	
	basic_operation();
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
