#include "main.h"
char* MAIN_MENU = "\n----------MAIN MENU----------\n\
										Choose your option (1, 2, ..): \n\
										1. Student info\n\
										2. Basic operation\n\
										3. Simple led\n\
										4. Advance led\n\
										5. Timer\n\
										ESC: return previous menu\n\
										Your option: ";

int main(){
	int choose;
	queue_init(&queue_sender);
	queue_init(&queue_receiver);
	
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
		
		from_receive_to_send(&queue_sender, &queue_receiver);
		uart_send("\n");
		
		// Check input option
		// TODO: check if user input orther keys
		choose = queue_receiver.items[0];
		
		switch (choose)
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
			case '4':
				advance_led();
				break;
			case '5':
				timer_counter();
				break;
			default:
				break;
		}
	}
	return 0;
}

