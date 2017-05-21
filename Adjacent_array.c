/*
 * Adjacent_array.c
 *
 * Created: 2017-05-09 11:36:36
 *  Author: Deep
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Adjacent_array.h"
#include "Main_array.h"
#include "ShortestPath.h"


#define NUMBER_OF_ADJACENTS 4

uint8_t adjacent_nodes_array[NUMBER_OF_ADJACENTS][3];

void Load_row_in_adjacent_array(uint8_t y_position, uint8_t x_position, uint8_t adjacent_counter, uint8_t iteration_counter)
{
	adjacent_nodes_array[adjacent_counter][0] = y_position;
	adjacent_nodes_array[adjacent_counter][1] = x_position;
	adjacent_nodes_array[adjacent_counter][2] = iteration_counter;
}

void Load_adjacent_nodes(uint8_t y_position, uint8_t x_position) // Puts the correct adjacent nodes in adjacent_nodes_array
{
	uint8_t adjacent_node_counter = 0;
	uint8_t iteration_counter = main_node_array[node_counter][2] + 1;

	if(!Check_node_if_wall(y_position + 1, x_position))
	{
		Load_row_in_adjacent_array(y_position + 1, x_position, adjacent_node_counter, iteration_counter); // Load lower position if not wall
		adjacent_node_counter++;
	}
	
	if(x_position != 0)
	{
		if(!Check_node_if_wall(y_position - 1, x_position))
		{
			Load_row_in_adjacent_array(y_position - 1, x_position, adjacent_node_counter, iteration_counter); // Load upper position if not wall
			adjacent_node_counter++;
		}
	}
	
	if(!Check_node_if_wall(y_position, x_position + 1))
	{
		Load_row_in_adjacent_array(y_position, x_position + 1, adjacent_node_counter, iteration_counter); // Load right position if not wall
		adjacent_node_counter++;
	}
	
	if(y_position != 0)
	{
		if(!Check_node_if_wall(y_position, x_position - 1))
		{
			Load_row_in_adjacent_array(y_position, x_position - 1, adjacent_node_counter, iteration_counter); // Load left position if not wall (CHECK COORDINATE DEFINITIONS!!)
			adjacent_node_counter++;
		}
	}
	
	if(adjacent_node_counter < 4)
	{
		Load_row_in_adjacent_array(0xFF, 1, adjacent_node_counter, 0); // Stop position in the array
	}
}
