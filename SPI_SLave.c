/*
 * AVRGCC1.c
 *
 * Created: 2017-03-25 16:41:22
 *  Author: hyggan
 */  


#include <avr/io.h>
#include <avr/interrupt.h>
volatile uint8_t data;

#define DD_MOSI PINB4;
#define DDR_SPI DDRB;

void spi_init_slave (void)
{
 DDRB=(1<<DDB6);               //MISO as OUTPUT
 SPCR=(1<<SPIE)|(1<<SPE);       //Enable SPI && interrupt enable bit
 SPDR=0;
}

//char mode;

uint8_t SPI_SlaveReceive(void)
{		
  /* Wait for reception complete */
 // while (!(SPSR & (1<<SPIF)))   ;
  /* Return data register */
  
  uint8_t value = SPDR;
  SPDR = 0x01;
  
  return    value;
}

	


//annan
/*
char ReadByteSPI(char addr)
{
	SPDR = addr;					//Load byte to Data register
	while(!(SPSR & (1<<SPIF))); 	// Wait for transmission complete 
	addr=SPDR;
	return addr;
}
*/


typedef struct {
     char * buf;
     int head;
     int tail;
     int size;
} fifo_t;

static fifo_t * spi_fifo;

void spi_fifo_init(fifo_t * fifo){
     spi_fifo = fifo;
}

//This initializes the FIFO structure with the given buffer and size
void fifo_init(fifo_t * f, char * buf, int size){
     f->head = 0;
     f->tail = 0;
     f->size = size;
     f->buf = buf;
}

//This reads nbytes bytes from the FIFO
//The number of bytes read is returned
int fifo_read(fifo_t * f, void * buf, int nbytes){
     int i;
     char * p;
     p = buf;
     for(i=0; i < nbytes; i++){
          if( f->tail != f->head ){ //see if any data is available
               *p++ = f->buf[f->tail];  //grab a byte from the buffer
               f->tail++;  //increment the tail
               if( f->tail == f->size ){  //check for wrap-around
                    f->tail = 0;
               }
          } else {
               return i; //number of bytes read
          }
     }
     return nbytes;
}

//Writes nbytes  to the FIFO
//If the head runs in to the tail, not all bytes are written
//The number of bytes written is returned
int fifo_write(fifo_t * f, const void * buf, int nbytes){
     int i;
     const char * p;
     p = buf;
     for(i=0; i < nbytes; i++){
           //first check to see if there is space in the buffer
           if( (f->head + 1 == f->tail) ||
                ( (f->head + 1 == f->size) && (f->tail == 0) )){
                 return i; //no more room
           } else {
               f->buf[f->head] = *p++;
               f->head++;  //increment the head
               if( f->head == f->size ){  //check for wrap-around
                    f->head = 0;
               }
           }
     }
     return nbytes;
}



ISR(SPI_STC_vect)
{
	DDRD |= (1<<DDD3);
    char incoming_byte;     
	//check the register for incoming data
    incoming_byte = SPI_SlaveReceive(); //read the SPI data
	

	if (incoming_byte == 0b00011000)
	{
	    	PORTD = (1<<PORTD3);
	}		
    //fifo_write(spi_fifo, &incoming_byte, 1); //write the data to the fifo
}

     //execute any required ISR exit code maybe


int main(void)
{
	static fifo_t * spi_fifo;
    spi_init_slave();                             //Initialize slave SPI

    sei();
    while(1)
    {

    }
}