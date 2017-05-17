/*
 * matchtile.h
 *
 * Created: 4/25/2017 10:47:35 AM
 *  Author: andan879
 */ 
#include "global_variables.h"

#include <stdint.h>
#include <stdbool.h>

#ifndef MATCHTILE_H_
#define MATCHTILE_H_

int line_array_x[30];
int line_array_y[29];

int Get_tile(int xcoord, int ycoord);
void Set_tile(uint8_t xcoord, uint8_t ycoord, uint8_t newTile);
int Convert_rob_loc_map_glob_x(int coordRobot);
int Convert_rob_loc_map_glob_y(int coordRobot);
void init_const();
int avg_array(int a[], int num_elements);
int match_x();
int match_y();
int Match_x_next(int dont_match);
int Match_y_next(int dont_match);
int Max_array(int a[], int num_elements);
int Min_array(int a[], int num_elements);
bool No_corner(int x[], int y[]);
bool X_line(int x[], int y[]);
int Match_tile_x(int x[],int y[]);
int Match_tile_y(int x[],int y[]);
void Update_map(int x[],int y[]);



#endif /* MATCHTILE_H_ */