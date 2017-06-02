/*
 * Main_array.c
 *
 * Created: 2017-05-09 
 * Author: Gustav Strandberg, gusst967
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Main_array.h"
#include "Adjacent_array.h"
#include "ShortestPath.h"

//-------------------------Main array for shortest path------------------------
#define MAXIMUM_QUEUE_SIZE 150

uint8_t main_node_array[MAXIMUM_QUEUE_SIZE][3];
uint8_t node_counter;
uint8_t last_node_counter;

//-------------------------------Functions-------------------------------------

// Loads the end point of the shortest path in the main array.
void Main_array_init(uint8_t y_end_position, uint8_t x_end_position)
{
	last_node_counter = 0;
	node_counter = 0;
	Load_row_in_main_array(y_end_position, x_end_position, 0);
}

// Loads a position with the associated counter value in the main array.
void Load_row_in_main_array(uint8_t y_position, uint8_t x_position, 
							uint8_t iteration_counter)
{
	main_node_array[last_node_counter][0] = y_position;
	main_node_array[last_node_counter][1] = x_position;
	main_node_array[last_node_counter][2] = iteration_counter;
	last_node_counter++;
}

// Transfers elements in the adjacent array in the main array. 
// Only nodes that aren't walls or already in the main array will be 
// transferred.
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
		
		uint8_t already_visited_position = Node_already_visited(y_position, 
																x_position);
		
		// If node has already been visited.
		if(already_visited_position != 0xFF) 
		{
			uint8_t previous_iteration_counter = 
					main_node_array[already_visited_position][2];
			
			// If the already visited node value > current node value,
			// then replace.
			if(current_iteration_counter < previous_iteration_counter) 
			{
				main_node_array[previous_iteration_counter][0] = y_position;
				main_node_array[previous_iteration_counter][1] = x_position;
				main_node_array[previous_iteration_counter][2] = 
				current_iteration_counter;
			}
		}
		else
		{
			Load_row_in_main_array(y_position, x_position, 
								   current_iteration_counter);
		}
		Load_row_in_adjacent_array(0, 1, i, 0);
	}
}

// Places the shortest path and removes all the other elements in the main
// array.
void Shortest_path_in_main_array()
{
	// A temporary array to store the shortest path.
	uint8_t temporary_array[150][3];
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
	
	// Load start point in temporary array.
	temporary_array[temporary_array_counter][0] = main_node_array[node_counter][0]; 
	temporary_array[temporary_array_counter][1] = main_node_array[node_counter][1];
	temporary_array[temporary_array_counter][2] = main_node_array[node_counter][2];
	temporary_array_counter++;
	
	main_node_array[node_counter][0] = 0;
	main_node_array[node_counter][1] = 0;
	main_node_array[node_counter][2] = 0;	
	node_counter--;
	
	// Loop to store the shortest path in the temporary array.
	// Checks the current nodes counter value and stops when that is 0.
	while(current_node_counter > 0)
	{
		uint8_t next_node_y_position = main_node_array[node_counter][0];
		uint8_t next_node_x_position = main_node_array[node_counter][1];
		uint8_t next_node_counter = main_node_array[node_counter][2];
		
		// If the next node has a counter value -1 of the current node
		// and the nodes are next to each other it is a part of the shortest
		// path.
		if((next_node_counter == (current_node_counter - 1)) && 
			Check_if_adjacent_node(current_node_y_position, 
				current_node_x_position, next_node_y_position, 
				next_node_x_position))
		{
			// Load next node in temporary array.
			temporary_array[temporary_array_counter][0] = next_node_y_position; 
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
	
	// Loads the temporary array in the main array.
	while(temporary_array[temporary_array_counter][0] != 0xFF)
	{
		main_node_array[node_counter][0] = 
		temporary_array[temporary_array_counter][0];	
		main_node_array[node_counter][1] = 
		temporary_array[temporary_array_counter][1];	
		main_node_array[node_counter][2] = 
		temporary_array[temporary_array_counter][2];		
		node_counter++;
		temporary_array_counter++;
		last_node_counter++;
	}
}

// Checks if two nodes are next to each other.
bool Check_if_adjacent_node(uint8_t y_position_1, uint8_t x_position_1, 
							uint8_t y_position_2, uint8_t x_position_2)
{
	if(((x_position_1 == x_position_2 + 1) && (y_position_1 == y_position_2)) |
	   ((x_position_1 == x_position_2 - 1) && (y_position_1 == y_position_2)) |
	   ((x_position_1 == x_position_2) && (y_position_1 == y_position_2 + 1)) | 
	   ((x_position_1 == x_position_2) && (y_position_1 == y_position_2 - 1)))
	   
	   {
			return true;
	   }
	   return false;
}
