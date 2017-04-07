/*
 * AVRGCC1.c
 *
 * Created: 2017-03-25 16:41:22
 *  Author: hyggan
 */  


#include <avr/io.h>
#include <avr/interrupt.h>
#include "queue.h"
#include <stdbool.h>
#include "UART.h"

//volatile uint8_t data;

#define DD_MOSI PINB4;
#define DDR_SPI DDRB;

uint8_t data = 0;
uint8_t data2 = 0;
uint8_t data3 = 0;

uint8_t uartdata;

int count = 0;

//bool queued = false;

void spi_init_slave (void)
{
  DDRB=(1<<DDB6);               //MISO as OUTPUT
  SPCR=(1<<SPIE)|(1<<SPE);       //Enable SPI && interrupt enable bit
  SPDR=0;
}

//char mode;

void SPI_SlaveReceive(void)
{		
  /* Wait for reception complete */
 // while (!(SPSR & (1<<SPIF)))   ;
  /* Return data register */
  
  QueuePut(SPDR);
  SPDR = 0x01;
  
  //return    value;
}

ISR(SPI_STC_vect)
{
      
	//check the register for incoming data
  /*  incoming_byte = SPI_SlaveReceive(); //read the SPI data
	//fifo_write(spi_fifo, &incoming_byte, 1); //write the data to the fifo
	//fifo_read(spi_fifo, void * buf, 1)
	
	
	if (incoming_byte == 0b00011000)
	{
	    	PORTD |= (1<<PORTD3);
	}		
   */
	//SPI_SlaveReceive();
	count++;
	QueuePut(SPDR);
	//SPDR = 1;
}
     //execute any required ISR exit code maybe

ISR(USART0_RX_vect)
{
	uartdata = UDR0;
	
	QueueGet(&data);
	
	USART_Transmit('[');
	USART_Transmit(data);
	USART_Transmit(']');
	USART_Transmit('\n');
}

int main(void)
{
	DDRD = 0b00001010;
	DDRB |= (1<<DDB1);
	//static fifo_t * spi_fifo;
    spi_init_slave();  //Initialize slave SPI
	QueueInit();
	USART_Init();
	Interrupt_Init();
    sei();
    while(1)
    {

		/*
		if(count == 3)
		{
			QueueGet(&data);
			QueueGet(&data2);
			QueueGet(&data3);
			count = 0;
		}
		
		if (data == 3)
		{
			PORTB |= (1<<1);
		}
		
		if(data2 == 7)
		{		
			PORTD |= (1<<3);
		}
		
		if(data3 == 9)
		{
			PORTD &= ~(1<<3);
			
			PORTA &= ~(1<<1);
		}*/
    }
}
