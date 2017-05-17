/*
 * Main_array.h
 *
 * Created: 2017-05-09 11:30:29
 *  Author: Deep
 */ 


#ifndef MAIN_ARRAY_H_
#define MAIN_ARRAY_H_

extern uint8_t main_node_array[150][3];
extern uint8_t node_counter;
extern uint8_t last_node_counter;

void Main_array_init(uint8_t, uint8_t);

void Load_row_in_main_array(uint8_t, uint8_t, uint8_t);

void Transfer_to_main_array();

void Shortest_path_in_main_array();

bool Check_if_adjacent_node(uint8_t, uint8_t, uint8_t, uint8_t);

#endif /* MAIN_ARRAY_H_ */