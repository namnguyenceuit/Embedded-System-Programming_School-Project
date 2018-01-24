#include "main.h"

void uart_interrupt_init(void);
void queue_push_string(queue_t * q, const char * string, const uint8_t length);
void uart_send(char *q);
void uart_receive(void);
void from_receive_to_send(queue_t * send, queue_t * receive);
