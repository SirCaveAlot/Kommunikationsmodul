/*
 * fake_robot_code.h
 *
 * Created: 5/22/2017 6:39:35 PM
 *  Author: Gustaf Westerholm guswe541
 */ 
extern int xpos;
extern int ypos;
extern int direction;
extern uint8_t undiscovered_tiles_array[29][29];

void fake_robot_set_direction(int next_direction);

void fake_robot_turn_right();

void fake_robot_turn_left();

void fake_robot_move();

void nearest_path_to_array();

int fake_next_y_position(int next_direction);

int fake_next_x_position(int next_direction);

int fake_right_x_pos();

int fake_right_y_pos();

int fake_left_x_pos();

int fake_left_y_pos();

int fake_forward_to_left_x_pos();

int fake_forward_to_left_y_pos();


