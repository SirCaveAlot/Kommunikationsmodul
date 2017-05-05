

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include "SPI_slave.h"

/* Queue structure */
#define QUEUE_ELEMENTS 12000
#define QUEUE_SIZE (QUEUE_ELEMENTS + 1)
volatile uint8_t Queue[QUEUE_SIZE];
volatile int Queue_In, Queue_Out;
volatile int queue_length = 0;

void spi_init_slave ()
{
  DDRB=(1<<DDB6);               //MISO as OUTPUT
  SPDR='D';
  SPCR=(1<<SPIE)|(1<<SPE);       //Enable SPI && interrupt enable bit
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
    Queue[Queue_In] = new;
	
	queue_length++;

    Queue_In = (Queue_In + 1) % QUEUE_SIZE;
}

void Queue_Get(uint8_t *old)
{
    *old = Queue[Queue_Out];

	queue_length--;

	Queue_Out = (Queue_Out + 1) % QUEUE_SIZE;
}

bool Queue_full()
{
	return queue_length == 12000;
}

bool Queue_empty()
{
	return queue_length == 0;
}

bool Queue_contains(int i)
{
	return queue_length >= i;
}

uint8_t Queue_Peek(int i)
{
	int peek_value = (Queue_Out + i) % QUEUE_SIZE;
	return Queue[peek_value];
}

void Queue_Remove()
{
	Queue_Out = (Queue_Out + 1) % QUEUE_SIZE;
}
