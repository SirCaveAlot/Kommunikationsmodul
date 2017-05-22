﻿/*
 * Positioning.h
 *
 * Created: 5/17/2017 8:37:06 AM
 *  Author: guswe541
 */ 

#ifndef POSITIONING_H_
#define POSITIONING_H_

extern int8_t xpos;
extern int8_t ypos;
extern int8_t direction;

void Robot_turn_right();

void Robot_turn_left();

void Robot_turn_around();

void Robot_set_direction(uint8_t next_direction);

void set_direction(int8_t next_direction);

void turn_right();

void turn_left();

void turn_around();

int8_t next_y_position(int8_t next_direction);

int8_t next_x_position(int8_t next_direction);

int8_t right_x_pos();

int8_t right_y_pos();

int8_t left_x_pos();

int8_t left_y_pos();

int8_t back_x_pos();

int8_t back_y_pos();

int8_t forward_to_left_x_pos();

int8_t forward_to_left_y_pos();

#endif /* POSITIONING_H_ */