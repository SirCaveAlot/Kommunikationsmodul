/*
 * main.c
 *
 * Created: 4/9/2017 4:27:57 PM
 *  Author: jakpa844
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "SPI_slave.h"
#include "UART.h"

// Variables 
uint8_t data = 0;
uint8_t uartdata;

//SPI receive interrupt
ISR(SPI_STC_vect)
{
	QueuePut(SPDR);
}

//UART receive interrupt
ISR(USART0_RX_vect)
{
	uartdata = UDR0;
	
	QueueGet(&data);
	
	USART_Transmit('[');
	USART_Transmit(data);
	USART_Transmit(']');
	USART_Transmit('\n');
	
	if(SPDR == 0)
	{
		SPDR = uartdata;
	}
}

int main(void)
{
	DDRD = 0b00001010;
	DDRB |= (1<<DDB1);
	
    spi_init_slave();  //Initialize slave SPI
	QueueInit();
	USART_Init();
	Interrupt_Init();
    sei();
	
    while(1)
    {
    }
}
