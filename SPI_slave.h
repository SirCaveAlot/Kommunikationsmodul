/*
 * SPI_slave.h
 *
 * Created: 4/9/2017 4:28:33 PM
 *  Author: jakpa844
 */ 


#ifndef SPI_SLAVE_H_
#define SPI_SLAVE_H_

void spi_init_slave ();

void Queue_Init();

void Queue_Put(uint8_t new);

void Queue_Get(uint8_t *old);

#endif /* SPI_SLAVE_H_ */