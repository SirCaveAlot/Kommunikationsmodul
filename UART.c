/*
 * CFile1.c
 *
 * Created: 3/29/2017 3:04:47 PM
 *  Author: jakpa844
 */ 

/*
 * Kommunikationsmodul.c
 *
 * Created: 3/26/2017 11:35:04 AM
 *  Author: jakpa844
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "UART.h"

#define clkspd 14745600
#define BAUD 115200
#define UBBR clkspd/16/BAUD-1

/* Queue structure */
#define UART_QUEUE_ELEMENTS 20
#define UART_QUEUE_SIZE (UART_QUEUE_ELEMENTS + 1)
volatile uint8_t UART_Queue[UART_QUEUE_SIZE];
uint8_t UART_Queue_In, UART_Queue_Out;

void USART_Init( )
{
	/* Set baud rate */
	UBRR0H = 0;
	UBRR0L = 7;
	
	UBRR1H = 0;
	UBRR1L = 7;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = 0b00000110;
	UCSR1C = 0b00000110;
}

void USART_Transmit(uint8_t uartdata, int port)
{
	if(port == 0)
	{
		/* Wait for empty transmit buffer */
		while (!(UCSR0A & (1<<UDRE0)));
		/* Put data into buffer, sends the data */
		UDR0 = uartdata;
	}
	if(port == 1)
	{
		/* Wait for empty transmit buffer */
		while (!(UCSR1A & (1<<UDRE1)));
		/* Put data into buffer, sends the data */
		UDR1 = uartdata;
	}
}

void Interrupt_Init()
{
	UCSR0B |= (1<<RXCIE0);
	UCSR1B |= (1<<RXCIE1);
	//sei();
}

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

void UART_Queue_Init(void)
{
    UART_Queue_In = UART_Queue_Out = 0;
}

void UART_Queue_Put(uint8_t new)
{
    if(UART_Queue_In == UART_Queue_Out && UART_Queue[0] != 0)
    {
        return; /* Queue Full*/
    }

    UART_Queue[UART_Queue_In] = new;

    UART_Queue_In = (UART_Queue_In + 1) % UART_QUEUE_SIZE;

   // return 0; // No errors
}

void UART_Queue_Get(uint8_t *old)
{
    if(UART_Queue_In == UART_Queue_Out && UART_Queue[0] == 0)
    {
		*old = 0;
        return; /* Queue Empty - nothing to get*/
    }

    *old = UART_Queue[UART_Queue_Out];
	
	UART_Queue[UART_Queue_Out] = 0;

	UART_Queue_Out = (UART_Queue_Out + 1) % UART_QUEUE_SIZE;

    //return 0; // No errors
}