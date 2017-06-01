/*
 * mapping.c
 *
 * Created: 5/11/2017 10:56:34 AM
 *  Author: Andreas Andersson, andan879
 */ 

	
#include <avr/io.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "mapping.h"
#include "matchtile.h"
#include "global_variables.h"

/*Definitions of constants*/
#define val M_PI/180
#define Marcus 360/1000
#define marcus_to_radian M_PI/500
#define WHEEL_DIAMETER 63.34
#define WHEEL_CIRCUMFERENCE (WHEEL_DIAMETER * M_PI)
#define WHEEL_SLICE (WHEEL_CIRCUMFERENCE/16)

/*Bools that are true if the robot has walls to the right,
left or front*/
volatile bool right_side_detected;
volatile bool left_side_detected;
volatile bool front_side_detected;

/*2000 distances from the laser + 2000 angles from the rotating laser tower*/
int size = 4000;

/*Arrays used for every group of (cartesian) coordinates from the rotating laser*/
int array_x[10];
int array_y[10];

/*Updates the robot position by rounding the internal coordinates to the closest
possible tile coordinates. Prevents the robot position errors from accumulating,
by running this function every time the robot stops in the middle of a tile*/
void Calibrate_robot_position()
{
	robot_pos.x = round((float)robot_pos.x / 400) * 400;
	robot_pos.y = round((float)robot_pos.y / 400) * 400;
	
}


/*Set:er for the robot position*/
void Set_robot_position(int16_t xpos, int16_t ypos)
{
	robot_pos.x = xpos;
	robot_pos.y = ypos; 
}

/*Set:er for the robot direction
set direction 8 -> angle: 0 radians
set directoin 4 -> angle: pi/2 radians
and so on*/
void Set_robot_angle_direction(uint8_t direction)
{
	if(direction==8)
	{
		robot_pos.angle = 0;
	}
	else if(direction==4)
	{
		robot_pos.angle = M_PI/2;
	}
	else if(direction==2)
	{
		robot_pos.angle = M_PI;
	}
	else if(direction==6)
	{
		robot_pos.angle = 3*M_PI/2;
	}
}

/*Get:er for the robot direction
angle: 0 radians -> direction 8  
angle: pi/2 radians -> directoin 4  
and so on*/
uint8_t Get_robot_direction()
{
	/*returns 66 if something is wrong with
	the internal direction of the robot*/
	uint8_t direction = 66;
	
	if(robot_pos.angle == 0)
	{
		direction = 8;
	}
	else if(robot_pos.angle == M_PI/2)
	{
		direction = 4;
	}
	else if(robot_pos.angle == M_PI)
	{
		direction = 2;
	}
	else if(robot_pos.angle == 3*M_PI/2)
	{
		direction = 6;
	}
	return direction;
}

/*returns the traveled distance of the robot, using the 
number of color shifts of the robot wheel pattern*/
uint8_t Wheelshifts_to_distance(uint8_t nr_of_wheel_shifts)
{
	return round(WHEEL_SLICE*nr_of_wheel_shifts);
}

/*Updates the robot position using the calculated traveled distance*/
void update_robot_position(uint16_t dist_traveled_mm)
{
	if(robot_pos.angle == 0)
	{   
		robot_pos.y = robot_pos.y + dist_traveled_mm;
	}
	else if(robot_pos.angle == M_PI/2)
	{
		robot_pos.x = robot_pos.x - dist_traveled_mm;
	}
	else if(robot_pos.angle == M_PI)
	{
		robot_pos.y = robot_pos.y - dist_traveled_mm;
	}
	else if(robot_pos.angle == 3*M_PI/2)
	{
		robot_pos.x = robot_pos.x + dist_traveled_mm;
	}
	
	robot_pos.x_tile = Get_robot_tile_x();
	robot_pos.y_tile = Get_robot_tile_y();
	
	Set_tile(robot_pos.x_tile, robot_pos.y_tile, 1);
}
/*Get:er for the robot x tile coordinate (returning the matrix coordinate)*/
uint8_t Get_robot_tile_x()
{
	int x_tile_cm;
	uint8_t x_tile_matrix = 255;
	
	for(int i=0; i < 29; i++)
	{
		if((robot_pos.x/10 > line_array_x[i] ) && (robot_pos.x/10 < line_array_x[i+1]))
		{
			x_tile_cm = (line_array_x[i] + line_array_x[i+1])/2;
			x_tile_matrix = Convert_rob_loc_map_glob_x(x_tile_cm);
			return x_tile_matrix;	
		}	
	}
	return x_tile_matrix;
}

/*Get:er for the robot y tile coordinate (returning the matrix coordinate)*/
uint8_t Get_robot_tile_y()
{
	int y_tile_cm;
	uint8_t y_tile_matrix = 255;
	
	for(int i=0; i < 28; i++)
	{
		if((robot_pos.y/10 > line_array_y[i] ) && (robot_pos.y/10 < line_array_y[i+1]))
		{
			y_tile_cm = (line_array_y[i] + line_array_y[i+1])/2;
			y_tile_matrix = Convert_rob_loc_map_glob_y(y_tile_cm);
			return y_tile_matrix;
		}
	}
	return y_tile_matrix;
}

/*Updates bool if there is a wall to the right*/
void Right_side_detectable(uint8_t IR_data, uint8_t IR_back_data)
{
	if(IR_data >= 67 && IR_back_data >= 67)
	{
		right_side_detected = true;
	}
	else
	{
		right_side_detected = false;
	}
}
/*Updates bool if there is a wall to the left*/
void Left_side_detectable(uint8_t IR_data, uint8_t IR_back_data)
{
	if(IR_data >= 67 && IR_back_data >= 67)
	{
		left_side_detected = true;
	}
	else
	{
		left_side_detected = false;
	}
}
/*Updates bool if there is a wall to the front*/
void Front_side_detectable(uint8_t IR_data)
{
	if(IR_data >= 67)
	{
		front_side_detected = true;
	}
	else
	{
		front_side_detected = false;
	}
}

/*Adds walls into the map matrix, using the IR sensors*/
void Set_tile_from_ir()
{
	/*Sets the robot tile position to non-wall*/
	Set_tile(robot_pos.x_tile, robot_pos.y_tile, 1);
	
	//Don't add walls if the robot isn't located in the center of a tile
    if(((abs(robot_pos.x/10 % 40) > 5) && (abs(robot_pos.x/10 % 40) < 35))  ||
	   ((abs(robot_pos.y/10 % 40) > 5)	&& (abs(robot_pos.y/10 % 40) < 35)))
	{
		return;
	}
		
	uint8_t direction_tile_robot = Get_robot_direction();
	
	if(direction_tile_robot == 8) //direction up
	{
		if(left_side_detected)
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 1); 
			// Check tile value. Open tile
		}
		
		if(right_side_detected)
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 1); 
			// Check tile value. Open tile
		}
		
		if(front_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 1); // 
			Check tile value. Open tile
		}
	}
	
	else if(direction_tile_robot == 6) // direction right
	{
		if(left_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 1); 
			// Check tile value. Open tile
		}
		
		if(right_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 255); 
			// Check tile value. Wall tile
		}
		else 
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 1); 
			// Check tile value. Open tile
		}
		
		if(front_side_detected)
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 1); 
			// Check tile value. Open tile
		}
		
	}
	
	else if(direction_tile_robot == 2) // direction down
	{
		if(left_side_detected)
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 1); 
			// Check tile value. Open tile
		}
		
		if(right_side_detected)
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 1); 
			// Check tile value. Open tile
		}
		
		if(front_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 1); 
			// Check tile value. Open tile
		}		
	}
	
	else if(direction_tile_robot == 4) // direction left
	{
		if(left_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 255); 
			// Check tile value. Wall tile
		}
		else 
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 1); 
			// Check tile value. Open tile
		}
		
		if(right_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 1); 
			// Check tile value. Open tile
		}
		
		if(front_side_detected)
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 255); 
			// Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 1); 
			// Check tile value. Open tile
		}		
	}
}

/*Sets the person in need into the map matrix*/
void Set_peepz_in_da_needz()
{
	uint8_t volatile peep_x = robot_pos.x_tile;
	uint8_t volatile peep_y = robot_pos.y_tile;
	
	if (Get_robot_direction() == 8)
	{
		peep_y = peep_y - 1;
	}
	else if (Get_robot_direction() == 2)
	{
		peep_y = peep_y + 1;
	}
	else if (Get_robot_direction() == 6)
	{
		peep_x = peep_x + 1;
	}
	else if (Get_robot_direction() == 4)
	{
		peep_x = peep_x -1;
	}
	Set_tile(peep_x,peep_y,2);
}

/*Iterates through 2000 distances and angles from the rotating laser
to update the map. Creating groups with 10 points in each.
The groups overlap since the step length between the groups is 4*/

void Window ()
{
	uint16_t vector_position = 0;
	for(int index = 0; index < size - window_size; index = index + 4)
	{
		for(int i = 0; i < 2 * window_size; i++)
		{
			vector_position = index + i;
			if (i % 2 == 0)
			{
				//Creates an array with cartesian x coordinates of measured points 
				//(relative to the robot origin)
				array_x[i / 2] = (int) 
				(distance_array[vector_position] << 8 
				| distance_array[vector_position + 1]) *
				(cos(((angle_array[vector_position] << 8 
				| angle_array[vector_position + 1]) - 6 * 1000 / 360) *
				 marcus_to_radian + (M_PI / 2) + (robot_pos.angle))) + robot_pos.x/10;
				
				//Creates an array with cartesian y coordinates of measured points 
				//(relative to the robot origin)
				array_y[i / 2] = (int) 
				(distance_array[vector_position] << 8 
				| distance_array[vector_position + 1]) *
				(sin(((angle_array[vector_position] << 8
				 | angle_array[vector_position + 1]) - 6 * 1000 / 360) * 
				marcus_to_radian + (M_PI / 2) + (robot_pos.angle))) + robot_pos.y/10;
			}
		}
		
		//Uses the cartesian coordinates to update the map. 
		Update_map(array_x, array_y);
	}
}