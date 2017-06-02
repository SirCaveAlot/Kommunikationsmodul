/*
 * SPI.c
 *
 * Created: 4/3/2017 2:10:56 PM
 *  Author: jakpa844
 * Code written by Jakob Palm
 *
 * This file contains the queue for SPI data.
 */ 

// 
// void SPI_slave_init()
// {
// 	DDRB |= (1 << 6); // MISO as output, slave configuration.
// 	SPCR |= (1 << SPE) | (1 << SPIE); // SPI and SPI interrupt enabled.
// 	SPSR |= (0 << SPI2X);
// 	SPDR = 0x00; // Clear SPI interrupt flag by reading SPSR and SPDR.
// }
// 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "SPI.h"
#include "mapping.h"
#include "global_variables.h"
#include "UART.h"
#include "Movement.h"


/* Queue structure */
#define SPI_QUEUE_ELEMENTS 1000
#define SPI_QUEUE_SIZE (SPI_QUEUE_ELEMENTS + 1)
volatile uint8_t SPI_queue[SPI_QUEUE_SIZE];
volatile uint8_t SPI_queue_in, SPI_queue_out;
uint8_t SPI_queue_length;

bool dequeue = false;
bool dequeue_L = false;

// Initializes SPI.
void Spi_init()
{
	DDRB = (1<<DDB6);               //MISO as OUTPUT
	SPDR = 'D';
	SPCR = (1<<SPIE)|(1<<SPE);       //Enable SPI && interrupt enable bit
}

// Initializes the SPI queue
void SPI_queue_init()
{
	SPI_queue_in = SPI_queue_out = 0;
	SPI_queue_length = 0;
	dequeue = false;
}

// Inserts a uint8_t into the SPI queue
void SPI_queue_put(uint8_t new)
{
	if(SPI_queue_length == SPI_QUEUE_ELEMENTS)
	{
		return; /* Queue Full*/
	}

	SPI_queue[SPI_queue_in] = new;
	SPI_queue_in = (SPI_queue_in + 1) % SPI_QUEUE_SIZE;	
	SPI_queue_length++;
}

// Pulls the first uint8_t from the SPI queue
void SPI_queue_get(uint8_t *old)
{
	if(SPI_queue_length == 0)
	{
		return; /* Queue Empty - nothing to get*/
	}

	*old = SPI_queue[SPI_queue_out];	
	SPI_queue_out = (SPI_queue_out + 1) % SPI_QUEUE_SIZE;	
	SPI_queue_length--;
}

// Looks at the first element in the SPI queue
uint8_t SPI_queue_peek(uint8_t queue_index)
{
	return SPI_queue[queue_index];
}

// Removes the first element in the SPI queue
void SPI_queue_remove()
{
	if(SPI_queue_length == 0)
	{
		return; // Queue Empty - nothing to remove
	}
	SPI_queue_out = (SPI_queue_out + 1) % SPI_QUEUE_SIZE;
	SPI_queue_length--;
}

// Dequeues the SPI when in Drive mode
void Dequeue_SPI_queue_D_mode()
{		
	// Checks if all data has been received
	if(SPI_queue_length < 13)
	{
		dequeue = false;
		return;
	}
	
	// Check if data has arrived in correct order
	Start_dequeuing_D_mode();
	
	// Dequeues the SPI queue
	if(dequeue)
	{
		uint8_t last_byte = 0;
		if(mode == 'D')
		{	
			// 0xFFFF
			SPI_queue_get(&last_byte);
			USART_Transmit(last_byte, 1);
			SPI_queue_get(&last_byte);
			USART_Transmit(last_byte, 1);
			
			// Right IR
			SPI_queue_get(&right_IR);
			USART_Transmit(right_IR, 1);
			
			// Right IR back
			SPI_queue_get(&right_IR_back);
			USART_Transmit(right_IR_back, 1);
			Right_side_detectable(right_IR, right_IR_back);

			// Left IR
			SPI_queue_get(&left_IR);
			USART_Transmit(left_IR, 1);
			
			// Left IR back
			SPI_queue_get(&left_IR_back);
			USART_Transmit(left_IR_back, 1);
			Left_side_detectable(left_IR, left_IR_back);
			
			// Forward IR
			SPI_queue_get(&front_IR);
			USART_Transmit(front_IR, 1);
			Front_side_detectable(front_IR);
			
			// Tape
			USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
			SPI_queue_remove();
			USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
			SPI_queue_remove();
			
			// Wheels
			SPI_queue_get(&distance_traveled);
			distance_traveled = Wheelshifts_to_distance(distance_traveled);
			distance_traveled = (distance_traveled + Wheelshifts_to_distance(SPI_queue_peek(SPI_queue_out))) / 2;
			SPI_queue_remove();
			if ((!auto_control || running == true) && (last_movement == 'f' || last_movement == 'b'))
			{
				update_robot_position(distance_traveled);
			}
			if (last_movement != 'r' || last_movement != 'l')
			{
			    Set_tile_from_ir();
	
			}
			
			// Robot position in mm			
			USART_Transmit((uint8_t)(robot_pos.x >> 8), 1);
			USART_Transmit((uint8_t)(robot_pos.x), 1);
			USART_Transmit((uint8_t)(robot_pos.y >> 8), 1);
			USART_Transmit((uint8_t)(robot_pos.y), 1);
			
			distance_traveled = 0;
			
			// Gyro
			USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
			SPI_queue_remove();
			USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
			SPI_queue_remove();
			
			// LIDAR distance
			USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
			SPI_queue_remove();
			USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
			SPI_queue_remove();
			
			// Last movement
			USART_Transmit(last_movement, 1);
			
			// Robot direction
			USART_Transmit(Get_robot_direction(), 1);
			
			// Robot position in the map_array coordinates. 
			USART_Transmit(robot_pos.x_tile, 1);
			USART_Transmit(robot_pos.y_tile, 1);
			if(mode_changed)
			{
				mode = 'L';
				mode_changed = false;
			}
			// Mode
			USART_Transmit(mode, 1);
			
			
			dequeue = false;
		}
	}
	else
	{
		SPI_queue_remove();
	}

}

// Checks if data has arrived in correct order
void Start_dequeuing_D_mode()
{
	uint8_t first_value = SPI_queue_peek(SPI_queue_out);
	uint8_t second_value = SPI_queue_peek(SPI_queue_out + 1);
	
	if(first_value == 0xFF && second_value == 0xFF)
	{
		uint8_t ninth_value = SPI_queue_peek(SPI_queue_out + 8);
		uint8_t tenth_value = SPI_queue_peek(SPI_queue_out + 9);
		
		if(ninth_value == 0xFF && tenth_value == 0xFF)
		{
			dequeue = false;
		}
		else
		{
			dequeue = true;
		}
	}
	else
	{
		dequeue = false;
	}
}

// Dequeues the SPI queue when in Laser mode
void Dequeue_SPI_queue_L_mode()
{
	// Check if an entire reading has been transmitted yet (0xFFFF, distance, angle)
	if(SPI_queue_length < 6)
	{
		dequeue_L = false;
		return;
	}
	// Check if data has arrived in the correct order
	Start_dequeuing_L_mode();
	
	// Dequeues the SPI queue
	if(dequeue_L)
	{
		// 0xFFFF
		USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
		SPI_queue_remove();
		USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
		SPI_queue_remove();
		
		// Laser distance
		distance_array[distance_counter] = SPI_queue_peek(SPI_queue_out);
		USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
		distance_counter++;
		SPI_queue_remove();

		distance_array[distance_counter] = SPI_queue_peek(SPI_queue_out);
		USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
		distance_counter++;
		SPI_queue_remove();
		
		// Laser angle
		angle_array[angle_counter] = SPI_queue_peek(SPI_queue_out);
		USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
		angle_counter++;
		SPI_queue_remove();

		angle_array[angle_counter] = SPI_queue_peek(SPI_queue_out);
		USART_Transmit(SPI_queue_peek(SPI_queue_out), 1);
		angle_counter++;
		SPI_queue_remove();
		
		// Check if received 2000 readings (uint16_t, so 4000 uint8_t)
		if(angle_counter == 4000)
		{
			mode = 'S';
			angle_counter = 0;
			distance_counter = 0;
			running = false;
// 			USART_Transmit(0, 0);
// 			USART_Transmit('S', 0);
// 			USART_Transmit(0, 0);
			USART_Transmit('S', 1);
			USART_Transmit('S', 1);
			USART_Transmit('S', 1);
		}
		dequeue_L = false;
	}
	else 
	{
		SPI_queue_remove();
	}
}

// Checks if data arrived in correct order
void Start_dequeuing_L_mode()
{
	uint8_t first_value = SPI_queue_peek(SPI_queue_out);
	uint8_t second_value = SPI_queue_peek(SPI_queue_out + 1);
	uint8_t third_value = SPI_queue_peek(SPI_queue_out + 2);
	
	if(first_value == 0xFF && second_value == 0xFF && third_value != 0xFF)
	{
		dequeue_L = true;
	}
}
