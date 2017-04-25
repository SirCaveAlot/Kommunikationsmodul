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
#include "Movement.h"

// Variables 
uint8_t data = 0;
uint8_t uartdata;
uint8_t next_movement = 0;

bool moving = false;
bool auto_control = false;

//SPI receive interrupt
ISR(SPI_STC_vect)
{
	PORTA |= (1<<0);
	uint8_t data_received;
	data_received = SPDR;
	Queue_Put(data_received);
	USART_Transmit(data_received, 1);
	SPDR = 'D';
	PORTA &= ~(1<<0);
	
}

//UART receive interrupt
ISR(USART1_RX_vect)
{
	uartdata = UDR1;
	Movement_Queue_Put(uartdata);
}

ISR(USART0_RX_vect)
{
	if (UDR0 == 'd')
	{
		moving = false;
	}
}

int main(void)
{
	DDRA = 0xFF;
	DDRD = 0b00001010;
	DDRB |= (1<<DDB1);
	
    spi_init_slave();  //Initialize slave SPI
	Queue_Init();
	Movement_Queue_Init();
	UART_Queue_Init();
	USART_Init();
	Interrupt_Init();
    sei();
	
    while(1)
    {
		if (auto_control == false)
		{
			Movement_Queue_Get(&next_movement);
			if(next_movement != 0)
			{
				USART_Transmit(next_movement, 0);
				USART_Transmit(0x00, 0);
			}
		}
		
		if (moving == false && auto_control == true)
		{
			Movement_Queue_Get(&next_movement);
			USART_Transmit(next_movement, 0);
			moving = true;
		}
    }
}