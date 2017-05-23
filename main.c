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

#include "SPI.h"

#include "UART.h"
#include "Movement.h"
#include "mapping.h"
#include "matchtile.h"
#include "global_variables.h"
#include "SearchAndFind.h"
#include "Positioning.h"

// Variables 
uint8_t data = 0;
uint8_t next_movement = 0;
uint8_t next_uart = 0;
uint8_t uart0_received = 0;
uint8_t next_data = 0;																																																																																	 
volatile uint8_t uart1_received = 0;


volatile uint8_t data_counter = 0;
volatile uint8_t data_received;

volatile int drive_count = 0;   //Counter for drive-mode, in which it only receives 10 values. After the 10th value we send the current mode to let the PC know if it changed. 



//SPI receive interrupt
ISR(SPI_STC_vect)
{
	data_received = SPDR;
	SPDR = mode;
	SPI_queue_put(data_received);
}

//UART receive interrupt
ISR(USART1_RX_vect)
{
	uart1_received = UDR1;
	UART_Queue_Put(uart1_received);
}

ISR(USART0_RX_vect)
{
	uart0_received = UDR0;
	if (auto_control)
	{
		if (uart0_received == 'd')
		{
			if (last_movement == 'b')
			{
				Robot_turn_around();
				last_movement = 'f';
				Set_peepz_in_da_needz();
			}
			running = false;
		}
		else if (uart0_received == 't')
		{

			if (robot_pos.x_tile == 14 && (robot_pos.y_tile == 14 || robot_pos.y_tile == 13))
			{
				USART_Transmit('F', 0);
			}
			else
			{
				last_movement = 'b';
				USART_Transmit('B', 0);
			}

		}
	}
}
// gustav
void Simulation()
{
	if(auto_control)
	{
// 		Movement_Queue_Put('L');
// 		Movement_Queue_Put('f');
// 		Movement_Queue_Put(3);
// 		Movement_Queue_Put('L');
// 		Movement_Queue_Put('r');
// 		Movement_Queue_Put(90);
// 		Movement_Queue_Put('f');
// 		Movement_Queue_Put(2);

		Movement_Queue_Put('f');
		Movement_Queue_Put(3);
/*		
		Movement_Queue_Put('l');
		Movement_Queue_Put(90);
		Movement_Queue_Put('f');
		Movement_Queue_Put(3);
		Movement_Queue_Put('l');
		Movement_Queue_Put(90);
		Movement_Queue_Put('f');
		Movement_Queue_Put(3);
		Movement_Queue_Put('l');
		Movement_Queue_Put(90);
		Movement_Queue_Put('f');
		Movement_Queue_Put(3);
		Movement_Queue_Put('l');
		Movement_Queue_Put(90);
*/
//		Movement_Queue_Put('L');


				
// 		Movement_Queue_Put('b');
// 		Movement_Queue_Put(3);
// 		Movement_Queue_Put('l');
// 		Movement_Queue_Put(90);
// 		Movement_Queue_Put('r');
// 		Movement_Queue_Put(90);
// 		Movement_Queue_Put('f');
// 		Movement_Queue_Put(1);
	}
}
//
int main(void)
{
	DDRA = 0xFF;
	DDRD = 0b00001010;
	DDRB |= (1<<DDB1);
	
	robot_pos.x = 0;
	robot_pos.y = 0;
	robot_pos.angle = 0;
	robot_pos.x_tile = 14;
	robot_pos.y_tile = 14;
	
	
    Spi_init();		//Initialize slave SPI
	Movement_Queue_Init();
	SPI_queue_init();
	UART_Queue_Init();
	USART_Init();
	Interrupt_Init();
	sei();
	
    while(1)
    {
	
		
		if(mode == 'S')
		{
			Calibrate_robot_position();
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
		if(!UART_queue_empty())
		{
			UART_Queue_Get(&next_uart);
			if(!Movement_queue_full() && next_uart != 'Y')
			{
				Movement_Queue_Put(next_uart);
			}
			switch(next_uart)
			{
				case 'A':
				auto_control = !auto_control;
				Movement_Queue_Out = (Movement_Queue_In - 1) % 21;
				Movement_queue_length = 1;
				//Gustav
				if(competition_mode == 0)
				{
					//Simulation();
				}
// 				Movement_Queue_Put('f');
// 				Movement_Queue_Put(3);
				//Gustav end
				break;
				
				// Gustav start
				
				case 'C':
// 				if(competition_mode == 2)
// 				{
// 					competition_mode = 3;
// 					USART_Transmit('C', 0);
// 				}
// 				else
// 				{
				competition_mode = 1;
				
				Movement_Queue_Put('f');
				Movement_Queue_Put(15);
				
				break;
				
				case 'f':
				if(auto_control)
				{
					Movement_Queue_Put(3);
				}
				break;
				
				case 'b':
				if(auto_control)
				{
					Movement_Queue_Put(3);
				}
				break;
				
				case 'r':
				if(auto_control)
				{
					Movement_Queue_Put(90);
				}
				break;
				
				case 'l':
				if(auto_control)
				{
					Movement_Queue_Put(90);
				}
				break;
				
				// Gustav end
				
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
		
		if (!Movement_queue_empty() && mode == 'D')
		{		
			if(auto_control)
			{
				if (running == false)
				{
					Movement_Queue_Get(&next_movement);
					PORTA = next_movement;
					if(next_movement == 'L')
					{
						mode_changed = true;
						running = true;
					}
					USART_Transmit(0, 0);
					if(next_movement == 'A' || next_movement == 'L' || next_movement == 's' || next_movement == 'S' || next_movement == 'C')
					{
						USART_Transmit(next_movement, 0);
						USART_Transmit(0, 0);
					}
					else
					{
						if(next_movement == 'f' || next_movement == 'l' || next_movement == 'r' || next_movement == 'b')
						{
							
							
							if(last_movement == 'b' && next_movement != 'b')
							{
								Robot_turn_around();
							}
							last_movement = next_movement;
							if (next_movement == 'r')
							{
								Robot_turn_right();
							}
							else if(next_movement == 'l')
							{
								Robot_turn_left();
							}
							else if(next_movement == 'b')
							{
								Robot_turn_around();
							}
						}
						USART_Transmit(next_movement, 0);
						Movement_Queue_Get(&next_movement);
						if(next_movement == 180)
						{
							if(last_movement == 'l')
							{
								Robot_turn_left();
							}
							else if(last_movement == 'r')
							{
								Robot_turn_right();
							}
						}
						USART_Transmit(next_movement, 0);
						running = true;
					}
				}
			}
			else
			{
				Movement_Queue_Get(&next_movement);
				if(next_movement == 'f' || next_movement == 'l' || next_movement == 'r' || next_movement == 'b')
				{
					if(last_movement == 'b' && next_movement != 'b')
					{
						Robot_turn_around();
					}
					last_movement = next_movement;
					if (next_movement == 'r')
					{
						Robot_turn_right();
					}
					else if(next_movement == 'l')
					{
						Robot_turn_left();
					}
					else if(next_movement == 'b')
					{
						Robot_turn_around();
					}
				}
				USART_Transmit(0, 0);
				USART_Transmit(next_movement, 0);
				USART_Transmit(0, 0);
			}
		}
		
		if(mode == 'D')
		{
			Dequeue_SPI_queue_D_mode();
		}
		else if(mode == 'L')
		{
			Dequeue_SPI_queue_L_mode();
		}
		if(competition_mode == 1)
		{
			//PORTA = competition_mode;
			robot_keep_right();
		}
		else if(competition_mode == 2)
		{
			// Fill the remaining tiles
		}
		else if(competition_mode == 3)
		{
			// Shortest path algorithm
		}
	}
}
