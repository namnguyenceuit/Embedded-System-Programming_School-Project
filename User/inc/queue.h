/** @file queue.h
*
* @brief Header file of the Queue module
*
* @par
@ COPYRIGHT NOTICE: Chung Vinh Kien
* All rights reserved.
*/

#ifndef _QUEUE_H
#define _QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif
	

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported constants --------------------------------------------------------*/                                      
#define QUEUE_MAX_COUNT 	255              /*!< Queue's array maximum items */

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/**
  * @brief Queue structure
  */

typedef struct
{
	uint8_t capacity;                        /*!< Specifies counting number of items in queue */ 
	uint8_t items[QUEUE_MAX_COUNT];          /*!< Queue's array */
} queue_t;

/* Exported functions ------------------------------------------------------- */

void queue_init(queue_t * Q);                                             /*!< Initialization function */
uint8_t queue_is_empty(const queue_t * Q);                                /*!< Check if queue is empty */
uint8_t queue_is_full(const queue_t * Q);                                 /*!< Check if queue is full */
uint8_t queue_push(queue_t * Q, const uint8_t data);                      /*!< Push data to queue */
uint8_t queue_pop(queue_t * Q, uint8_t * success);                      /*!< Popup data from queue */
uint8_t queue_peek(const queue_t * Q, uint8_t * success);               /*!< Peek the front item of queue */


#ifdef __cplusplus
}
#endif

#endif /* _QUEUE_H */
