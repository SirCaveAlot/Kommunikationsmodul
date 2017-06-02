/*
 * main.c
 *
 * Created: 4/9/2017 4:27:57 PM
 *  Author: jakpa844
 * Code written by Jakob Palm
 *
 * This file contains all functions that deal with interrupts, 
 * aswell as the main loop.
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
#include "ShortestPath.h"
#include "Main_array.h"

// Variables 
uint8_t data = 0;
uint8_t next_uart = 0;
uint8_t uart0_received = 0;
uint8_t next_data = 0;																																																																																	 
volatile uint8_t uart1_received = 0;
volatile uint8_t data_counter = 0;
volatile uint8_t data_received;
volatile int drive_count = 0;  

//SPI receive interrupt
ISR(SPI_STC_vect)
{
	data_received = SPDR;
	SPDR = mode;
	SPI_queue_put(data_received);
}

//UART1 receive interrupt (interrupts sent from the PC)
ISR(USART1_RX_vect)
{
	uart1_received = UDR1;
	UART_Queue_Put(uart1_received);
}

// UART0 receive interrupts (interrupts sent from the control unit)
ISR(USART0_RX_vect)
{
	uart0_received = UDR0;
	if (auto_control)
	{
		// If the control unit is done with the last movement decision, set running = false so 
		// we can send the next movement decision.
		if (uart0_received == 'd')
		{
			if(last_movement == 'b')
			{
				Robot_turn_around();
				last_movement = 'f';
				Set_peepz_in_da_needz();
			}
			Calibrate_robot_position();
			running = false;
		}
		// Checks if the control unit has registered tape on the floor.
		else if (uart0_received == 't')
		{
			if (robot_pos.x_tile == 14 && robot_pos.y_tile == 13)
			{
				USART_Transmit('F', 0);
			}
			else if (robot_pos.x_tile == 14 && robot_pos.y_tile == 14)
			{
				if(Get_robot_direction() == 8)
				{
					USART_Transmit('F', 0);
				}
				else if(Get_robot_direction() == 2)
				{
					competition_mode = 2;
					Movement_Queue_Put('s');
					running = false;
				}	
			}
			else if(competition_mode == 1)
			{
				Robot_turn_around();
				last_movement = 'b';
				USART_Transmit('B', 0);
			}

		}
	}
}

// Simulation can be used to test if certain movement decisions work.
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

	}
}
// The main function for the communication module. 
int main(void)
{
	// Initializes everything.
	DDRA = 0xFF;
	DDRD = 0b00001010;
	DDRB |= (1<<DDB1);
	
	robot_pos.x = 0;
	robot_pos.y = 0;
	robot_pos.angle = 0;
	robot_pos.x_tile = 14;
	robot_pos.y_tile = 14;
	
    	Spi_init();
	Movement_Queue_Init();
	SPI_queue_init();
	UART_Queue_Init();
	USART_Init();
	Interrupt_Init();
	sei();
    // Main loop
 	while(1)
 	{
	
		// If the comm. module is in stop mode, send the entire map to the PC.
		if(mode == 'S')
		{
			//Calibrate_robot_position();
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
		
		// Check if received anything from the PC
		if(!UART_queue_empty())
		{
			UART_Queue_Get(&next_uart);
			if(!Movement_queue_full() && next_uart != 'Y')
			{
				Movement_Queue_Put(next_uart);
			}
			// All cases of user input.
			switch(next_uart)
			{
				// 'A' switches between auto control and manual control
				case 'A':
				auto_control = !auto_control;
				Movement_Queue_Out = (Movement_Queue_In - 1) % 21;
				Movement_queue_length = 1;
				if(competition_mode == 0)
				{
					//Simulation();
				}
// 				Movement_Queue_Put('f');
// 				Movement_Queue_Put(3);
				
				break;
				
				// 'C' is used for competition mode
				case 'C':
				if(competition_mode == 2)
				{
					competition_mode = 3;
					//PORTA = competition_mode;
					robot_pos.y_tile = 14;
					robot_pos.x_tile = 14;
					robot_pos.x = 0;
					robot_pos.y = 0;
					Set_robot_angle_direction(8);
				}
				else
				{
				competition_mode = 1;
				Movement_Queue_Put('f');
				Movement_Queue_Put(15);
				}
				break;
				
				// 'f' move the robot forward
				case 'f':
				if(auto_control)
				{
					Movement_Queue_Put(3);
				}
				break;
				
				// 'b' moves the robot backwards
				case 'b':
				if(auto_control)
				{
					Movement_Queue_Put(3);
				}
				break;
				
				// 'r' rotates the robot to the right
				case 'r':
				if(auto_control)
				{
					Movement_Queue_Put(90);
				}
				break;
				
				// 'l' rotates the robot to the left
				case 'l':
				if(auto_control)
				{
					Movement_Queue_Put(90);
				}
				break;
				
				// 'S' switches to stop mode
				case 'S':
				
				mode = 'S';
				drive_count = 0;
				
				break;
				
				// 'L' switches to laser mode
				case 'L':
				
				mode = 'L';
				drive_count = 0;
				
				break;
				
				// 'T' switches to test mode, not currently used
				case 'T':
				
				mode = 'T';
				drive_count = 0;
				
				break;
				
				// 'Y' is a handshake from the PC, used to synchronize the 
				// transmit of the entire map during 'S' mode
				case 'Y':
				
				pc_ready = true;
				
				break;
				
				// Otherwise set the mode to drive mode
				default:
				
				mode = 'D';
				drive_count = 0;

				break;
			}
		}
		
		// Check if anything has been put on the movement queue
		if (!Movement_queue_empty() && mode == 'D')
		{		
			if(auto_control)
			{
				if (running == false)
				{
					Movement_Queue_Get(&next_movement);
					PORTA = next_movement;
					// If 'L is the next movement, wait for the 'L' to be transmitted to the PC
					// before switching mode
					if(next_movement == 'L')
					{
						mode_changed = true;
						running = true;
					}
					USART_Transmit(0, 0);
					// Check if the next movement isn't something that requires a distance or angle.
					if(next_movement == 'A' || next_movement == 'L' || next_movement == 's' || next_movement == 'S' || 
					   next_movement == 'C' || next_movement == 'o' || next_movement == 'c')
					{
						USART_Transmit(next_movement, 0);
						USART_Transmit(0, 0);
					}
					else
					{
						if(next_movement == 'f' || next_movement == 'l' || next_movement == 'r' || next_movement == 'b')
						{
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
						//PORTA = next_movement;
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
		
		// Dequeue the SPI queue during drive mode
		if(mode == 'D')
		{
			Dequeue_SPI_queue_D_mode();
		}
		// Dequeue the SPI queue during laser mode
		else if(mode == 'L')
		{
			Dequeue_SPI_queue_L_mode();
		}
		
		// Take movement decisions based on our robot_keep_right algorithm if competition_mode == 1
		if(competition_mode == 1)
		{
			//PORTA = competition_mode;
			robot_keep_right();
		}
		else if(competition_mode == 2)
		{
			// Create shortest path
			if(!shortest_path_created)
			{
				Main_array_init(Find_y_end_position(), Find_x_end_position());
				Pathfinder();
				nearest_path_to_array();
			}
		}
		// Else take movement decisions based on the created shortest path
		else if(competition_mode == 3)
		{
			// Shortest path algorithm
			if(!nearest_path_driven && !running)
			{
				drive_nearest_path();
			}
			else if(!nearest_path_driven_back && nearest_path_driven && !running)
			{
				drive_back_nearest_path();
			}
		}
	}
}
