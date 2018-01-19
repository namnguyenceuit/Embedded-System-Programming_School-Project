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
	uint8_t result;
	if(0 == Q->capacity)
	{
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}


/**
  * @brief  Check if queue is full or not
  */
uint8_t queue_is_full(const queue_t * Q)
{
	uint8_t result;
	if(QUEUE_MAX_COUNT == Q->capacity)
	{
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

/**
  * @brief  Push data to queue
  */
uint8_t queue_push(queue_t * Q, const uint8_t data)
{
	uint8_t success;
	if(1 == queue_is_full(Q))
	{
		success = 0;
	}
	else
	{
		Q->items[Q->capacity] = data;
		Q->capacity++;
		success = 1;
	}
	return success;
}

/**
  * @brief  Popup data from queue
  */
uint8_t queue_pop(queue_t * Q, uint8_t * success) 
{
	int i;
	uint8_t data;
	if(1 == queue_is_empty(Q))
	{
		*(success) = 0;
	}
	else
	{
		data = Q->items[0];
		Q->capacity--;
		
		for(i = 0; i < Q->capacity; i++)
		{
			Q->items[i] = Q->items[i + 1];
		}
		*(success) = 1;
	}
	return data;
}

/**
  * @brief  Peek the front item of queue
  */
uint8_t queue_peek(const queue_t * Q, uint8_t * success)
{
	uint8_t data;
	if(1 == queue_is_empty(Q))
	{
		*(success) = 0;
	}
	else
	{
		data = Q->items[0];
		*(success) = 1;
	}
	return data;
}

/* Private functions ---------------------------------------------------------*/
