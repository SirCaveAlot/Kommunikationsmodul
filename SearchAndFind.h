﻿/*
 * SearchAndFind.h
 *
 * Created: 5/12/2017 2:53:24 PM
 *  Author: marwa079
 */ 


#ifndef SEARCHANDFIND_H_
#define SEARCHANDFIND_H_

void robot_turn_right();
void robot_turn_left();
void robot_turn_around();

extern bool finished_with_drive_nearest_path;
//extern int right_distance;
//extern int left_distance;

extern uint8_t start_position_x;
extern uint8_t start_position_y;
extern uint8_t end_position_x;
extern uint8_t end_position_y;

bool detect_wall(int next_xposition, int next_yposition);

void robot_move();

void robot_move_distance(int number_of_steps);

void robot_keep_right();

void set_coordinate_in_array(int y, int x, int value);

void set_coordinate_in_NP_array(int y, int x, int value);

void save_detected_walls_in_array();

void save_detected_outer_wall_in_array();

bool drive_nearest_path();

bool drive_back_nearest_path();

bool detect_path(int next_yposition,int next_xposition, int value);

void drive_to_distressed_and_return();

void nearest_path_to_array();

void robot_detect_outer_walls();

void find_unknown_cells_after_detection_of_outer_walls();

// int first_undiscovered_tile_y;
// 
// int first_undiscovered_tile_x;

#endif /* SEARCHANDFIND_H_ */