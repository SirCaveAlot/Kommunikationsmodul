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

uint8_t robot_array[29][29];

uint8_t undiscovered_tiles_array[29][29];
//always turning right in relation to where it's heading

// void nearest_path_to_array() //takes the coordinates from gurras_array and puts it in nearest_path_array[29][29]
// {
// 	int m = 0;
// 	while(gurras_array[m][2] != 0)
// 	{
// 		nearest_path_array[gurras_array[m][0]][gurras_array[m][1]] = 3;
// 		m++;
// 	}
// 	nearest_path_array[gurras_array[m][0]][gurras_array[m][1]] = 3;
// 	
// }


//back at start position?
bool back_at_start(int yposition,int xposition)
{
	if(map_array[yposition][xposition] == 3)
	{
		turn_around(); //turn around (assuming start position was up)
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

// bool detect_path(int next_yposition,int next_xposition, int value) //detects if coordinate is given path coordinate
// {
// 	if(nearest_path_array[next_yposition][next_xposition] == value)
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// 	
// }

void robot_move() //moves robot one coordinate forward
{
	xpos = next_x_position(direction);
	ypos = next_y_position(direction);
}

void robot_move_distance(int number_of_steps)
{
	for(int i = 0; i < number_of_steps; i++)
	{
		robot_move();
	}
}

void set_coordinate_in_array(int y, int x, int value) //sets coordinate in array to value
{
	robot_array[y][x] = value;
}

// void set_coordinate_in_NP_array(int y, int x, int value) //sets coordinate in nearest_path_array to value
// {
// 	nearest_path_array[y][x] = value;
// }

//As long as we don't find a wall ahead or a clear at the right hand side, the robot keeps moving forward
//BUT! if we first off, find a clear way to travel to the right, we turn right and if we find a wall we
//clearly have to turn left
void robot_keep_right()
{
	if(!back_at_start(Get_robot_tile_y(), Get_robot_tile_x()))
	{
		if(!running && Movement_queue_empty())
		{
			if ((front_side_detected || map_array[next_y_position(Get_robot_direction())][next_x_position(Get_robot_direction())] == 2) && left_side_detected && right_side_detected)
			{   
				Calibrate_robot_position();
				Movement_Queue_Put('r');
				Movement_Queue_Put(180);
				Movement_Queue_Put('f');
				Movement_Queue_Put(15);
			}
			else if ((front_side_detected || map_array[next_y_position(Get_robot_direction())][next_x_position(Get_robot_direction())] == 2) && right_side_detected)
			{
				Calibrate_robot_position();
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
// 			if(!front_side_detected && !left_side_detected && right_side_detected) //as long as we have wall to the right
// 			{
// 				Movement_Queue_Put('f');
// 				Movement_Queue_Put(15);
// 			}
// 			else if(front_side_detected && !left_side_detected && right_side_detected)
// 			{
// 				Movement_Queue_Put('l');
// 				Movement_Queue_Put(90);
// 				Movement_Queue_Put('f');
// 				Movement_Queue_Put(15);
// 			}
// 			else if(left_side_detected && right_side_detected)
// 			{
// 				Movement_Queue_Put('r');
// 				Movement_Queue_Put(180);
// 				Movement_Queue_Put('f');
// 				Movement_Queue_Put(15);
// 			}
// 			else if(!right_side_detected)
// 			{
// 				Movement_Queue_Put('L');
// 				Movement_Queue_Put('r');
// 				Movement_Queue_Put(90);
// 				Movement_Queue_Put('f');
// 				Movement_Queue_Put(15);
// 			}
		}
	}
	else
	{
		//robot_detect_outer_walls(); // THIS NEEDS TO BE RUN WITHOUT ACTUALLY CHANGING ROBOTS POSITION
		competition_mode = 2;
		USART_Transmit('C', 0);
	}
}

// bool drive_nearest_path() //follows given path from gurras_array
// {
// 	if(detect_path(next_y_position(direction), next_x_position(direction), 3))
// 	{
// 		set_coordinate_in_NP_array(ypos, xpos, 1); // if robot move is outside of while loop in main function, setting coordinate
// 		robot_move();							   // here will save STARTING and end POSITION as 3 and set rest of path to 1.
// 		return false;
// 		
// 	}
// 	else if(detect_path(right_y_pos(), right_x_pos(), 3))
// 	{
// 		set_coordinate_in_NP_array(ypos, xpos, 1);
// 		robot_turn_right();
// 		return false;
// 	}
// 	else if(detect_path(left_y_pos(), left_x_pos(), 3))
// 	{
// 		set_coordinate_in_NP_array(ypos, xpos, 1);
// 		robot_turn_left();
// 		return false;
// 	}
// 	else
// 	{
// 		finished_with_drive_nearest_path = true;
// 		return true;
// 	}
// 
// }


// bool drive_back_nearest_path() //drives the same way back, drives_nearest_path need to be finished when starting this function
// {
// 	if(detect_path(next_y_position(direction), next_x_position(direction), 1))
// 	{
// 		set_coordinate_in_NP_array(ypos, xpos, 2);
// 		robot_move();
// 		return false;
// 		
// 	}
// 	else if(detect_path(right_y_pos(), right_x_pos(), 1))
// 	{
// 		set_coordinate_in_NP_array(ypos, xpos, 2);
// 		robot_turn_right();
// 		return false;
// 	}
// 	else if(detect_path(left_y_pos(), left_x_pos(), 1))
// 	{
// 		set_coordinate_in_NP_array(ypos, xpos, 2);
// 		robot_turn_left();
// 		return false;
// 	}
// 	else if(detect_path(back_y_pos(), back_x_pos(), 1))
// 	{
// 		set_coordinate_in_NP_array(ypos, xpos, 2);
// 		robot_turn_right();
// 		robot_turn_right();
// 		return false;
// 	}
// 	else
// 	{
// 		return true;
// 	}
// 
// }
// 
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
	turn_left();
	robot_move();
	while (map_array[next_y_position(direction)][next_x_position(direction)] != 4)
	{
		save_detected_outer_wall_in_array(ypos, xpos);
		if(detect_outer_wall(right_y_pos(), right_x_pos()))
		{
			turn_right();
			robot_move();
		}
		else if(detect_outer_wall(next_y_position(direction), next_x_position(direction)))
		{
			robot_move();
		}
		else if(detect_outer_wall(left_y_pos(), left_x_pos()))
		{
			turn_left();
		}
		else if(detect_outer_wall(forward_to_left_y_pos(), forward_to_left_x_pos()))
		{
			robot_move();
			turn_left();
			robot_move();
			save_detected_outer_wall_in_array(ypos, xpos);
			turn_around();
			if(detect_outer_wall(forward_to_left_y_pos(), forward_to_left_x_pos()))
			{
				robot_move();
				turn_left();
				robot_move();
				save_detected_outer_wall_in_array(ypos, xpos);
			}
			else
			{
				turn_around();
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
				bool last_4 = false;
				int n2 = n;
				while((undiscovered_tiles_array[m][n2+1] != 4) && !last_4)
				{
					if(n2 > 28)
					{
						last_4 = true;
						while((n <= n2) && last_4)
						{
							undiscovered_tiles_array[m][n+1] = 4;
							n++;
						}
					}
					else
					{
						n2++;
					}
				}
				while((n <= n2) && !last_4)
				{
					if(map_array[m][n+1] == 0)
					{
						if(m == 0 || m == 28)
						{
							undiscovered_tiles_array[m][n+1] = 4;
						}
						else
						{
							undiscovered_tiles_array[m][n+1] = 0; //undetected space
						}
					}
					else if(map_array[m][n+1] == 1)
					{
						undiscovered_tiles_array[m][n+1] = 1; //free space
					}
					else if(map_array[m][n+1] == 5 && undiscovered_tiles_array[m][n+1] > 0)
					{
						undiscovered_tiles_array[m][n+1] = 4; //outer wall
					}
					else if(map_array[m][n+1] == 5)
					{
						undiscovered_tiles_array[m][n+1] = 5; //inner wall
					}
					else if(map_array[m][n+1] == 3)
					{
						undiscovered_tiles_array[m][n+1] = 3;
					}
					n++;
				}
			}
			n++;
		}
		m++;
	}
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

void save_detected_walls_in_array() //saves detected walls  as 5 and detected free space as 1 in array
{
	set_coordinate_in_array(ypos, xpos, 1);
	//if(right_distance < 150)
	if(map_array[right_y_pos()][right_x_pos()] >= 5)
	{
		set_coordinate_in_array(right_y_pos(), right_x_pos(), 5);
	}
	//if(left_distance < 150)
	if(map_array[left_y_pos()][left_x_pos()] >= 5)
	{
		set_coordinate_in_array(left_y_pos(), left_x_pos(), 5);
	}
	if(map_array[right_y_pos()][right_x_pos()] == 1)
	{
		set_coordinate_in_array(right_y_pos(), right_x_pos(), 1);
	}
	if(map_array[left_y_pos()][left_x_pos()] == 1)
	{
		set_coordinate_in_array(left_y_pos(), left_x_pos(), 1);
	}
}

void save_detected_outer_wall_in_array(int y, int x) //saves detected outer walls as 4
{
	undiscovered_tiles_array[y][x] = 4;
}