/*
 * SPI_slave.h
 *
 * Created: 4/9/2017 4:28:33 PM
 *  Author: jakpa844
 */ 
#include <stdint.h>

#ifndef SPI_SLAVE_H_
#define SPI_SLAVE_H_

void spi_init_slave ();

void Queue_Init();

void Queue_Put(uint8_t new);

void Queue_Get(uint8_t *old);

bool Queue_empty();

bool Queue_full();

bool Queue_contains(int i);

void Queue_Remove();

uint8_t Queue_Peek(int i);

#endif /* SPI_SLAVE_H_ */