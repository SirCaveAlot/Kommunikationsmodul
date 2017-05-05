/*
 * global_variables.h
 *
 * Created: 5/3/2017 9:06:29 AM
 *  Author: andan879
 */ 


#ifndef GLOBAL_VARIABLES_H_
#define GLOBAL_VARIABLES_H_

typedef struct coordinates {
	double x;
	double y;
}coordinates;

extern coordinates robot_pos;

extern int map_array[28][29];

extern int window_size;

extern int allowed_point_difference_XorY;
extern int allowed_point_difference_Corner;


#endif /* GLOBAL_VARIABLES_H_ */