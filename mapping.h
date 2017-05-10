/*
 * IncFile1.h
 *
 * Created: 2017-04-19 18:25:40
 *  Author: hyggan
 */ 




#ifndef CONVERSION_H_v
#define CONVERSION_H_
#include "global_variables.h"

//-------------------------- Test Fuction --------------------------------------
	
//void DegreeToRadian(uint16_t array[]);

//void PolarToCartesian(uint16_t radius_array[], uint16_t theta_array[]);

//void RelativeToEffective (struct coordinates robot_position);
void Window ();
void Set_robot_position(double xpos, double ypos);
void Set_robot_angle_direction(double direction);
uint8_t Get_robot_direction();
uint16_t Wheelshifts_to_distance(uint8_t nr_of_wheel_shifts);
void update_robot_position(uint8_t nr_of_wheel_shifts);
uint8_t Get_robot_tile_x();
uint8_t Get_robot_tile_y();

// void Cartesian_window (struct coordinates robot_position, uint16_t radius_array[],
//  uint16_t theta_array[], int window_size, int size);





#endif /* CONVERSION_H_ */