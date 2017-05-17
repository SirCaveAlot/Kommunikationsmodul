/*
 * Positioning.c
 *
 * Created: 5/17/2017 8:31:50 AM
 *  Author: guswe541
 */ 


#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include "SearchAndFind.h"

#define up 8
#define down 2
#define left 4
#define right 6

int8_t xpos;
int8_t ypos;
int8_t direction;


//changing robot direction to given direction
void robot_set_direction(int8_t next_direction)
{
	direction = next_direction;
}

//always turning right in relation to where it's heading
void robot_turn_right()
{
	if(direction == up)
	{
		robot_set_direction(right);
	}
	else if(direction == right)
	{
		robot_set_direction(down);
	}
	else if(direction == down)
	{
		robot_set_direction(left);
	}
	else if(direction == left)
	{
		robot_set_direction(up);
	}
}

//always turning left in relation to where it's heading
void robot_turn_left()
{
	if(direction ==  up)
	{
		robot_set_direction(left);
	}
	else if(direction == right)
	{
		robot_set_direction(up);
	}
	else if(direction == down)
	{
		robot_set_direction(right);
	}
	else if(direction == left)
	{
		robot_set_direction(down);
	}
}

void robot_turn_around()
{
	if(direction ==  up)
	{
		robot_set_direction(down);
	}
	else if(direction == right)
	{
		robot_set_direction(left);
	}
	else if(direction == down)
	{
		robot_set_direction(up);
	}
	else if(direction == left)
	{
		robot_set_direction(right);
	}
}

//calculates next pos (general)
int8_t next_y_position(int8_t next_direction)
{
	int8_t next_ypos;
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

int8_t next_x_position(int8_t next_direction)
{
	int8_t next_xpos;
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

//position to the right of the robot x coordinates
int8_t right_x_pos()
{
	int8_t right_x_pos;
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

//position to the right of the robot y coordinates
int8_t right_y_pos()
{
	int8_t right_y_pos = ypos;
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

//position to the left of the robot x coordinates
int8_t left_x_pos()
{
	int8_t left_x_pos = xpos;
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

//position to the left of the robot y coordinates
int8_t left_y_pos()
{
	int8_t left_y_pos = ypos;
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

int8_t back_x_pos()
{
	int8_t back_x_pos = xpos;
	if(direction == up)
	{
		back_x_pos = xpos;
	}
	else if(direction == down)
	{
		back_x_pos = xpos;
	}
	else if(direction == left)
	{
		back_x_pos = xpos + 1;
	}
	else if(direction == right)
	{
		back_x_pos = xpos - 1;
	}
	return back_x_pos;
}

int8_t back_y_pos()
{
	int8_t back_y_pos = ypos;
	if(direction == up)
	{
		back_y_pos = ypos + 1;
	}
	else if(direction == down)
	{
		back_y_pos = ypos - 1;
	}
	else if(direction == left)
	{
		back_y_pos = ypos;
	}
	else if(direction == right)
	{
		back_y_pos = ypos;
	}
	return back_y_pos;
}

int8_t forward_to_left_x_pos()
{
	int8_t forward_to_left_x_pos = xpos;
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

int8_t forward_to_left_y_pos()
{
	int8_t forward_to_left_y_pos = ypos;
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


