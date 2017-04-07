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
#include <string.h>
#include <stdbool.h>
#include "UART.h"

#define clkspd 14745600
#define BAUD 115200
#define UBBR clkspd/16/BAUD-1




void USART_Init( )
{
	/* Set baud rate */
	UBRR0H = 0;
	UBRR0L = 7;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = 0b00000110;
}

void USART_Transmit(uint8_t uartdata)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = uartdata;
}

uint8_t USART_Receive( void )
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

void Interrupt_Init()
{
	UCSR0B |= (1<<RXCIE0);
	sei();
}
