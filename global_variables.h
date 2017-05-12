/*
 * global_variables.h
 *
 * Created: 5/3/2017 9:06:29 AM
 *  Author: andan879
 */ 

#include <stdint.h>

#ifndef GLOBAL_VARIABLES_H_
#define GLOBAL_VARIABLES_H_

extern volatile uint8_t distance_array[4000];
extern volatile uint8_t angle_array[4000];

extern volatile uint8_t distance_traveled;
extern uint8_t last_movement;

typedef struct coordinates
{
	int x;
	int y;
	float angle;
}coordinates;

extern coordinates robot_pos;

extern uint8_t map_array[28][29];

extern int window_size;

extern int allowed_point_difference_XorY;
extern int allowed_point_difference_Corner;

#endif /* GLOBAL_VARIABLES_H_ */