/*
 * ShortestPath.h
 *
 * Created: 2017-05-08 15:47:00
 *  Author: Deep
 */ 


#ifndef SHORTESTPATH_H_
#define SHORTESTPATH_H_

bool Check_node_if_wall(uint8_t, uint8_t);

uint8_t Node_already_visited(uint8_t, uint8_t);

bool Finished();

void Pathfinder();

bool Start_point_found(uint8_t, uint8_t);

uint8_t Find_y_end_position();

uint8_t Find_x_end_position();

#endif /* SHORTESTPATH_H_ */