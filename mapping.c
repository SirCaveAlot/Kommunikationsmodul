/*
 * GccApplication2.c
 *
 * Created: 2017-04-19 18:24:06
 * Author : hyggan
 */ 
	
#include "mapping.h"
#include "matchtile.h"
#include "global_variables.h"

#include <avr/io.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define pi  3.14159
#define val pi/180
#define Marcus 360/1000
#define marcus_to_radian pi/500

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
  
//------------------------------------Testcase 6 - en array på 20 element alla med (14,14), (14,13), (13,14), (14,15) -funkar -----------------------------------------

//------------------------------------Testcase 7- kollar att om vi har ett hörn så skapar vi ingen linje (ingen tile läggs till)  -----------------------------------------

//------------------------------------Testcase 8- kollar att om vi har ett hörn så skapar vi ingen linje (ingen tile läggs till)  -----------------------------------------

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
int size = 4000;
// double rcv_radius[5] = {28.28,24,20,24,35.28};
// double rcv_theta[5] = {315,345,0,15,45};

// double x_coordinates[5];
// double y_coordinates[5];
int array_x[20];
int array_y[20];

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
	//Tar ut ett fönster på ett visst antal element och gör en vekotr av dem
	uint16_t vector_position = 0;
	// Om det finns mindre plats än window_size, ta bar ett fönster de element som finns kvar
	for(int index = 0; index < size + 1 - window_size; index++)
	{
		for(int i = 0; i < 2 * window_size; i++)
		{   
			vector_position = index + i;
			if (i % 2 == 0)
			{
				array_x[i / 2] = (int) (distance_array[vector_position] << 8 | distance_array[vector_position + 1]) *
				(cos(((angle_array[vector_position] << 8 | angle_array[vector_position + 1]) - 29 * 1000 / 360) * marcus_to_radian + (pi / 2))) + robot_pos.x;
				array_y[i / 2] = (int) (distance_array[vector_position] << 8 | distance_array[vector_position + 1]) *
				(sin(((angle_array[vector_position] << 8 | angle_array[vector_position + 1]) - 29 * 1000 / 360) * marcus_to_radian + (pi / 2))) + robot_pos.y;
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
