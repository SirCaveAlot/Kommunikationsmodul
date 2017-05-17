/*
 * ShortestPath.c
 *
 * Created: 2017-05-08 14:27:51
 *  Author: Deep
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ShortestPath.h"
#include "Main_array.h"
#include "Adjacent_array.h"
#include "SearchAndFindGurra.h"

// uint8_t Map_array[15][15] =
// {
// 	{5, 6, 7, 8, 9, 5, 6, 7, 8, 9, 5, 6, 7, 8, 9},
// 	{6, 2, 5, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 1, 5},
// 	{7, 1, 6, 1, 6, 1, 5, 5, 6, 1, 5, 1, 6, 1, 6},
// 	{8, 1, 7, 1, 1, 1, 1, 6, 7, 1, 6, 1, 7, 1, 7},
// 	{9, 1, 1, 1, 8, 1, 1, 1, 1, 2, 7, 1, 8, 1, 8},
// 	{5, 1, 8, 8, 9, 1, 1, 1, 1, 1, 8, 1, 9, 1, 9},
// 	{6, 1, 9, 1, 6, 1, 1, 1, 1, 1, 9, 1, 5, 1, 5},
// 	{7, 1, 1, 1, 5, 1, 1, 1, 1, 1, 8, 1, 6, 1, 6},
// 	{8, 5, 5, 9, 6, 4, 1, 1, 1, 1, 7, 1, 1, 1, 7},
// 	{9, 1, 1, 1, 1, 5, 5, 6, 8, 1, 6, 5, 7, 1, 8},
// 	{5, 6, 6, 1, 7, 8, 1, 7, 1, 1, 1, 1, 8, 1, 9},
// 	{6, 1, 7, 1, 1, 1, 1, 1, 1, 6, 5, 1, 9, 1, 5},
// 	{7, 1, 8, 1, 8, 9, 5, 1, 9, 5, 1, 1, 8, 1, 6},
// 	{8, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 7, 1, 7},
// 	{5, 6, 7, 8, 9, 5, 6, 7, 8, 9, 5, 6, 7, 8, 9}
// };


uint8_t Map_array[15][15] = 
	{
		{5, 6, 7, 8, 9, 5, 6, 7, 8, 9, 5, 6, 7, 8, 9},
		{6, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 4, 5},
		{7, 1, 6, 1, 6, 1, 5, 5, 6, 1, 5, 1, 6, 1, 6},
		{8, 1, 7, 1, 7, 1, 1, 1, 7, 1, 6, 1, 7, 1, 7},
		{9, 1, 1, 1, 8, 1, 6, 1, 8, 1, 7, 1, 8, 1, 8},
		{5, 1, 8, 8, 9, 1, 7, 1, 9, 1, 8, 1, 9, 1, 9},
		{6, 1, 9, 1, 1, 1, 8, 1, 5, 1, 9, 1, 5, 1, 5},
		{7, 1, 1, 1, 5, 6, 9, 1, 6, 1, 8, 1, 6, 1, 6},
		{8, 5, 5, 9, 6, 7, 1, 1, 7, 1, 7, 1, 1, 1, 7},
		{9, 1, 1, 1, 1, 1, 1, 6, 8, 1, 6, 5, 7, 1, 8},
		{5, 6, 6, 1, 7, 8, 1, 7, 1, 1, 1, 1, 8, 1, 9},
		{6, 2, 7, 1, 1, 1, 1, 1, 1, 6, 5, 1, 9, 1, 5},
		{7, 1, 8, 1, 8, 9, 5, 1, 9, 5, 1, 1, 8, 1, 6},
		{8, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 7, 1, 7},
		{5, 6, 7, 8, 9, 5, 6, 7, 8, 9, 5, 6, 7, 8, 9}
	};

bool Check_node_if_wall(uint8_t y_position, uint8_t x_position)
{
	if(Map_array[y_position][x_position] >= 5)
	{
		return true;
	}
	return false;
}

uint8_t Node_already_visited(uint8_t y_position, uint8_t x_position)
{
	for(uint8_t i = 0; i < last_node_counter; i++)
	{
		uint8_t previous_y_position = main_node_array[i][0];
		uint8_t previous_x_position = main_node_array[i][1];
		
		if((previous_y_position == y_position) && (previous_x_position == x_position)/* && (previous_counter < iteration_counter) */)
		{
			return i;
		}
	}
	return 0xFF;
}

bool Finished()
{
	uint8_t y_position = main_node_array[node_counter][0];
	uint8_t x_position = main_node_array[node_counter][1];
	
	return ((node_counter == last_node_counter) | Start_point_found(y_position, x_position));
}

void Pathfinder()
{
	while(!Finished())
	{
		uint8_t y_position = main_node_array[node_counter][0];
		uint8_t x_position = main_node_array[node_counter][1];
		
		Load_adjacent_nodes(y_position, x_position);
		Transfer_to_main_array();
	}
	Shortest_path_in_main_array();
}

bool Start_point_found(uint8_t y_position, uint8_t x_position)
{
	if(y_position == start_position_y && x_position == start_position_x)
	{
		return true;
	}
	return false;
}

// int main()
// {
// 	Main_array_init(11, 1);
// 	Pathfinder();
// 	return 0;
// }