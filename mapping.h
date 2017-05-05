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

void Andreas_func(double array1[], double array2[]);	
	
void DegreeToRadian(double array[]);

void PolarToCartesian(double radius_array[], double theta_array[]);

void RelativeToEffective (struct coordinates robot_position);

void Window ();

void Cartesian_window (struct coordinates robot_position, double radius_array[],
 double theta_array[], int window_size, int size);





#endif /* CONVERSION_H_ */