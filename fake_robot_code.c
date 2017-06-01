/*
 * fake_robot_code.c
 *
 * Created: 5/22/2017 6:38:56 PM
 *  Author: jakpa844
 */ 

//THIS CODE IS USED TO FIND OUTER WALLS. HERE CHANGES IN DIRECTIONS AND POSITIONS DOES NOT AFFECT THE ACTUAL ROBOT
//IT ONLY AFFECTS AN IMAGINARY ROBOT. THIS CODE IS NOT USED ANYWHERE YET

#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include "global_variables.h"
#include "SearchAndFind.h"

#define up 8
#define down 2
#define left 4
#define right 6

int xpos;
int ypos;
int direction;

uint8_t undiscovered_tiles_array[29][29];
//always turning right in relation to where it's heading


//changing robot direction to given direction
void fake_robot_set_direction(int next_direction)
{
	direction = next_direction;
}


//always turning right in relation to where it's heading
void fake_robot_turn_right()
{
	if(direction == up)
	{
		fake_robot_set_direction(right);
	}
	else if(direction == right)
	{
		fake_robot_set_direction(down);
	}
	else if(direction == down)
	{
		fake_robot_set_direction(left);
	}
	else if(direction == left)
	{
		fake_robot_set_direction(up);
	}
}

//always turning left in relation to where it's heading
void fake_robot_turn_left()
{
	if(direction ==  up)
	{
		fake_robot_set_direction(left);
	}
	else if(direction == right)
	{
		fake_robot_set_direction(up);
	}
	else if(direction == down)
	{
		fake_robot_set_direction(right);
	}
	else if(direction == left)
	{
		fake_robot_set_direction(down);
	}
}

//calculates next pos (general)
int fake_next_y_position(int next_direction)
{
	int next_ypos;
	next_ypos = ypos;
	
	if(next_direction == up)
	{
		next_ypos--;
	}
	else if(next_direction == down)
	{
		next_ypos++;
	}
	return next_ypos;
}

int fake_next_x_position(int next_direction)
{
	int next_xpos;
	next_xpos = xpos;
	
	if(next_direction == right)
	{
		next_xpos++;
	}
	else if(next_direction == left)
	{
		next_xpos--;
	}
	return next_xpos;
}

void fake_robot_move() //moves fake robot one coordinate forward
{
	xpos = fake_next_x_position(direction);
	ypos = fake_next_y_position(direction);
}


//position to the right of the fake robot x coordinates
int fake_right_x_pos()
{
	int right_x_pos;
	right_x_pos = xpos;
	if(direction == up)
	{
		right_x_pos = xpos +1;
	}
	else if(direction == down)
	{
		right_x_pos = xpos -1;
	}
	else if(direction == left)
	{
		right_x_pos = xpos;
	}
	else if(direction == right)
	{
		right_x_pos = xpos;
	}
	return right_x_pos;
}

//position to the right of the fake robot y coordinates
int fake_right_y_pos()
{
	int right_y_pos = ypos;
	if(direction == up)
	{
		right_y_pos = ypos;
	}
	else if(direction == down)
	{
		right_y_pos = ypos;
	}
	else if(direction == left)
	{
		right_y_pos = ypos -1;
	}
	else if(direction == right)
	{
		right_y_pos = ypos +1;
	}
	return right_y_pos;
}

//position to the left of the fake robot x coordinates
int fake_left_x_pos()
{
	int left_x_pos = xpos;
	if(direction == up)
	{
		left_x_pos = xpos -1;
	}
	else if(direction == down)
	{
		left_x_pos = xpos +1;
	}
	else if(direction == left)
	{
		left_x_pos = xpos;
	}
	else if(direction == right)
	{
		left_x_pos = xpos;
	}
	return left_x_pos;
}

//position to the left of the fake robot y coordinates
int fake_left_y_pos()
{
	int left_y_pos = ypos;
	if(direction == up)
	{
		left_y_pos = ypos;
	}
	else if(direction == down)
	{
		left_y_pos = ypos;
	}
	else if(direction == left)
	{
		left_y_pos = ypos +1;
	}
	else if(direction == right)
	{
		left_y_pos = ypos -1;
	}
	return left_y_pos;
}

int fake_forward_to_left_x_pos()
{
	int forward_to_left_x_pos = xpos;
	if(direction == up)
	{
		forward_to_left_x_pos = xpos -1;
	}
	else if(direction == down)
	{
		forward_to_left_x_pos = xpos +1;
	}
	else if(direction == left)
	{
		forward_to_left_x_pos = xpos -1;
	}
	else if(direction == right)
	{
		forward_to_left_x_pos = xpos +1;
	}
	return forward_to_left_x_pos;
}

int fake_forward_to_left_y_pos()
{
	int forward_to_left_y_pos = ypos;
	if(direction == up)
	{
		forward_to_left_y_pos = ypos -1;
	}
	else if(direction == down)
	{
		forward_to_left_y_pos = ypos +1;
	}
	else if(direction == left)
	{
		forward_to_left_y_pos = ypos +1;
	}
	else if(direction == right)
	{
		forward_to_left_y_pos = ypos -1;
	}
	return forward_to_left_y_pos;
}


bool fake_detect_outer_wall(int next_yposition,int next_xposition)
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
	

void save_detected_outer_wall_in_array(int y, int x) //saves detected outer walls  as 6
{
	undiscovered_tiles_array[y][x] = 6;
}

