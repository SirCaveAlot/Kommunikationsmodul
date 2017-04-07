/*
 * queue.h
 *
 * Created: 4/7/2017 10:18:23 AM
 *  Author: jakpa844
 */ 

#include <avr/io.h>
#ifndef QUEUE_H_
#define QUEUE_H_

void QueueInit(void);

void QueuePut(uint8_t new);

void QueueGet(uint8_t *old);


#endif /* QUEUE_H_ */