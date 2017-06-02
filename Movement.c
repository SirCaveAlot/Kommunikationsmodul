/*
 * Movement.c
 *
 * Created: 4/20/2017 2:08:41 PM
 *  Author: jakpa844
 * Code written by Jakob Palm 
 *
 * This file contains the movement queue for movement decisions, which are sent to the control module.
 */ 

#include <avr/io.h>
#include "Movement.h"
#include <stdbool.h>

/* Queue structure */
#define MOVEMENT_QUEUE_ELEMENTS 50
#define MOVEMENT_QUEUE_SIZE (MOVEMENT_QUEUE_ELEMENTS + 1)
volatile uint8_t Movement_Queue[MOVEMENT_QUEUE_SIZE];
volatile int Movement_Queue_In, Movement_Queue_Out;
volatile int Movement_queue_length = 0;

// Initializes the queue.
void Movement_Queue_Init(void)
{
    Movement_Queue_In = Movement_Queue_Out = Movement_queue_length = 0;
}

// Inserts a uint8_t into the queue
void Movement_Queue_Put(uint8_t new)
{
    Movement_Queue[Movement_Queue_In] = new;

	Movement_queue_length++;

    Movement_Queue_In = (Movement_Queue_In + 1) % MOVEMENT_QUEUE_SIZE;
}

// Checks if queue is full
bool Movement_queue_full()
{
	return Movement_queue_length == MOVEMENT_QUEUE_ELEMENTS;
}

// Checks if queue is empty
bool Movement_queue_empty()
{
	return Movement_queue_length == 0;
}

// Pulls the first uint8_t from the queue
void Movement_Queue_Get(uint8_t *old)
{
    *old = Movement_Queue[Movement_Queue_Out];

	Movement_queue_length--;

	Movement_Queue_Out = (Movement_Queue_Out + 1) % MOVEMENT_QUEUE_SIZE;
}


