/*
 * CFile1.c
 *
 * Created: 3/29/2017 3:04:47 PM
 *  Author: jakpa844
 * Code written by Jakob Palm
 *
 * This file contains the queue for all UART data received by the FireFly modem connected to the PC,
 * aswell as all functions to receive and send data through UART.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "UART.h"

/* Queue structure */
#define UART_QUEUE_ELEMENTS 20
#define UART_QUEUE_SIZE (UART_QUEUE_ELEMENTS + 1)
volatile uint8_t UART_Queue[UART_QUEUE_SIZE];
uint8_t volatile UART_Queue_In, UART_Queue_Out;
volatile int UART_queue_length = 0;

// Initializes the UART busses, both to the FireFly and to the control unit
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

// Transmits uartdata to either the control unit (port = 0) or the firefly (port = 1)
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

// Initializes the interrupt registers
void Interrupt_Init()
{
	UCSR0B |= (1<<RXCIE0);
	UCSR1B |= (1<<RXCIE1);
	//sei();
}

// Initializes the UART queue
void UART_Queue_Init(void)
{
    UART_Queue_In = UART_Queue_Out = 0;
}

// Inserts an uint8_t in the UART queue
void UART_Queue_Put(uint8_t new)
{
    UART_Queue[UART_Queue_In] = new;
	
	UART_queue_length++;

    UART_Queue_In = (UART_Queue_In + 1) % UART_QUEUE_SIZE;
}

// Pulls the first uint8_t from the UART queue
void UART_Queue_Get(uint8_t *old)
{
    *old = UART_Queue[UART_Queue_Out];
	
	UART_queue_length--;

	UART_Queue_Out = (UART_Queue_Out + 1) % UART_QUEUE_SIZE;
}

// Checks if the UART queue is full
bool UART_queue_full()
{
	return UART_queue_length == UART_QUEUE_ELEMENTS;
}

// Checks if the UART queue is empty
bool UART_queue_empty()
{
	return UART_queue_length == 0;
}
