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
#include "mapping.h"
#include "global_variables.h"
//always turning right in relation to where it's heading

void robot_turn_right()
{
	if(Get_robot_direction() == up)
	{
		Set_robot_angle_direction(right);
	}
	else if(Get_robot_direction() == right)
	{
		Set_robot_angle_direction(down);
	}
	else if(Get_robot_direction() == down)
	{
		Set_robot_angle_direction(left);
	}
	else if(Get_robot_direction() == left)
	{
		Set_robot_angle_direction(up);
	}
}

//always turning left in relation to where it's heading
void robot_turn_left()
{
	if(Get_robot_direction() ==  up)
	{
		Set_robot_angle_direction(left);
	}
	else if(Get_robot_direction() == right)
	{
		Set_robot_angle_direction(up);
	}
	else if(Get_robot_direction() == down)
	{
		Set_robot_angle_direction(right);
	}
	else if(Get_robot_direction() == left)
	{
		Set_robot_angle_direction(down);
	}
}

void robot_turn_around()
{
	if(Get_robot_direction() ==  up)
	{
		Set_robot_angle_direction(down);
	}
	else if(Get_robot_direction() == right)
	{
		Set_robot_angle_direction(left);
	}
	else if(Get_robot_direction() == down)
	{
		Set_robot_angle_direction(up);
	}
	else if(Get_robot_direction() == left)
	{
		Set_robot_angle_direction(right);
	}
}