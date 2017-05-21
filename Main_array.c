/*
 * Main_array.c
 *
 * Created: 2017-05-09 11:30:16
 *  Author: Deep
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Main_array.h"
#include "Adjacent_array.h"
#include "ShortestPath.h"

#define MAXIMUM_QUEUE_SIZE 150

uint8_t main_node_array[MAXIMUM_QUEUE_SIZE][3];
uint8_t node_counter;
uint8_t last_node_counter;

void Main_array_init(uint8_t y_end_position, uint8_t x_end_position)
{
	last_node_counter = 0;
	node_counter = 0;
	Load_row_in_main_array(y_end_position, x_end_position, 0);
}

void Load_row_in_main_array(uint8_t y_position, uint8_t x_position, uint8_t iteration_counter)
{
	main_node_array[last_node_counter][0] = y_position;
	main_node_array[last_node_counter][1] = x_position;
	main_node_array[last_node_counter][2] = iteration_counter;
	last_node_counter++;
}

void Transfer_to_main_array()
{
	node_counter++;
	for(uint8_t i = 0; i < 4; i++)
	{
		if(adjacent_nodes_array[i][0] == 0xFF)
		{
			Load_row_in_adjacent_array(0, 1, i, 0);
			return;
		}
		uint8_t y_position = adjacent_nodes_array[i][0];
		uint8_t x_position = adjacent_nodes_array[i][1];
		uint8_t current_iteration_counter = adjacent_nodes_array[i][2];
		
		uint8_t already_visited_position = Node_already_visited(y_position, x_position);
		
		if(already_visited_position != 0xFF) // If node has already been visited
		{
			uint8_t previous_iteration_counter = main_node_array[already_visited_position][2];
			
			if(current_iteration_counter < previous_iteration_counter) // If the already visited nodes counter > current nodes counter, then replace
			{
				main_node_array[previous_iteration_counter][0] = y_position;
				main_node_array[previous_iteration_counter][1] = x_position;
				main_node_array[previous_iteration_counter][2] = current_iteration_counter;
			}
		}
		else
		{
			Load_row_in_main_array(y_position, x_position, current_iteration_counter);
		}
		Load_row_in_adjacent_array(0, 1, i, 0);
	}
}

void Shortest_path_in_main_array()
{
	uint8_t temporary_array[150][3]; // Check if 150 is enough.
	uint8_t temporary_array_counter = 0;
	
	while(last_node_counter > node_counter)
	{
		main_node_array[last_node_counter][0] = 0;
		main_node_array[last_node_counter][1] = 0;
		main_node_array[last_node_counter][2] = 0;
		last_node_counter--;
	}
	
	uint8_t current_node_y_position = main_node_array[node_counter][0];
	uint8_t current_node_x_position = main_node_array[node_counter][1];
	uint8_t current_node_counter = main_node_array[node_counter][2];
	
	temporary_array[temporary_array_counter][0] = main_node_array[node_counter][0]; // Load start poin in temporary array
	temporary_array[temporary_array_counter][1] = main_node_array[node_counter][1];
	temporary_array[temporary_array_counter][2] = main_node_array[node_counter][2];
	temporary_array_counter++;
	
	main_node_array[node_counter][0] = 0;
	main_node_array[node_counter][1] = 0;
	main_node_array[node_counter][2] = 0;	
	node_counter--;
	
	while(current_node_counter > 0)
	{
		uint8_t next_node_y_position = main_node_array[node_counter][0];
		uint8_t next_node_x_position = main_node_array[node_counter][1];
		uint8_t next_node_counter = main_node_array[node_counter][2];
		
		if((next_node_counter == (current_node_counter - 1)) && 
			Check_if_adjacent_node(current_node_y_position, current_node_x_position, next_node_y_position, next_node_x_position))
		{
			temporary_array[temporary_array_counter][0] = next_node_y_position; // Load next node in temporary array
			temporary_array[temporary_array_counter][1] = next_node_x_position;
			temporary_array[temporary_array_counter][2] = next_node_counter;
			temporary_array_counter++;
			
			current_node_y_position = next_node_y_position; 
			current_node_x_position = next_node_x_position;
			current_node_counter = next_node_counter;
		}
		
		main_node_array[node_counter][0] = 0;
		main_node_array[node_counter][1] = 0;
		main_node_array[node_counter][2] = 0;
		node_counter--;	
	}
	
	temporary_array[temporary_array_counter + 1][0] = 0xFF;
	temporary_array_counter = 0;
	node_counter = 0;
	last_node_counter = 1;
	
	while(temporary_array[temporary_array_counter][0] != 0xFF)
	{
		main_node_array[node_counter][0] = temporary_array[temporary_array_counter][0];	
		main_node_array[node_counter][1] = temporary_array[temporary_array_counter][1];	
		main_node_array[node_counter][2] = temporary_array[temporary_array_counter][2];		
		node_counter++;
		temporary_array_counter++;
		last_node_counter++;
	}
}

bool Check_if_adjacent_node(uint8_t y_position_1, uint8_t x_position_1, uint8_t y_position_2, uint8_t x_position_2)
{
	if(((x_position_1 == x_position_2 + 1) && (y_position_1 == y_position_2)) | ((x_position_1 == x_position_2 - 1) && (y_position_1 == y_position_2)) |
	   ((x_position_1 == x_position_2) && (y_position_1 == y_position_2 + 1)) | ((x_position_1 == x_position_2) && (y_position_1 == y_position_2 - 1)))
	   {
			return true;
	   }
	   return false;
}

// void Convert_to_steering_modes(uint8_t path_array[150][3])
// {
// 	uint8_t array_counter = 0;
// 	
// 	while(path_array[array_counter + 1][])
// }