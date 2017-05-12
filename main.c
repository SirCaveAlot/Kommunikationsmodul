﻿/*
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
uint8_t uart0_received = 0;
uint8_t next_data = 0;
volatile char mode = 'D';																																																																																					 
volatile uint8_t uart1_received = 0;

volatile int distance_counter = 0;
volatile int angle_counter = 0;
volatile uint8_t data_counter = 0;
volatile uint8_t data_received;

volatile int drive_count = 0;     //Counter for drive-mode, in which it only receives 10 values. After the 10th value we send the current mode to let the PC know if it changed. 

bool pc_ready = false;
bool auto_control = false;
volatile bool running = false;

//SPI receive interrupt
ISR(SPI_STC_vect)
{
	//PORTA = data_received;
	data_received = SPDR;
	
	SPDR = mode;
	
	if(mode == 'D')
	{
		USART_Transmit(data_received, 1);
		if (data_received == 0xFF && (drive_count == 0 || drive_count == 1))
		{
			drive_count++;
		}
		else if(data_received != 0xFF && (drive_count == 0 || drive_count == 1))
		{
			drive_count = 0;
		}
		else if (drive_count >= 2)
		{
			if(drive_count == 3)
			{
				Right_side_detectable(data_received);
			}
			else if(drive_count == 4)
			{
				Left_side_detectable(data_received);
			}
			else if(drive_count == 6)
			{
				distance_traveled = data_received;
				data_received = Wheelshifts_to_distance(data_received);
			}
			else if(drive_count == 7)
			{
				distance_traveled = (distance_traveled + data_received) / 2;
				if (running == true && last_movement == 'f')
				{
					update_robot_position(distance_traveled);
				}
				distance_traveled = 0;
				data_received = Wheelshifts_to_distance(data_received);
			}
			else if(drive_count == 8)
			{
				USART_Transmit(last_movement, 1);
			}
			else if(drive_count == 9)
			{
				USART_Transmit(Get_robot_direction(), 1);
			}
			else if(drive_count == 11)
			{
				drive_count = 0;
				USART_Transmit(mode, 1);
			}
			else
			{
				drive_count++;
			}
		}
	}
	
	if(mode == 'L')
	{	
		USART_Transmit(data_received, 1);		
		if ((data_counter == 0 || data_counter == 1) && data_received == 0xFF)
		{
			data_counter++;
		}
		else if((data_counter == 0 || data_counter == 1) && data_received != 0xFF)
		{
			data_counter = 0;
		}
		else if(data_counter == 2 || data_counter == 3)
		{
			data_counter++;
			distance_array[distance_counter] = data_received;
			distance_counter++;
		}
		else if(data_counter == 4)
		{
			data_counter++;
			angle_array[angle_counter] = data_received;
			angle_counter++;
		}
		else
		{
			data_counter = 0;
			angle_array[angle_counter] = data_received;
			angle_counter++;
			
			if(angle_counter == 4000)
			{
				angle_counter = 0;
				USART_Transmit('S', 0);
				USART_Transmit('S', 1);
				USART_Transmit('S', 1);
				USART_Transmit('S', 1);
				mode = 'S';
			}
		}
		
		if (distance_counter == 4000)
		{
			distance_counter = 0;
		}
	}
}

//UART receive interrupt
ISR(USART1_RX_vect)
{
	uart1_received = UDR1;
	//UART_Queue_Put(UDR1);
	if(!Movement_queue_full() && uart1_received != 'Y')
	{
		Movement_Queue_Put(next_uart);
	}
	
	switch(uart1_received)
	{
		case 'A':
		auto_control = !auto_control;
					// 				Movement_Queue_Put('f');
					// 				Movement_Queue_Put(3);
					// 				Movement_Queue_Put('r');
					// 				Movement_Queue_Put(90);
					// 				Movement_Queue_Put('l');
					// 				Movement_Queue_Put(90);
					// 				Movement_Queue_Put('b');
					// 				Movement_Queue_Put(3);
					
		break;
					
		case 'S':
					
		mode = 'S';
		drive_count = 0;
					
		break;
					
		case 'L':
					
		mode = 'L';
		
		drive_count = 0;
					
		break;
					
		case 'T':
					
		mode = 'T';
		drive_count = 0;
					
		break;
					
		case 'Y':
					
		pc_ready = true;
					
		break;
					
		default:
					
		mode = 'D';
		drive_count = 0;

		break;
	}

}

ISR(USART0_RX_vect)
{
	uart0_received = UDR0;
	if (auto_control)
	{
		if (uart0_received == 'd')
		{
			running = false;
		}
	}
}

int main(void)
{
	DDRA = 0xFF;
	DDRD = 0b00001010;
	DDRB |= (1<<DDB1);
	
	robot_pos.x = 0;
	robot_pos.y = 0;
	robot_pos.angle = 0;
	
    spi_init_slave();  //Initialize slave SPI
	Movement_Queue_Init();
	UART_Queue_Init();
	USART_Init();
	Interrupt_Init();
    sei();
	
    while(1)
    {	
		if(mode == 'S')
		{
			Window();
			if(pc_ready)
			{
				for (int i = 0; i < 28; i++)
				{
					for (int j = 0; j < 29; j++)
					{
						USART_Transmit(map_array[i][j], 1);
					}
				}
				drive_count = 0;
				mode = 'D';
				pc_ready = false;
			}
		
 		}
 		
		if (!Movement_queue_empty())
		{
			if(auto_control)
			{
				if (running == false)
				{
					Movement_Queue_Get(&next_movement);
					USART_Transmit(0, 0);
					if(next_movement == 'A' || next_movement == 'L')
					{
						USART_Transmit(next_movement, 0);
						USART_Transmit(0, 0);
					} 
					else
					{
						if(next_movement == 'f' || next_movement == 'l' || next_movement == 'r' || next_movement == 'b')
						{
							last_movement = next_movement;
						}
						USART_Transmit(next_movement, 0);
						Movement_Queue_Get(&next_movement);
						USART_Transmit(next_movement, 0);
						running = true;
					}
				}
	
			}
			else
			{
				USART_Transmit(0, 0);
				USART_Transmit(next_movement, 0);
				USART_Transmit(0, 0);
			}
		}
		
// 		if(!UART_queue_empty())
// 		{
// 			UART_Queue_Get(&next_uart);
// 			if(!Movement_queue_full() && next_uart != 'Y')
// 			{
// 				Movement_Queue_Put(next_uart);
// 			}
// 			switch(next_uart)
// 			{
// 				case 'A':
// 				auto_control = !auto_control;
// // 				Movement_Queue_Put('f');
// // 				Movement_Queue_Put(3);
// // 				Movement_Queue_Put('r');
// // 				Movement_Queue_Put(90);
// // 				Movement_Queue_Put('l');
// // 				Movement_Queue_Put(90);
// // 				Movement_Queue_Put('b');
// // 				Movement_Queue_Put(3);
// 				
// 				break;
// 				
// 				case 'S':
// 				
// 				mode = 'S';
// 				drive_count = 0;
// 				
// 				break;
// 				
// 				case 'L':
// 				
// 				mode = 'L';
// 				drive_count = 0;
// 				
// 				break;
// 				
// 				case 'T':
// 				
// 				mode = 'T';
// 				drive_count = 0;
// 				
// 				break;
// 				
// 				case 'Y':
// 				
// 				pc_ready = true;
// 			
// 				break;
// 				
// 				default:
// 				
// 				mode = 'D';
// 				drive_count = 0;
// 
// 				break;
// 			}
// 		}
	}
}