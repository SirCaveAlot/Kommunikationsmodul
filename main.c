/*
 * main.c
 *
 * Created: 4/9/2017 4:27:57 PM
 *  Author: jakpa844
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "SPI_slave.h"
#include "UART.h"
#include "Movement.h"
#include "mapping.h"
#include "matchtile.h"
#include "global_variables.h"
//#include <stdint-gcc.h>

// Variables 
uint8_t data = 0;
uint8_t next_movement = 0;
uint8_t next_uart = 0;
uint8_t next_data = 0;
volatile char mode = 'D';
volatile uint8_t LIDAR_array[12000];
//volatile uint8_t angle_array[4000];

																																																																																													 

volatile uint8_t LIDAR_counter = 0;
volatile uint8_t laser_counter = 0;

volatile int count1 = 1; //Counter for laser values, count1 = 1 means distance, = 2 means angle, = 3 means stop byte (0xFF)
volatile int count2 = 0; //Counter for 16-bit values, count2 = 0 means zero bytes received, = 1 means one byte, and = 2 means two bytes been received, so increase count1 by one. 
volatile int count3 = 0; //Counter for the amount of laser values. Increases for every distance or angle received. Waits for 8000 total bytes (4000 distances, 4000 angles). 
int drive_count = 0;     //Counter for drive-mode, in which it only receives 10 values. After the 10th value we send the current mode to let the PC know if it changed. 

bool auto_control = false;
volatile bool check_send = true;

//SPI receive interrupt
ISR(SPI_STC_vect)
{
	volatile uint8_t data_received = SPDR;
	PORTA = data_received;
	if(!Queue_full())
	{
		Queue_Put(data_received);
	}
	SPDR = mode;
}

//UART receive interrupt
ISR(USART1_RX_vect)
{
	volatile uint8_t uartdata = UDR1;
	UART_Queue_Put(uartdata);
}

ISR(USART0_RX_vect)
{

}

int main(void)
{
	DDRA = 0xFF;
	DDRD = 0b00001010;
	DDRB |= (1<<DDB1);
	
    spi_init_slave();  //Initialize slave SPI
//	Queue_Init();
	Movement_Queue_Init();
	UART_Queue_Init();
	USART_Init();
	Interrupt_Init();
    sei();
	 
    while(1)
    {	
		if(mode == 'S')
		{
			for (int i = 0; i < 28; i++)
			{
				for ( int j = 0; j < 29; j++)
				{
					USART_Transmit(map_array[i][j], 1);
				}
			}
			mode = 'D';
 		}
		 
		if(!Queue_empty())
		{
			if(mode == 'D')
			{
				Queue_Get(&next_data);
				USART_Transmit(next_data, 1);
				drive_count++;
				if(drive_count == 11)
				{
					drive_count = 0;
					USART_Transmit(mode, 1);
				}
			}
			else if(mode == 'L')
			{
				USART_Transmit(next_data, 1);
				LIDAR_array[LIDAR_counter] = next_data;
				LIDAR_counter++;
					
				if(LIDAR_counter >= 11999)
				{
					mode = 'D';
					LIDAR_counter = 0;
				}
			}
		}
 		
		Movement_Queue_Get(&next_movement);
		if(next_movement != 0)
		{
			if(auto_control)
			{
				USART_Transmit(0, 0);
				if(next_movement == 'A' || next_movement == 'L')
				{
					USART_Transmit(next_movement, 0);
					USART_Transmit(0, 0);
				}
				else
				{
					USART_Transmit(next_movement, 0);
					Movement_Queue_Get(&next_movement);
					USART_Transmit(next_movement, 0);
				}
			}
			else
			{
				USART_Transmit(0, 0);
				USART_Transmit(next_movement, 0);
				USART_Transmit(0, 0);
			}
		}
		
		UART_Queue_Get(&next_uart);
		if(next_uart != 0)
		{
			Movement_Queue_Put(next_uart);
			switch(next_uart)
			{
				case 'A':
				auto_control = !auto_control;
					
				break;
				
				case 'S':
			
				mode = 'S';
			
				break;
						
				case 'L':
		
				mode = 'L';
		
				break;
							
				case 'T':
	
				mode = 'T';
		
				break;
						
				default:
	
				mode = 'D';

				break;	
			}
		}
	}
}