/*
 * SearchAndFind.c
 *
 * Created: 5/12/2017 2:53:12 PM
 *  Author: marwa079
 */ 

#define up 8
#define right 6
#define down 2
#define left 4

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include "SearchAndFind.h"
#include "Positioning.h"
#include "mapping.h"
#include "global_variables.h"
#include "Movement.h"
#include "UART.h"
#include "fake_robot_code.h"
#include "Main_array.h"

uint8_t robot_array[29][29];


//back at start position?
bool back_at_start(int yposition,int xposition)
{
	if(yposition == 14 && xposition == 14)
	{
		//Robot_turn_around(); //turn around (assuming start position was up)
		return true; // right-lap is finished
	}
	else
	{
		return false;
	}
}
//detecting walls
bool detect_wall(int next_yposition,int next_xposition)
{
	if(map_array[next_yposition][next_xposition] == 0)
	{
		return true;
	}
	else if(map_array[next_yposition][next_xposition] == 1)
	{
		return false;
	}
	else if(map_array[next_yposition][next_xposition] == 2)
	{
		return true;
	}
	else if(map_array[next_yposition][next_xposition] == 4)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

bool detect_outer_wall(int next_yposition,int next_xposition)
{
	if(next_yposition < 0 || next_xposition < 0)
	{
		return false;
	}
	else if(map_array[next_yposition][next_xposition] == 5)
	{
		return true;
	}
	else if(map_array[next_yposition][next_xposition] == 4)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}


void set_coordinate_in_array(int y, int x, int value) //sets coordinate in array to value
{
	robot_array[y][x] = value;
}


//As long as we don't find a wall ahead or a clear at the right hand side, the robot keeps moving forward
//BUT! if we first off, find a clear way to travel to the right, we turn right and if we find a wall we
//clearly have to turn left
void robot_keep_right()
{
	if(!running && Movement_queue_empty())
	{
		if ((front_side_detected || map_array[next_y_position(Get_robot_direction())][next_x_position(Get_robot_direction())] == 2) && left_side_detected && right_side_detected)
		{   
			//Calibrate_robot_position();
			Movement_Queue_Put('l');
			Movement_Queue_Put(180);
			Movement_Queue_Put('f');
			Movement_Queue_Put(15);
		}
		else if ((front_side_detected || map_array[next_y_position(Get_robot_direction())][next_x_position(Get_robot_direction())] == 2) && right_side_detected)
		{
			//Calibrate_robot_position();
			Movement_Queue_Put('l');
			Movement_Queue_Put(90);
			Movement_Queue_Put('f');
			Movement_Queue_Put(15);
		}
		else if (right_side_detected)
		{
			Movement_Queue_Put('f');
			Movement_Queue_Put(15);
		}
		else if (map_array[next_y_position(Get_robot_direction())][next_x_position(Get_robot_direction())] == 2 && left_side_detected)
		{
			Movement_Queue_Put('r');
			Movement_Queue_Put(90);
		}
		else if (map_array[next_y_position(Get_robot_direction())][next_x_position(Get_robot_direction())] == 2)
		{
			Movement_Queue_Put('l');
			Movement_Queue_Put(90);
			Movement_Queue_Put('f');
			Movement_Queue_Put(15);
		}
		else
		{
			//Calibrate_robot_position();
			Movement_Queue_Put('L');
			Movement_Queue_Put('r');
			Movement_Queue_Put(90);
			Movement_Queue_Put('f');
			Movement_Queue_Put(15);
		}
	}
// 	if(next_movement == 's')
// 	{
// 		competition_mode = 2;
// 	}
}



void find_first_undiscovered_tile_x()
{
	uint8_t y_position = 1; // y_position defined from upper left corner and down.
	while(y_position < 28)
	{
		uint8_t x_position = 1;
		while(x_position < 28)
		{
			if(undiscovered_tiles_array[y_position][x_position] == 0)
			{
				if(undiscovered_tiles_array[y_position - 1][x_position] == 1)
				{
					end_position_x = x_position;
					end_position_y = y_position - 1;
					return;
				}
				else if(undiscovered_tiles_array[y_position][x_position - 1] == 1)
				{
					end_position_x = x_position - 1;
					end_position_y = y_position;
					return;
				}
				else if(undiscovered_tiles_array[y_position + 1][x_position] == 1)
				{
					end_position_x = x_position;
					end_position_y = y_position + 1;
					return;
				}
				else if(undiscovered_tiles_array[y_position][x_position + 1] == 1)
				{
					end_position_x = x_position + 1;
					end_position_y = y_position;
					return;
				}
			}
			x_position++;
		}
		y_position++;
	}
}



int nearest_path_array[29][29]; //contains the coordinates with the nearest path

int gurras_array[20][3] = //example of what gurras_array might look like
{
	{14, 14, 4} ,
	{13, 14, 3} ,
	{13, 15, 2} ,
	{12, 15, 1} ,
	{11, 15, 0} 
};

void nearest_path_to_array() //takes the coordinates from gurras_array and puts it in nearest_path_array[29][29]
{
	int m = 0;
	while(main_node_array[m][2] != 0)
	{
		nearest_path_array[main_node_array[m][0]][main_node_array[m][1]] = 3;
		m++;
	}
	nearest_path_array[main_node_array[m][0]][main_node_array[m][1]] = 3;
	
}


// bool detect_outer_wall(int next_yposition,int next_xposition)
// {
// 	if(next_yposition < 0 || next_xposition < 0)
// 	{
// 		return false;
// 	}
// 	else if(map_array[next_yposition][next_xposition] == 5)
// 	{
// 		return true;
// 	}
// 	else if(map_array[next_yposition][next_xposition] == 4)
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// 	
// }

bool detect_path(int next_yposition,int next_xposition, int value) //detects if coordinate is given path coordinate
{
/*	nearest_path_to_array();*/
	if(nearest_path_array[next_yposition][next_xposition] == value)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

// void set_coordinate_in_array(int y, int x, int value) //sets coordinate in array to value
// {
// 	robot_array[y][x] = value;
// }

void set_coordinate_in_NP_array(int y, int x, int value) //sets coordinate in nearest_path_array to value
{
	nearest_path_array[y][x] = value;
}



void drive_nearest_path() //follows given path from gurras_array
{
	uint8_t steps = 0;
	while(detect_path(y_positions_forward(Get_robot_direction(), steps + 1), x_positions_forward(Get_robot_direction(), steps + 1), 3))
	{
		if(detect_path(y_positions_forward(Get_robot_direction(), steps), x_positions_forward(Get_robot_direction(), steps), 3))
		{
			set_coordinate_in_NP_array(y_positions_forward(Get_robot_direction(), steps), x_positions_forward(Get_robot_direction(), steps), 1);
		}
		steps++;
	}

	if(steps > 0)
	{
		Movement_Queue_Put('f');
		Movement_Queue_Put(steps);
	}
	else if(detect_path(right_y_pos(), right_x_pos(), 3))
	{
		Movement_Queue_Put('r');
		Movement_Queue_Put(90);
	}
	else if(detect_path(left_y_pos(), left_x_pos(), 3))
	{
		Movement_Queue_Put('l');
		Movement_Queue_Put(90);
	}
	else
	{
		PORTA = 0xFF;
		Movement_Queue_Put('o');
		nearest_path_driven = true;
	}
	
// 	if(map_array[robot_pos.y_tile][robot_pos.x_tile] == 2)
// 	{
// 		PORTA = 0xFF;
// 		Movement_Queue_Put('o');
// 		nearest_path_driven = true;
// 	}
}


void drive_back_nearest_path() //drives the same way back, drives_nearest_path need to be finished when starting this function
{
	int steps = 0;
	if(robot_pos.y_tile == 14 && robot_pos.x_tile == 14)
	{
		nearest_path_driven_back = true;
		return;
	}
	while(detect_path(y_positions_forward(Get_robot_direction(), steps + 1), x_positions_forward(Get_robot_direction(), steps + 1), 1))
	{
		if(detect_path(y_positions_forward(Get_robot_direction(), steps), x_positions_forward(Get_robot_direction(), steps), 3))
		{
			set_coordinate_in_NP_array(y_positions_forward(Get_robot_direction(), steps), x_positions_forward(Get_robot_direction(), steps), 0);
		}
		
		if(steps > 15)
		{
			nearest_path_driven_back = true;
			Movement_Queue_Put('f');
			Movement_Queue_Put(15);
			return;
		}
		steps++;
	}
	if(steps > 0)
	{
		Movement_Queue_Put('f');
		Movement_Queue_Put(steps);
	}
	else if(detect_path(right_y_pos(), right_x_pos(), 1))
	{
		Movement_Queue_Put('r');
		Movement_Queue_Put(90);
	}
	else if(detect_path(left_y_pos(), left_x_pos(), 1))
	{
		//robot_turn_left();
		Movement_Queue_Put('l');
		Movement_Queue_Put(90);
	}
	else if(detect_path(back_y_pos(), back_x_pos(), 1))
	{
		Movement_Queue_Put('l');
		Movement_Queue_Put(180);
	}

}

// void drive_to_distressed_and_return()
// {
// 	if(finished_with_drive_nearest_path)
// 	{
// 		//leave something for distressed
// 		drive_back_nearest_path();
// 	}
// 	else
// 	{
// 		drive_nearest_path();
// 	}
// }

void robot_detect_outer_walls()
{
	fake_robot_turn_left();
	fake_robot_move();
	while (map_array[next_y_position(direction)][next_x_position(direction)] != 4)
	{
		save_detected_outer_wall_in_array(ypos, xpos);
		if(detect_outer_wall(right_y_pos(), right_x_pos()))
		{
			fake_robot_turn_right();
			fake_robot_move();
		}
		else if(detect_outer_wall(next_y_position(direction), next_x_position(direction)))
		{
			fake_robot_move();
		}
		else if(detect_outer_wall(left_y_pos(), left_x_pos()))
		{
			fake_robot_turn_left();
		}
		else if(detect_outer_wall(forward_to_left_y_pos(), forward_to_left_x_pos()))
		{
			fake_robot_move();
			fake_robot_turn_left();
			fake_robot_move();
			save_detected_outer_wall_in_array(ypos, xpos);
			fake_robot_turn_left();
			fake_robot_turn_left();
			if(detect_outer_wall(forward_to_left_y_pos(), forward_to_left_x_pos()))
			{
				fake_robot_move();
				fake_robot_turn_left();
				fake_robot_move();
				save_detected_outer_wall_in_array(ypos, xpos);
			}
			else
			{
				fake_robot_turn_left();
				fake_robot_turn_left();
			}
			
		}
	}
	save_detected_outer_wall_in_array(ypos, xpos);
	find_unknown_cells_after_detection_of_outer_walls();
}

void find_unknown_cells_after_detection_of_outer_walls() //finds unknowns cells inside detected outer walls after robot_keep_right and sets them to 1 inside the undiscovered_tiles_array
{
	int m = 0; //use robot array instead of map array when working
	while(m < 29)
	{
		int n = 0;
		while(n < 29)
		{
			if(undiscovered_tiles_array[m][n] == 6)
			{
				bool last_6 = false;
				int n2 = n;
				while((undiscovered_tiles_array[m][n2+1] != 6) && !last_6)
				{
					if(n2 > 28)
					{
						last_6 = true;
					}
					else
					{
						n2++;
					}
				}
				while((n <= n2) && !last_6)
				{
					if(map_array[m][n+1] == 0)
					{
						if(m == 0 || m == 28)
						{
							map_array[m][n+1] = 0; //changed array here to map_array from undisc...
						}
						else
						{
							map_array[m][n+1] = 1; //undetected space //changed array here to map_array from undisc...
						}
					}
					else if(map_array[m][n+1] == 1)
					{
						undiscovered_tiles_array[m][n+1] = 1; //free space and undetected space is the same now as ordered by GURRA STRANDBERG
					}
					else if(map_array[m][n+1] == 5 && undiscovered_tiles_array[m][n+1] > 0)
					{
						undiscovered_tiles_array[m][n+1] = 6; //outer wall
					}
					else if(map_array[m][n+1] == 5)
					{
						undiscovered_tiles_array[m][n+1] = 5; //inner wall
					}
					else if(map_array[m][n+1] == 4)
					{
						undiscovered_tiles_array[m][n+1] = 4;
					}
					n++;
				}
			}
			n++;
		}
		m++;
	}
}
