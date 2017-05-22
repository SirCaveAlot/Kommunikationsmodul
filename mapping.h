/*
 * mapping.h
 *
 * Created: 5/11/2017 10:56:34 AM
 *  Author: andan879
 */ 

#ifndef MAPPING_H_
#define MAPPING_H_

void Calibrate_robot_position();
void Window ();
void Set_robot_position(int16_t xpos, int16_t ypos);
void Set_robot_angle_direction(uint8_t direction);
uint8_t Get_robot_direction();
uint8_t Wheelshifts_to_distance(uint8_t nr_of_wheel_shifts);
void update_robot_position(uint16_t nr_of_wheel_shifts);
uint8_t Get_robot_tile_x();
uint8_t Get_robot_tile_y();
void Right_side_detectable(uint8_t IR_data, uint8_t IR_back_data);
void Left_side_detectable(uint8_t IR_data);
void Front_side_detectable(uint8_t IR_data);
void Set_tile_from_ir();
void Set_peepz_in_da_needz();

volatile extern bool right_side_detected;
volatile extern bool left_side_detected;
volatile extern bool front_side_detected;

#endif /* MAPPING_H_ */