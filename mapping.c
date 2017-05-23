/*
 * GccApplication2.c
 *
 * Created: 2017-04-19 18:24:06
 * Author : hyggan
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


#define val M_PI/180
#define Marcus 360/1000
#define marcus_to_radian M_PI/500

#define WHEEL_DIAMETER 63.34
#define WHEEL_CIRCUMFERENCE (WHEEL_DIAMETER * M_PI)
#define WHEEL_SLICE (WHEEL_CIRCUMFERENCE/16)

volatile bool right_side_detected;
volatile bool left_side_detected;
volatile bool front_side_detected;


int size = 4000;
int array_x[10];
int array_y[10];

/*
void Calibrate_robot_position()
{
	for(int i=0; i < 29; i++)
	{
		if((robot_pos.x/10 > line_array_x[i] ) && (robot_pos.x/10 < line_array_x[i+1]))
		{
			robot_pos.x = (line_array_x[i] + line_array_x[i+1])/2;
		}
	}
	for(int i=0; i < 28; i++)
	{
		if((robot_pos.y/10 > line_array_y[i] ) && (robot_pos.y/10 < line_array_y[i+1]))
		{
			robot_pos.y = (line_array_y[i] + line_array_y[i+1])/2;
		}
	}
}*/

void Calibrate_robot_position()
{
	robot_pos.x = round((float)robot_pos.x / 400) * 400;
	robot_pos.y = round((float)robot_pos.y / 400) * 400;
	
}



void Set_robot_position(int16_t xpos, int16_t ypos)
{
	robot_pos.x = xpos;
	robot_pos.y = ypos; 
}

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

uint8_t Get_robot_direction()
{
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

uint8_t Wheelshifts_to_distance(uint8_t nr_of_wheel_shifts)
{
// 	if(last_movement == 'b')
// 	{
// 		return -WHEEL_SLICE * nr_of_wheel_shifts;
// 	}
	return round(WHEEL_SLICE*nr_of_wheel_shifts);
}

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
	/*
	uint8_t return_val = round((float) ((robot_pos.x / 10) + 560) / 40 ); 
	if (return_val > 127)
	{
		return 0;
	}
	*/
	return x_tile_matrix;
}



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
	
	/*uint8_t return_val = round((float) ((robot_pos.y / 10) + 560) / 40 );
	if (return_val > 127)
	{
		return 0;
	}*/
	
	return y_tile_matrix;
}

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

void Left_side_detectable(uint8_t IR_data)
{
	if(IR_data >= 67)
	{
		left_side_detected = true;
	}
	else
	{
		left_side_detected = false;
	}
}

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

void Set_tile_from_ir()
{
	Set_tile(robot_pos.x_tile, robot_pos.y_tile, 1);
	
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
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 1); // Check tile value. Open tile
		}
		if(right_side_detected)
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 1); // Check tile value. Open tile
		}
		
		if(front_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 1); // Check tile value. Open tile
		}
	}
	
	else if(direction_tile_robot == 6) // direction right
	{
		if(left_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 1); // Check tile value. Open tile
		}
		if(right_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 1); // Check tile value. Open tile
		}
		
		if(front_side_detected)
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 1); // Check tile value. Open tile
		}
		
	}
	
	else if(direction_tile_robot == 2) // direction down
	{
		if(left_side_detected)
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile + 1, robot_pos.y_tile, 1); // Check tile value. Open tile
		}
		if(right_side_detected)
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 1); // Check tile value. Open tile
		}
		
		if(front_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 1); // Check tile value. Open tile
		}		
	}
	
	else if(direction_tile_robot == 4) // direction left
	{
		if(left_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile + 1, 1); // Check tile value. Open tile
		}
		if(right_side_detected)
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile, robot_pos.y_tile - 1, 1); // Check tile value. Open tile
		}
		
		if(front_side_detected)
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 255); // Check tile value. Wall tile
		}
		else
		{
			Set_tile(robot_pos.x_tile - 1, robot_pos.y_tile, 1); // Check tile value. Open tile
		}		
	}
}


void Set_peepz_in_da_needz()
{
	/*
	uint8_t peep_x = robot_pos.x % 40;
	uint8_t peep_y = robot_pos.y % 40;*/
	
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


//heloooooooooooooooooooooooooooooo

//int i;
//int theta;

/*
 //------------------------------------Testcase 1 (13,14) - funkar   -----------------------------------------
int size= 5;
double rcv_radius[5]={28.28,24,20,24,35.28};
double rcv_theta[5]={135,155,180,205,225};
double x_coordinates[5];
double y_coordinates[5];
int array_x[5];
int array_y[5];
void Test_values(){}


 //------------------------------------Testcase 2 - (15,14) increase 0 -funkar -----------------------------------------
 int size= 5;
 double rcv_radius[5]={28.28,24,20,24,35.28};
 double rcv_theta[5]={315,345,0,15,45};
 double x_coordinates[5];
 double y_coordinates[5];
 int array_x[5];
 int array_y[5];
 void Test_values(){}



//------------------------------------Testcase 3 - (14,13) increase= 90 - funkar-----------------------------------------
 
//------------------------------------Testcase 4 - (14,13) increase= 180 - funkar -----------------------------------------
 
//------------------------------------Testcase 5 - (14,13) increase= 270 -funkar -----------------------------------------
  
//------------------------------------Testcase 6 - en array p 20 element alla med (14,14), (14,13), (13,14), (14,15) -funkar -----------------------------------------

//------------------------------------Testcase 7- kollar att om vi har ett hrn skapar vi ingen linje (ingen tile lggs till)  -----------------------------------------

//------------------------------------Testcase 8- kollar att om vi har ett hrn s skapar vi ingen linje (ingen tile lggs till)  -----------------------------------------

#define increase 90
int size= 20;
double rcv_radius[20]={28.28,24,20,24,35.28,
	 28.28,24,20,24,35.28,
	 28.28,24,20,24,35.28,
	 28.28,24,20,24,35.28};
double rcv_theta[20]={315,345,0,15,45,
	315+2*increase,345+2*increase,0+2*increase,15+2*increase,45+2*increase 
	,315+3*increase,345+3*increase,0+3*increase,15+3*increase,45+3*increase 
	,315+increase,345+increase,0+increase,15+increase,45+increase};

double x_coordinates[20];
double y_coordinates[20];
int array_x[20];
int array_y[20];
void Test_values(){}
*/

/*#define increase 90*/

// double rcv_radius[5] = {28.28,24,20,24,35.28};
// double rcv_theta[5] = {315,345,0,15,45};

// double x_coordinates[5];
// double y_coordinates[5];

// void DegreeToRadian(uint8_t array[])
// {
// 	for(int i = 0; i < size ; i + 1)
// 	{
// 		array[i] = marcus_to_radian * (array[i] + 250);
// 	}
// 	
// } 

// 
// void PolarToCartesian(uint16_t radius_array[], uint16_t theta_array[])
// {
// 	for(int i = 0; i < size; i + 2)
// 	{
// 			 = (radius_array[i] << 8 | radius_array[i + 1]) * 
// 							(cos(theta_array[i] << 8 | theta_array [i + 1]));
// 			
// 			 = (radius_array[i] << 8 | radius_array[i + 1]) * 
// 							(sin(theta_array[i] << 8 | theta_array [i + 1]));
// 	}
// }


// void RelativeToEffective (struct coordinates robot_position)
// {	
// 	for(int i = 0; i < size ; i++)
// 	{
// 		x_coordinates[i] = x_coordinates[i] + robot_position.x;
// 		
// 		y_coordinates[i] = y_coordinates[i] + robot_position.y;	
// 	}
// }



void Window ()
{
	//Tar ut ett fnster p ett visst antal element och gr en vekotr av dem
	uint16_t vector_position = 0;
	// Om det finns mindre plats än window_size, ta bar ett fönster de element som finns kvar
	for(int index = 0; index < size - window_size; index = index + 4)
	{
		for(int i = 0; i < 2 * window_size; i++)
		{
			vector_position = index + i;
			if (i % 2 == 0)
			{
				array_x[i / 2] = (int) (distance_array[vector_position] << 8 | distance_array[vector_position + 1]) *
				(cos(((angle_array[vector_position] << 8 | angle_array[vector_position + 1]) - 6 * 1000 / 360) *
				 marcus_to_radian + (M_PI / 2) + (robot_pos.angle))) 
				+ robot_pos.x/10;
				array_y[i / 2] = (int) (distance_array[vector_position] << 8 | distance_array[vector_position + 1]) *
				(sin(((angle_array[vector_position] << 8 | angle_array[vector_position + 1]) - 6 * 1000 / 360) * 
				marcus_to_radian + (M_PI / 2) + (robot_pos.angle)))
				 + robot_pos.y/10;
			}
		}
		
		Update_map(array_x, array_y);
	}
}


// void Cartesian_window (uint8_t radius_array[], uint8_t theta_array[], int window_size, int size)
//  {
// 	//DegreeToRadian(theta_array);
// 	//PolarToCartesian(radius_array, theta_array);
// 	//RelativeToEffective(robot_position);
// 	Window();
//  }


// int testmapArrayTileGet1;
// int testmapArrayTileGet2;
// int testmapArrayTileGet3;
// int testmapArrayTileGet4;
/*
int testBestmatch_x;
int testbest_match_y;
int testnext_best_match_y;
int testMatch_tile_x;
int testMatch_tile_y;
int testX_lineBool;

int xArrayTest[5] = {-19, -21, -19, -21, -24};
int yArrayTest[5] = {19, 10, 0, -10, -24};

	
int testX_line(int x[],int y[]){
	if(X_line(x,y))
	{
	return 123;
	}
	else
	{
	return 456;
	}
}*/

// int main(void)
// {	
// 	Test_values();
// 	robot_pos.x = 0;
// 	robot_pos.y = 0;
// 	
// 	/*
// 	testX_lineBool=testX_line(xArrayTest,yArrayTest);
// 	testBestmatch_x=match_x(xArrayTest);
// 	testbest_match_y=match_y(yArrayTest);
// 	testnext_best_match_y=Match_y_next(yArrayTest,testbest_match_y);
// 	testMatch_tile_x=Match_tile_x(xArrayTest,yArrayTest);
// 	testMatch_tile_y=Match_tile_y(xArrayTest,yArrayTest);*/
// 	
// 	Cartesian_window (robot_pos,  rcv_radius, rcv_theta,3, size);
// 	testmapArrayTileGet1 = Get_tile(15,14,28,29,map_array);
// 	testmapArrayTileGet2 = Get_tile(14,13,28,29,map_array);
// 	testmapArrayTileGet3 = Get_tile(13,14,28,29,map_array);
// 	testmapArrayTileGet4 = Get_tile(14,15,28,29,map_array);
// 	
// 
// 
// }