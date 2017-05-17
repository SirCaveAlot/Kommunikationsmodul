/*
 * Movement.c
 *
 * Created: 4/20/2017 2:08:41 PM
 *  Author: jakpa844
 */ 

#include <avr/io.h>
#include "Movement.h"
#include <stdbool.h>

/* Queue structure */
#define MOVEMENT_QUEUE_ELEMENTS 50
#define MOVEMENT_QUEUE_SIZE (MOVEMENT_QUEUE_ELEMENTS + 1)
uint8_t Movement_Queue[MOVEMENT_QUEUE_SIZE];
int Movement_Queue_In, Movement_Queue_Out;
int Movement_queue_length = 0;


/* Very simple queue
 * These are FIFO queues which discard the new data when full.
 *
 * Queue is empty when in == out.
 * If in != out, then 
 *  - items are placed into in before incrementing in
 *  - items are removed from out before incrementing out
 *
 * The queue will hold QUEUE_ELEMENTS number of items before the
 * calls to QueuePut fail.
 */

void Movement_Queue_Init(void)
{
    Movement_Queue_In = Movement_Queue_Out = Movement_queue_length = 0;
}

void Movement_Queue_Put(uint8_t new)
{
    Movement_Queue[Movement_Queue_In] = new;

	Movement_queue_length++;

    Movement_Queue_In = (Movement_Queue_In + 1) % MOVEMENT_QUEUE_SIZE;
}

bool Movement_queue_full()
{
	return Movement_queue_length == MOVEMENT_QUEUE_ELEMENTS;
}

bool Movement_queue_empty()
{
	return Movement_queue_length == 0;
}

void Movement_Queue_Get(uint8_t *old)
{
    *old = Movement_Queue[Movement_Queue_Out];

	Movement_queue_length--;

	Movement_Queue_Out = (Movement_Queue_Out + 1) % MOVEMENT_QUEUE_SIZE;
}


