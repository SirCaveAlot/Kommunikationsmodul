/*
 * queue.c
 *
 * Created: 4/7/2017 10:18:08 AM
 *  Author: jakpa844
 */ 

/* Very simple queue
 * These are FIFO queues which discard the new data when full.
 *
 * Queue is empty when in == out.
 * If in != out, then 
 *  - items are placed into in before incrementing in
 *  - items are removed from out before incrementing out
 * Queue is full when in == (out-1 + QUEUE_SIZE) % QUEUE_SIZE;
 *
 * The queue will hold QUEUE_ELEMENTS number of items before the
 * calls to QueuePut fail.
 */

#include "queue.h"
#include <avr/io.h>

/* Queue structure */
#define QUEUE_ELEMENTS 25
#define QUEUE_SIZE (QUEUE_ELEMENTS + 1)
uint8_t Queue[QUEUE_SIZE];
int QueueIn, QueueOut;

void QueueInit(void)
{
    QueueIn = QueueOut = 0;
}

void QueuePut(uint8_t new)
{
    if(QueueIn == (( QueueOut - 1 + QUEUE_SIZE) % QUEUE_SIZE))
    {
        return; /* Queue Full*/
    }

    Queue[QueueIn] = new;

    QueueIn = (QueueIn + 1) % QUEUE_SIZE;

   // return 0; // No errors
}

void QueueGet(uint8_t *old)
{
    if(QueueIn == QueueOut)
    {
        return; /* Queue Empty - nothing to get*/
    }

    *old = Queue[QueueOut];


	int statement = QueueIn - QueueOut;

    for(int i = 0; i < statement; i++)
	{
		Queue[QueueOut + i] = Queue[QueueOut + i + 1];
	}

	QueueIn = (QueueIn - 1) % QUEUE_SIZE;

    //return 0; // No errors
}