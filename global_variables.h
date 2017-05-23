/*
 * global_variables.h
 *
 * Created: 5/3/2017 9:06:29 AM
 *  Author: andan879
 */ 

#include <stdint.h>
#include <stdbool.h>

#ifndef GLOBAL_VARIABLES_H_
#define GLOBAL_VARIABLES_H_

extern volatile uint8_t distance_array[4000];
extern volatile uint8_t angle_array[4000];

extern uint8_t mode;

extern uint8_t left_IR;
extern uint8_t right_IR;
extern uint8_t right_IR_back;
extern uint8_t front_IR;

extern int distance_counter;
extern int angle_counter;

extern bool pc_ready;
extern bool auto_control;
extern bool volatile running;
extern bool mode_changed;

extern uint8_t competition_mode;
extern uint8_t next_movement;

extern bool shortest_path_created;
extern bool nearest_path_driven;
extern bool nearest_path_driven_back;

extern uint8_t distance_traveled;
extern uint8_t volatile last_movement;

typedef struct coordinates
{
	int x;
	int y;
	float angle;
	uint8_t x_tile;
	uint8_t y_tile;
}coordinates;

extern coordinates robot_pos;

extern uint8_t map_array[28][29];

extern int window_size;

extern int allowed_point_difference_XorY;
extern int allowed_point_difference_Corner;

#endif /* GLOBAL_VARIABLES_H_ */