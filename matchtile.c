#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "matchtile.h"
#include "global_variables.h"

//Returns length of array
#define SIZE_OF_ARRAY(_array) (sizeof(_array) / sizeof(_array[0]))	 

//X lines
int line_array_x[30]={-580, -540, -500, -460, -420, -380, -340, -300, -260, -220, -180, -140, -100, -60, -20,
20, 60, 100, 140, 180, 220, 260, 300, 340, 380, 420, 460, 500, 540, 580};
//Y lines
int line_array_y[29]={-540, -500, -460, -420, -380, -340, -300, -260, -220, -180, -140, -100, -60, -20,
20, 60, 100, 140, 180, 220, 260, 300, 340, 380, 420, 460, 500, 540, 580};


int16_t avg_x_per_sample;
int16_t avg_y_per_sample;

//test stuff
/*
int groupArrayX[4]={467, 469, 475, 490};
int groupArrayY[4]={301, 298, 296, 305};
*/


int16_t max_avg_dist = 15;
int x_array_size = SIZE_OF_ARRAY(line_array_x);
int y_array_size = SIZE_OF_ARRAY(line_array_y);


int Get_tile(int xcoord, int ycoord)
{
	if((xcoord<0) || (ycoord<0) || (xcoord>29) || (ycoord>29))
	{
		return 1111;
	}
	else
	{
		return map_array[ycoord][xcoord];
	}
}

void Set_tile(uint8_t xcoord, uint8_t ycoord, uint8_t newTile)
{
	if((xcoord<0) || (ycoord < 0) || (xcoord>29) || (ycoord>29))
	{
		return;
	}
	else
	{
		map_array[ycoord][xcoord] = newTile;
	}
}

int Convert_rob_loc_map_glob_x(int coordRobot)
{
	if(coordRobot < -560 || coordRobot > 560)
	{
		return 1111;
	}
	
	return (coordRobot + 560) / 40;
}

int Convert_rob_loc_map_glob_y(int coordRobot)
{
		if(coordRobot < -520 || coordRobot > 560)
		{
			return 1111;
		}
		
		return -(coordRobot - 560) / 40;
}

//Returns the average value of the elements in an array: WORKS!
int avg_array(int a[], int num_elements)
{
	int32_t sum;
	int32_t i;
	int32_t avg;
	sum=0;
	avg=0;
	for (i=0; i<num_elements;i++)
	{
		sum=sum+a[i];
		avg=sum/(i+1);
	}
	return(avg);
}

//Matches a set/group of measured x or y values to the closest x or y line
int match_x()
{
	int16_t temp_x;
	int32_t temp_min_x;
	int32_t bestmatch_x;

	temp_min_x=10000;
	bestmatch_x = 11111;

	
	for(unsigned int i=0; i<x_array_size; ++i)
	{
		temp_x = line_array_x[i];
		if((temp_min_x > abs(avg_x_per_sample - temp_x)) && (abs(avg_x_per_sample - temp_x) < max_avg_dist))
		{
			temp_min_x=abs(avg_x_per_sample-temp_x);
			bestmatch_x = temp_x;
		}
	}
	if(bestmatch_x!=11111)
	{
		return bestmatch_x;
	}
	else
	{
		return 1;
	}
	
}

int match_y()
{
	int16_t temp_y;
	int32_t temp_min_y;
	int32_t best_match_y;
	
	temp_min_y=10000;
	best_match_y = 11111;

	

	for(unsigned int i=0; i<y_array_size; ++i)
	{
		temp_y = line_array_y[i];
		if((temp_min_y > abs(avg_y_per_sample - temp_y)) && (abs(avg_y_per_sample - temp_y) < max_avg_dist))
		{
			temp_min_y=abs(avg_y_per_sample-temp_y);
			best_match_y = temp_y;
		}
	}
	return best_match_y;
}

//Matches a set of measured x or y values to the next closest x or y line

int Match_x_next(int dont_match)
{
	if(dont_match!=1)
	{
		int16_t temp_x;
		int32_t temp_min_x;
		int32_t bestmatch_x;
		
		temp_min_x=10000;
		bestmatch_x = 11111;

		

		for(unsigned int i=0; i<x_array_size; ++i)
		{
			temp_x = line_array_x[i];
			if((temp_min_x > abs(avg_x_per_sample - temp_x)) && (temp_x!=dont_match))
			{
				temp_min_x=abs(avg_x_per_sample-temp_x);
				bestmatch_x = temp_x;
			}
		}
		return bestmatch_x;
	}
	else
	{
		return 1;
	}
	
}

int Match_y_next( int dont_match)
{
	if(dont_match!=1)
	{
		int16_t temp_y;
		int32_t temp_min_y;
		int32_t best_match_y;
		
		temp_min_y=10000;
		best_match_y = 11111;


		for(unsigned int i=0; i<y_array_size; ++i)
		{
			temp_y = line_array_y[i];
			if((temp_min_y > abs(avg_y_per_sample - temp_y)) && (temp_y!=dont_match))
			{
				temp_min_y=abs(avg_y_per_sample-temp_y);
				best_match_y = temp_y;
			}
		}
		return best_match_y;
	}
	else
	{
		return 1;
	}
	
}

//Finds max or min value of array

int Max_array(int a[], int num_elements)
{
	int i;
	int max=-32000;
	
	for (i=0; i<num_elements; i++)
	{
		if (a[i]>max)
		{
			max=a[i];
		}
	}
	return max;
}

int Min_array(int a[], int num_elements)
{
	int i;
	int min=32000;
	for (i=0; i<num_elements; i++)
	{
		if (a[i]<min)
		{
			min=a[i];
		}
	}
	return min;
}

bool No_corner(int x[], int y[])
{

	int min_x = Min_array(x,window_size);
	int max_x = Max_array(x,window_size);
	
	int min_y = Min_array(y,window_size);
	int max_y = Max_array(y,window_size);
	
	int max_diff_x = abs(max_x-min_x);
	int max_diff_y = abs(max_y-min_y);
	
	if((max_diff_x >= allowed_point_difference_Corner) && (max_diff_y >= allowed_point_difference_Corner))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool X_line(int x[], int y[])
{
	//int maxDiffAllowed = 10;
	
	int min_x = Min_array(x,window_size);
	int max_x = Max_array(x,window_size);
	
	int min_y = Min_array(y,window_size);
	int max_y = Max_array(y,window_size);
	
	int max_diff_x = abs(max_x-min_x);
	int max_diff_y = abs(max_y-min_y);
	
	if((max_diff_x <= allowed_point_difference_XorY) && (max_diff_y > allowed_point_difference_XorY))
	{
		return true;
	}
	else if((max_diff_x >= allowed_point_difference_XorY) && (max_diff_y < allowed_point_difference_XorY))
	{
		return false;
	}
	else
	{
		return false;
	}
	
}


//Returns the x tile coordinate. If it returns 3, the data window is bad and no tile should be added to the map
int Match_tile_x(int x[],int y[])
{
	int coord_x;
	
	int bestmatch_x = match_x();
	int next_best_match_x = Match_x_next(bestmatch_x);
	int avg_best_x = (bestmatch_x+next_best_match_x)/2;
	
	if ((X_line(x,y)) && (No_corner(x,y)))
	{
		//Put tile on the correct side of the detected line
		if(robot_pos.x/10<bestmatch_x)
		{
			coord_x = bestmatch_x + 20; 
		}
		else if(robot_pos.x/10>bestmatch_x)
		{
			coord_x = bestmatch_x - 20;
		}
		else
		{
			return 1; //Shouldn't happen #1
		}
	}
	else if((!X_line(x,y)) && (No_corner(x,y)))
	{
		if(avg_best_x > bestmatch_x)
		{
			coord_x = bestmatch_x + 20;
		}
		else if(avg_best_x < bestmatch_x)
		{
			coord_x = bestmatch_x - 20;
		}
		else
		{
			return 2; //Shouldn't happen #2
		}
	}
	else
	{
		return 3; //This can happen: bad data, corner etc. 
	}
	
	return coord_x;
}

//Returns the y tile coordinate. If it returns 3, the data window is bad and no tile should be added to the map
int Match_tile_y(int x[],int y[])
{
	int coord_y;
	
	int best_match_y = match_y();
	int next_best_match_y = Match_y_next(best_match_y);
	int avg_best_y = (best_match_y+next_best_match_y)/2;
	
	if ((X_line(x,y)) && (No_corner(x,y)))
	{
		//Put tile on the correct side of the detected line
		if(avg_best_y > best_match_y)
		{
			coord_y = best_match_y + 20;
		}
		else if(avg_best_y < best_match_y)
		{
			coord_y = best_match_y - 20;
		}
		else
		{
			return 2; //Shouldn't happen #2
		}
	}
	else if((!X_line(x,y)) && (No_corner(x,y)))
	{
		if(robot_pos.y/10<best_match_y)
		{
			coord_y = best_match_y + 20;
		}
		else if(robot_pos.y/10>best_match_y)
		{
			coord_y = best_match_y - 20;
		}
		else
		{
			return 1; //Shouldn't happen #1
		}
	}
	else
	{
		return 3; //Shouldn't happen #3. Bad data, corner etc.
	}
	
	return coord_y;
}




void Update_map(int x[],int y[])
{
	avg_x_per_sample = avg_array(x,window_size);
	avg_y_per_sample = avg_array(y,window_size);
	
	int x_tile_rob=Match_tile_x(x,y);
	int y_tile_rob=Match_tile_y(x,y);
	
	
	
	
	if((x_tile_rob!=3)&&(y_tile_rob!=3)&&(x_tile_rob!=2)&&(y_tile_rob!=2)&&(x_tile_rob!=1)&&(y_tile_rob!=1))
	{
		int x_tile_glob=Convert_rob_loc_map_glob_x(x_tile_rob);
		int y_tile_glob=Convert_rob_loc_map_glob_y(y_tile_rob);
		int newVal = 0;
		
		//Add +1 for each time the tile is detected
		int current_tile_value = Get_tile(x_tile_glob,y_tile_glob);
		
		if(current_tile_value == 0)
		{
			newVal = 5;
			Set_tile(x_tile_glob, y_tile_glob, newVal);
		}

		else if(current_tile_value < 255 && current_tile_value != 1 && current_tile_value != 2)
		{
			newVal = current_tile_value + 1;
		    Set_tile(x_tile_glob, y_tile_glob, newVal);
		}

		

	}
}
