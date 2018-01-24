#include "simple_led.h"

int chooseOP3;
char* OPTION3 = "3. Simple led:\n\
								a. On Green\n\
								b. Off Green\n\
								c. Blink led\n\
								ESC: return previous menu\n\
								Your option: ";

void simple_led()
{
	uart_send("\n");
	uart_send(OPTION3);
	queue_receiver.capacity = 0;
	uart_receive();	
	
	from_receive_to_send(&queue_sender, &queue_receiver);				
	uart_send("\n");
	
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	
	// get data to check if wrong input
	chooseOP3 = queue_receiver.items[0];
	
	// Check if user input two char like "aa" or "bb" or not a char in option like "f"
	// TODO: define variables' name
	//    					 a							c
	while (chooseOP3 < 97 || chooseOP3 > 99)
	{
		if (chooseOP3 == 27)
			return;
		uart_send("Not a option!\n");
		uart_send("\n");
		uart_send(OPTION3);
		uart_receive();
		
		// Print to terminal
		from_receive_to_send(&queue_sender, &queue_receiver);					
		uart_send("\n");
	
		chooseOP3 = queue_receiver.items[0];
	}
	
	switch (chooseOP3)
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

void blink()
{
	int i, j, times;
	char* INPUT_TIMES = "Blink LED! Input times you want to blink: ";
	char* BLINKING = "Blinking.... Pls wait & look at the board...\n";
		
	/* Process for times input	*/
	queue_push_string(&queue_sender, "\n", strlen("\n"));
	uart_send(INPUT_TIMES);
	get_input_number(&times);
	uart_send("\n");
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
