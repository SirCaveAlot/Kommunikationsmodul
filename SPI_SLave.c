

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "SPI_slave.h"

/* Queue structure */
#define QUEUE_ELEMENTS 100
#define QUEUE_SIZE (QUEUE_ELEMENTS + 1)
uint8_t Queue[QUEUE_SIZE];
int Queue_In, Queue_Out;

void spi_init_slave ()
{
  DDRB=(1<<DDB6);               //MISO as OUTPUT
  SPCR=(1<<SPIE)|(1<<SPE);       //Enable SPI && interrupt enable bit
  SPDR='D';
}


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


void Queue_Init(void)
{
    Queue_In = Queue_Out = 0;
}

void Queue_Put(uint8_t new)
{
    if(Queue_In == Queue_Out && Queue[0] != 0)
    {
        return; /* Queue Full*/
    }

    Queue[Queue_In] = new;

    Queue_In = (Queue_In + 1) % QUEUE_SIZE;

   // return 0; // No errors
}

void Queue_Get(uint8_t *old)
{
    if(Queue_In == Queue_Out && Queue[0] == 0)
    {
        return; /* Queue Empty - nothing to get*/
    }

    *old = Queue[Queue_Out];
	
	Queue[Queue_Out] = 0;

	//int statement = QueueIn - QueueOut;
/*
    for(int i = 0; i < statement; i++)
	{
		Queue[QueueOut + i] = Queue[QueueOut + i + 1];
	}*/

	Queue_Out = (Queue_Out + 1) % QUEUE_SIZE;

    //return 0; // No errors
}
