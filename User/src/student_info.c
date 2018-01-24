#include "student_info.h"

char* OPTION1 = "\n1. Student info:\n\
								ID: 14520555\n\
								Full name: Nguyen Thanh Nam\n\
								ESC: return previous menu\n";

void student_info()
{
	uart_send(OPTION1);
	uart_send("\n");
	uart_receive();	
}

