/*
 * Movement.c
 *
 * Created: 4/20/2017 2:08:41 PM
 *  Author: jakpa844
 */ 

#include <avr/io.h>
#include "Movement.h"

/* Queue structure */
#define MOVEMENT_QUEUE_ELEMENTS 100
#define MOVEMENT_QUEUE_SIZE (MOVEMENT_QUEUE_ELEMENTS + 1)
uint8_t Movement_Queue[MOVEMENT_QUEUE_SIZE];
int Movement_Queue_In, Movement_Queue_Out;


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
    Movement_Queue_In = Movement_Queue_Out = 0;
}

void Movement_Queue_Put(uint8_t new)
{
    if(Movement_Queue_In == Movement_Queue_Out && Movement_Queue[0] != 0)
    {
        return; /* Queue Full*/
    }

    Movement_Queue[Movement_Queue_In] = new;

    Movement_Queue_In = (Movement_Queue_In + 1) % MOVEMENT_QUEUE_SIZE;

   // return 0; // No errors
}

void Movement_Queue_Get(uint8_t *old)
{
    if(Movement_Queue_In == Movement_Queue_Out && Movement_Queue[0] == 0)
    {
        *old = 0;
		return; /* Queue Empty - nothing to get*/
    }

    *old = Movement_Queue[Movement_Queue_Out];
	
	Movement_Queue[Movement_Queue_Out] = 0;

	Movement_Queue_Out = (Movement_Queue_Out + 1) % MOVEMENT_QUEUE_SIZE;

    //return 0; // No errors
}


