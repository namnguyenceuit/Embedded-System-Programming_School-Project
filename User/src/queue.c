/** @file queue.c
*
* @brief Queue module
*
* @par
* All rights reserved.
*/


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "queue.h"
/* Private typedef -----------------------------------------------------------*/
/* Private constant ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initializes the queue
  */
void queue_init(queue_t * Q)
{
	int i;
	Q->capacity = 0;
	for(i = 0; i < QUEUE_MAX_COUNT; i++)
	{
		Q->items[i] = 0;
	}
}

/**
  * @brief  Check if queue is empty or not
  */
uint8_t queue_is_empty(const queue_t * Q)
{
	uint8_t b_result;
	if(0 == Q->capacity)
	{
		b_result = 1;
	}
	else
	{
		b_result = 0;
	}
	return b_result;
}

void empty_queue(queue_t *Q)
{
	Q->items[0] = 0;
}

/**
  * @brief  Check if queue is full or not
  */
uint8_t queue_is_full(const queue_t * Q)
{
	uint8_t b_result;
	if(QUEUE_MAX_COUNT == Q->capacity)
	{
		b_result = 1;
	}
	else
	{
		b_result = 0;
	}
	return b_result;
}

/**
  * @brief  Push data to queue
  */
uint8_t queue_push(queue_t * Q, const uint8_t data)
{
	uint8_t b_success;
	if(1 == queue_is_full(Q))
	{
		b_success = 0;
	}
	else
	{
		Q->items[Q->capacity] = data;
		Q->capacity++;
		b_success = 1;
	}
	return b_success;
}

/**
  * @brief  Popup data from queue
  */
uint8_t queue_pop(queue_t * Q, uint8_t * b_success) 
{
	int i;
	uint8_t data;
	if(1 == queue_is_empty(Q))
	{
		*(b_success) = 0;
	}
	else
	{
		data = Q->items[0];
		Q->capacity--;
		
		for(i = 0; i < Q->capacity; i++)
		{
			Q->items[i] = Q->items[i + 1];
		}
		*(b_success) = 1;
	}
	return data;
}

/**
  * @brief  Peek the front item of queue
  */
uint8_t queue_peek(const queue_t * Q, uint8_t * b_success)
{
	uint8_t data;
	if(1 == queue_is_empty(Q))
	{
		*(b_success) = 0;
	}
	else
	{
		data = Q->items[0];
		*(b_success) = 1;
	}
	return data;
}

/* Private functions ---------------------------------------------------------*/
