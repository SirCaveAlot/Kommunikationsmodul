/*
 * Movement.h
 *
 * Created: 4/20/2017 2:08:53 PM
 *  Author: jakpa844
 */ 

#include <stdint.h>
#include <stdbool.h>

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

extern int Movement_Queue_In;
extern int Movement_Queue_Out;
extern int Movement_queue_length;

void Movement_Queue_Init(void);

void Movement_Queue_Put(uint8_t new);

void Movement_Queue_Get(uint8_t *old);

bool Movement_queue_full();

bool Movement_queue_empty();

#endif /* MOVEMENT_H_ */